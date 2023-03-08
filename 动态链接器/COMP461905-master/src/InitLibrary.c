#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include <stdint.h>

#include "Link.h"
#include "LoaderInternal.h"

void InitLibrary(LinkMap *lib)
{
    Elf64_Dyn *Dyn = lib->dyn;
    void (*init_func)(void) = NULL;
    void **init_addr; //???
    Elf64_Rela *rela;
    int fcount = 0;
    int lcount = 0;
    while (Dyn->d_tag != DT_NULL)
    {
        if (Dyn->d_tag == DT_RELA)
            rela = (Elf64_Rela *)(Dyn->d_un.d_val);
        if (Dyn->d_tag == DT_RELACOUNT)
            lcount = (int)(Dyn->d_un.d_val);
        if (Dyn->d_tag == DT_INIT)
            init_func = (void *)Dyn->d_un.d_ptr;
        if (Dyn->d_tag == DT_INIT_ARRAY)
            init_addr = (void *)Dyn->d_un.d_ptr;
        if (Dyn->d_tag == DT_INIT_ARRAYSZ)
            fcount = (int)Dyn->d_un.d_val;
        Dyn++;
    }

    for (int i = 0; i < lcount; ++i)
    {
        uint64_t *addr = lib->addr + rela->r_offset;
        *addr = lib->addr + rela->r_addend;
        rela++;
    }
    init_func();
    for (int i = 0; i < fcount / 8; i++)
    {
        void (*func)(void) = (void *)(*init_addr);
        func();
        init_addr++;
    }
}

