#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <elf.h>
#include <unistd.h> //for getpagesize
#include <sys/mman.h>
#include <fcntl.h>

#include "Link.h"
#include "LoaderInternal.h"

#define ALIGN_DOWN(base, size) ((base) & -((__typeof__(base))(size)))
#define ALIGN_UP(base, size) ALIGN_DOWN((base) + (size)-1, (size))

static const char *sys_path[] = {
    "/usr/lib/x86_64-linux-gnu/",
    "/lib/x86_64-linux-gnu/",
    ""};

static const char *fake_so[] = {
    "libc.so.6",
    "ld-linux.so.2",
    ""};

static void setup_hash(LinkMap *l)
{
    uint32_t *hash;

    /* borrowed from dl-lookup.c:_dl_setup_hash */
    Elf32_Word *hash32 = (Elf32_Word *)l->dynInfo[DT_GNU_HASH_NEW]->d_un.d_ptr;
    l->l_nbuckets = *hash32++;
    Elf32_Word symbias = *hash32++;
    Elf32_Word bitmask_nwords = *hash32++;

    l->l_gnu_bitmask_idxbits = bitmask_nwords - 1;
    l->l_gnu_shift = *hash32++;

    l->l_gnu_bitmask = (Elf64_Addr *)hash32;
    hash32 += 64 / 32 * bitmask_nwords;

    l->l_gnu_buckets = hash32;
    hash32 += l->l_nbuckets;
    l->l_gnu_chain_zero = hash32 - symbias;
}

static void fill_info(LinkMap *lib)
{
    Elf64_Dyn *dyn = lib->dyn;
    Elf64_Dyn **dyn_info = lib->dynInfo;

    while (dyn->d_tag != DT_NULL)
    {
        if ((Elf64_Xword)dyn->d_tag < DT_NUM)
            dyn_info[dyn->d_tag] = dyn;
        else if ((Elf64_Xword)dyn->d_tag == DT_RELACOUNT)
            dyn_info[DT_RELACOUNT_NEW] = dyn;
        else if ((Elf64_Xword)dyn->d_tag == DT_GNU_HASH)
            dyn_info[DT_GNU_HASH_NEW] = dyn;
        ++dyn;
    }
#define rebase(tag)                                 \
    do                                              \
    {                                               \
        if (dyn_info[tag])                          \
            dyn_info[tag]->d_un.d_ptr += lib->addr; \
    } while (0)
    rebase(DT_SYMTAB);
    rebase(DT_STRTAB);
    rebase(DT_RELA);
    rebase(DT_JMPREL);
    rebase(DT_GNU_HASH_NEW); // DT_GNU_HASH
    rebase(DT_PLTGOT);
    rebase(DT_INIT);
    rebase(DT_INIT_ARRAY);
}

void *MapLibrary(const char *libpath)
{
    // set space for lib
    LinkMap *lib = malloc(sizeof(LinkMap));
    int fd = open(libpath, O_RDONLY);

    // read elfheader
    Elf64_Ehdr *elfh = malloc(sizeof(Elf64_Ehdr));
    pread(fd, elfh, sizeof(Elf64_Ehdr), 0);

    // read all segment header
    Elf64_Phdr *segh = malloc(sizeof(Elf64_Phdr) * elfh->e_phnum);
    pread(fd, segh, sizeof(Elf64_Phdr) * elfh->e_phnum, elfh->e_phoff);

    // load PT_LOAD into the buffer
    int first = 1;
    for (int i = 0; i < elfh->e_phnum; i++)
    {
        Elf64_Phdr *pt = &segh[i]; // get i th segment

        if (pt->p_type == PT_LOAD) // point to LOAD
        {
            int prot = 0;
            prot |= (pt->p_flags & PF_R) ? PROT_READ : 0;
            prot |= (pt->p_flags & PF_W) ? PROT_WRITE : 0;
            prot |= (pt->p_flags & PF_X) ? PROT_EXEC : 0;
            // NULL means "allow OS to pick up address for you"

            if (first)
            {
                size_t len = segh[elfh->e_phnum - 1].p_vaddr + segh[i].p_memsz - segh[i].p_vaddr;
                void *start_addr = mmap(NULL, ALIGN_UP(len, getpagesize()), prot,
                                        MAP_FILE | MAP_PRIVATE, fd, ALIGN_DOWN(pt->p_offset, getpagesize()));

                lib->addr = start_addr;
                first = 0;
            }
            else
            {
                Elf64_Addr start = ALIGN_DOWN(pt->p_vaddr + lib->addr, getpagesize());
                Elf64_Addr end = ALIGN_UP(pt->p_vaddr + lib->addr + pt->p_memsz, getpagesize());
                mmap((void *)start, end - start, prot, MAP_FILE | MAP_PRIVATE | MAP_FIXED, fd, ALIGN_DOWN(pt->p_offset, getpagesize()));
            }
        }
        else if (pt->p_type == PT_DYNAMIC)
            lib->dyn = lib->addr + pt->p_vaddr;
    }

    fill_info(lib);
    setup_hash(lib);

    return lib;
}