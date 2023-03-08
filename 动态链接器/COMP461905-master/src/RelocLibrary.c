#include <dlfcn.h> //turn to dlsym for help at fake load object
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <elf.h>
#include <link.h>
#include <string.h>

#include "Link.h"

// glibc version to hash a symbol
static uint_fast32_t
dl_new_hash(const char *s)
{
    uint_fast32_t h = 5381;
    for (unsigned char c = *s; c != '\0'; c = *++s)
        h = h * 33 + c;
    return h & 0xffffffff;
}

// find symbol `name` inside the symbol table of `dep`
void *symbolLookup(LinkMap *dep, const char *name)
{
    if (dep->fake)
    {
        void *handle = dlopen(dep->name, RTLD_LAZY);
        if (!handle)
        {
            fprintf(stderr, "relocLibrary error: cannot dlopen a fake object named %s", dep->name);
            abort();
        }
        dep->fakeHandle = handle;
        return dlsym(handle, name);
    }

    Elf64_Sym *symtab = (Elf64_Sym *)dep->dynInfo[DT_SYMTAB]->d_un.d_ptr;
    const char *strtab = (const char *)dep->dynInfo[DT_STRTAB]->d_un.d_ptr;

    uint_fast32_t new_hash = dl_new_hash(name);
    Elf64_Sym *sym;
    const Elf64_Addr *bitmask = dep->l_gnu_bitmask;
    uint32_t symidx;
    Elf64_Addr bitmask_word = bitmask[(new_hash / __ELF_NATIVE_CLASS) & dep->l_gnu_bitmask_idxbits];
    unsigned int hashbit1 = new_hash & (__ELF_NATIVE_CLASS - 1);
    unsigned int hashbit2 = ((new_hash >> dep->l_gnu_shift) & (__ELF_NATIVE_CLASS - 1));
    if ((bitmask_word >> hashbit1) & (bitmask_word >> hashbit2) & 1)
    {
        Elf32_Word bucket = dep->l_gnu_buckets[new_hash % dep->l_nbuckets];
        if (bucket != 0)
        {
            const Elf32_Word *hasharr = &dep->l_gnu_chain_zero[bucket];
            do
            {
                if (((*hasharr ^ new_hash) >> 1) == 0)
                {
                    symidx = hasharr - dep->l_gnu_chain_zero;
                    /* now, symtab[symidx] is the current symbol.
                       Hash table has done its job */
                    const char *symname = strtab + symtab[symidx].st_name;
                    if (!strcmp(symname, name))
                    {
                        Elf64_Sym *s = &symtab[symidx];
                        // return the real address of found symbol
                        return (void *)(s->st_value + dep->addr);
                    }
                }
            } while ((*hasharr++ & 1u) == 0);
        }
    }
    return NULL; // not this dependency
}

void RelocLibrary(LinkMap *lib, int mode)
{
    /* Your code here */
    // find the address of referred symbol (we already know)
    Elf64_Dyn *dyn = lib->dyn;
    Elf64_Rela *rela;
    int count = 0;
    if (dyn->d_tag == DT_NEEDED) // outer symbol relocation
    {
        void *handle = dlopen("libc.so.6", RTLD_LAZY);
        void *address = dlsym(handle, "printf");

        while (dyn->d_tag != DT_JMPREL && dyn->d_tag != DT_NULL)
            dyn++;
        if (dyn->d_tag == DT_NULL)
            return;
        rela = dyn->d_un.d_val;
        // add the address with addend and fill it
        Elf64_Addr *pos = rela->r_offset + lib->addr;
        *pos = (Elf64_Addr)address;
        return;
    }

//    // local symbol relocation
//    dyn = lib->dyn;
//    while (dyn->d_tag != DT_NULL)
//    {
//        if (dyn->d_tag == DT_RELA)
//            rela = (Elf64_Rela *)dyn->d_un.d_ptr; // start
//        if (dyn->d_tag == DT_RELACOUNT)
//            count = (int)dyn->d_un.d_val;
//        dyn++;
//    }
//
//    for (int i = 0; i < count; i++)
//    {
//        uint64_t *pos = (uint64_t *)(lib->addr + rela->r_offset);
//        *pos = (uint64_t)(lib->addr + rela->r_addend);
//        rela++;
//    }

    
}
