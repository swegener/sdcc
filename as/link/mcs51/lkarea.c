/* lkarea.c */

/*
 * (C) Copyright 1989-1995
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 *
 *  3-Nov-97 JLH:
 *           - change lkparea to use a_type == 0 as "virgin area" flag
 * 02-Apr-98 JLH: add code to link 8051 data spaces
 */

#include <stdio.h>
#include <string.h>
#include "aslink.h"

/*)Module   lkarea.c
 *
 *  The module lkarea.c contains the functions which
 *  create and link together all area definitions read
 *  from the .rel file(s).
 *
 *  lkarea.c contains the following functions:
 *      VOID    lnkarea()
 *      VOID    lnksect()
 *      VOID    lkparea()
 *      VOID    newarea()
 *
 *  lkarea.c contains no global variables.
 */

/*)Function VOID    newarea()
 *
 *  The function newarea() creates and/or modifies area
 *  and areax structures for each A directive read from
 *  the .rel file(s).  The function lkparea() is called
 *  to find the area structure associated with this name.
 *  If the area does not yet exist then a new area
 *  structure is created and linked to any existing
 *  linked area structures. The area flags are copied
 *  into the area flag variable.  For each occurence of
 *  an A directive an areax structure is created and
 *  linked to the areax structures associated with this
 *  area.  The size of this area section is placed into
 *  the areax structure.  The flag value for all subsequent
 *  area definitions for the same area are compared and
 *  flagged as an error if they are not identical.
 *  The areax structure created for every occurence of
 *  an A directive is loaded with a pointer to the base
 *  area structure and a pointer to the associated
 *  head structure.  And finally, a pointer to this
 *  areax structure is loaded into the list of areax
 *  structures in the head structure.  Refer to lkdata.c
 *  for details of the structures and their linkage.
 *
 *  local variables:
 *      areax **halp        pointer to an array of pointers
 *      int     i           counter, loop variable, value
 *      char    id[]        id string
 *      int     narea       number of areas in this head structure
 *      areax * taxp        pointer to an areax structure
 *                          to areax structures
 *
 *  global variables:
 *      area    *ap         Pointer to the current
 *                          area structure
 *      areax   *axp        Pointer to the current
 *                          areax structure
 *      head    *hp         Pointer to the current
 *                          head structure
 *      int     lkerr       error flag
 *
 *  functions called:
 *      Addr_T  eval()      lkeval.c
 *      VOID    exit()      c_library
 *      int     fprintf()   c_library
 *      VOID    getid()     lklex.c
 *      VOID    lkparea()   lkarea.c
 *      VOID    skip()      lklex.c
 *
 *  side effects:
 *      The area and areax structures are created and
 *      linked with the appropriate head structures.
 *      Failure to allocate area or areax structure
 *      space will terminate the linker.  Other internal
 *      errors most likely caused by corrupted .rel
 *      files will also terminate the linker.
 */

/*
 * Create an area entry.
 *
 * A xxxxxx size nnnn flags mm
 *   |           |          |
 *   |           |          `--  ap->a_flag
 *   |           `------------- axp->a_size
 *   `-------------------------  ap->a_id
 *
 */
VOID
newarea()
{
    register int i, narea;
    struct areax *taxp;
    struct areax **halp;
    char id[NCPS];

    /*
     * Create Area entry
     */
    getid(id, -1);
    lkparea(id);
    /*
     * Evaluate area size
     */
    skip(-1);
    axp->a_size = eval();
    /*
     * Evaluate flags
     */
    skip(-1);
    i = 0;
    taxp = ap->a_axp;
    while (taxp->a_axp) {
        ++i;
        taxp = taxp->a_axp;
    }
    if (i == 0) {
        ap->a_flag = eval();
    } else {
        i = eval();
/*      if (i && (ap->a_flag != i)) { */
/*          fprintf(stderr, "Conflicting flags in area %8s\n", id); */
/*          lkerr++; */
/*      } */
    }
    /*
     * Evaluate area address
     */
    skip(-1);
    axp->a_addr = eval();
    /*
     * Place pointer in header area list
     */
    if (headp == NULL) {
        fprintf(stderr, "No header defined\n");
        lkexit(1);
    }
    narea = hp->h_narea;
    halp = hp->a_list;
    for (i=0; i < narea ;++i) {
        if (halp[i] == NULL) {
            halp[i] = taxp;
            return;
        }
    }
    fprintf(stderr, "Header area list overflow\n");
    lkexit(1);
}

/*)Function VOID    lkparea(id)
 *
 *      char *  id      pointer to the area name string
 *
 *  The function lkparea() searches the linked area structures
 *  for a name match.  If the name is not found then an area
 *  structure is created.  An areax structure is created and
 *  appended to the areax structures linked to the area structure.
 *  The associated base area and head structure pointers are
 *  loaded into the areax structure.
 *
 *  local variables:
 *      area *  tap         pointer to an area structure
 *      areax * taxp        pointer to an areax structure
 *
 *  global variables:
 *      area    *ap         Pointer to the current
 *                          area structure
 *      area    *areap      The pointer to the first
 *                          area structure of a linked list
 *      areax   *axp        Pointer to the current
 *                          areax structure
 *
 *  functions called:
 *      VOID *  new()       lksym()
 *      char *  strcpy()    c_library
 *      int     symeq()     lksym.c
 *
 *  side effects:
 *      Area and/or areax structures are created.
 *      Failure to allocate space for created structures
 *      will terminate the linker.
 */

VOID
lkparea(char *id)
{
    register struct area *tap;
    register struct areax *taxp;

    ap = areap;
    axp = (struct areax *) new (sizeof(struct areax));
    axp->a_addr = -1; /* default: no address yet */
    while (ap) {
        if (symeq(id, ap->a_id)) {
            taxp = ap->a_axp;
            while (taxp->a_axp)
                taxp = taxp->a_axp;
            taxp->a_axp = axp;
            axp->a_bap = ap;
            axp->a_bhp = hp;
            return;
        }
        ap = ap->a_ap;
    }
    ap = (struct area *) new (sizeof(struct area));
    if (areap == NULL) {
        areap = ap;
    } else {
        tap = areap;
        while (tap->a_ap)
            tap = tap->a_ap;
        tap->a_ap = ap;
    }
    ap->a_axp = axp;
    axp->a_bap = ap;
    axp->a_bhp = hp;
    strncpy(ap->a_id, id, NCPS);
    ap->a_addr = 0;
}

/*)Function VOID    lnkarea()
 *
 *  The function lnkarea() resolves all area addresses.
 *  The function evaluates each area structure (and all
 *  the associated areax structures) in sequence.  The
 *  linking process supports four (4) possible area types:
 *
 *  ABS/OVR -   All sections (each individual areax
 *          section) starts at the identical base
 *          area address overlaying all other
 *          areax sections for this area.  The
 *          size of the area is largest of the area
 *          sections.
 *
 *  ABS/CON -   All sections (each individual areax
 *          section) are concatenated with the
 *          first section starting at the base
 *          area address.  The size of the area
 *          is the sum of the section sizes.
 *
 *  NOTE:   Multiple absolute (ABS) areas are
 *          never concatenated with each other,
 *          thus absolute area A and absolute area
 *          B will overlay each other if they begin
 *          at the same location (the default is
 *          always address 0 for absolute areas).
 *
 *  REL/OVR -   All sections (each individual areax
 *          section) starts at the identical base
 *          area address overlaying all other
 *          areax sections for this area.  The
 *          size of the area is largest of the area
 *          sections.
 *
 *  REL/CON -   All sections (each individual areax
 *          section) are concatenated with the
 *          first section starting at the base
 *          area address.  The size of the area
 *          is the sum of the section sizes.
 *
 *  NOTE:   Relocatable (REL) areas are always concatenated
 *          with each other, thus relocatable area B
 *          (defined after area A) will follow
 *          relocatable area A independent of the
 *          starting address of area A.  Within a
 *          specific area each areax section may be
 *          overlayed or concatenated with other
 *          areax sections.
 *
 *
 *  If a base address for an area is specified then the
 *  area will start at that address.  Any relocatable
 *  areas defined subsequently will be concatenated to the
 *  previous relocatable area if it does not have a base
 *  address specified.
 *
 *  The names s_<areaname> and l_<areaname> are created to
 *  define the starting address and length of each area.
 *
 *  local variables:
 *      Addr_T  rloc        ;current relocation address
 *      char    temp[]      ;temporary string
 *      struct symbol   *sp ;symbol structure
 *
 *  global variables:
 *      area    *ap         Pointer to the current
 *                          area structure
 *      area    *areap      The pointer to the first
 *                          area structure of a linked list
 *
 *  functions called:
 *      int     fprintf()   c_library
 *      VOID    lnksect()   lkarea.c
 *      symbol *lkpsym()    lksym.c
 *      char *  strncpy()   c_library
 *      int     symeq()     lksym.c
 *
 *  side effects:
 *      All area and areax addresses and sizes are
 *      determined and saved in their respective
 *      structures.
 */

/*
 * Resolve all area addresses.
 */
VOID
lnkarea()
{
    Addr_T rloc[4];
    int  locIndex;
    char temp[NCPS];
    struct sym *sp;
    /*JCF: used to save the REG_BANK_[0-3] and SBIT_BYTES area pointers*/
    struct area *ta[5];
    int j;

    rloc[0] = rloc[1] = rloc[2] = rloc[3] = 0;
    ap = areap;
    while (ap) {
        if (ap->a_flag&A_ABS) {
            /*
             * Absolute sections
             */
            lnksect(ap);
        } else {
            /* Determine memory space */
            locIndex = 0;
            if (ap->a_flag & A_CODE) {
                locIndex = 1;
            }
            if (ap->a_flag & A_XDATA) {
                locIndex = 2;
            }
            if (ap->a_flag & A_BIT) {
                locIndex = 3;
            }
            /*
             * Relocatable sections
             */
            if (ap->a_type == 0) {  /* JLH */
                ap->a_addr = rloc[ locIndex ];
                ap->a_type = 1;
            }
            lnksect(ap);
            rloc[ locIndex ] = ap->a_addr + ap->a_size;
        }

        /*
         * Create symbols called:
         *  s_<areaname>    the start address of the area
         *  l_<areaname>    the length of the area
         */

        if (! symeq(ap->a_id, _abs_)) {
            strncpy(temp+2,ap->a_id,NCPS-2);
            *(temp+1) = '_';

            *temp = 's';
            sp = lkpsym(temp, 1);
            sp->s_addr = ap->a_addr ;
            /* sp->s_axp = ap->a_axp;  JLH: was NULL; */
            sp->s_type |= S_DEF;

            *temp = 'l';
            sp = lkpsym(temp, 1);
            sp->s_addr = ap->a_size;
            sp->s_axp = NULL;
            sp->s_type |= S_DEF;

        }

        /*JCF: Since area BSEG is defined just before BSEG_BYTES, use the bit size of BSEG
        to compute the byte size of BSEG_BYTES: */
        if (!strcmp(ap->a_id, "BSEG")) {
            ap->a_ap->a_axp->a_size += ((ap->a_addr + ap->a_size + 7)/8); /*Bits to bytes*/
        }
        else if (!strcmp(ap->a_id, "REG_BANK_0")) ta[0]=ap;
        else if (!strcmp(ap->a_id, "REG_BANK_1")) ta[1]=ap;
        else if (!strcmp(ap->a_id, "REG_BANK_2")) ta[2]=ap;
        else if (!strcmp(ap->a_id, "REG_BANK_3")) ta[3]=ap;
        else if (!strcmp(ap->a_id, "BSEG_BYTES"))
        {
            ta[4]=ap;
            for(j=4; j>1; j--)
            {
                /*If upper register banks are not used roll back the relocation counter*/
                if ( (ta[j]->a_size==0) && (ta[j-1]->a_size==0) )
                {
                    rloc[0]-=8;
                }
                else break;
            }
        }
        ap = ap->a_ap;
    }
}

/*)Function VOID    lnksect()
 *
 *      area *  tap         pointer to an area structure
 *
 *  The function lnksect() is the function called by
 *  lnkarea() to resolve the areax addresses.  Refer
 *  to the function lnkarea() for more detail. Pageing
 *  boundary and length errors will be reported by this
 *  function.
 *
 *  local variables:
 *      Addr_T  size        size of area
 *      Addr_T  addr        address of area
 *      areax * taxp        pointer to an areax structure
 *
 *  global variables:
 *      int     lkerr       error flag
 *
 *  functions called:
 *      none
 *
 *  side effects:
 *      All area and areax addresses and sizes area determined
 *      and linked into the structures.
 */

VOID
lnksect(register struct area *tap)
{
    register Addr_T size, addr;
    register struct areax *taxp;

    size = 0;
    addr = tap->a_addr;
#if 0
    if ((tap->a_flag&A_PAG) && (addr & 0xFF)) {
        fprintf(stderr,
        "\n?ASlink-Warning-Paged Area %8s Boundary Error\n", tap->a_id);
        lkerr++;
    }
#endif
    taxp = tap->a_axp;
    if (tap->a_flag&A_OVR) {
        /*
         * Overlayed sections
         */
        while (taxp) {
            taxp->a_addr = addr;
            if (taxp->a_size > size)
                size = taxp->a_size;
            taxp = taxp->a_axp;
        }
    } else {
        /*
         * Concatenated sections
         */
        while (taxp) {
            taxp->a_addr = addr;
            addr += taxp->a_size;
            size += taxp->a_size;
            taxp = taxp->a_axp;
        }
    }
    tap->a_size = size;
    if ((tap->a_flag&A_PAG) && (size > 256)) {
        fprintf(stderr,
        "\n?ASlink-Warning-Paged Area %8s Length Error\n", tap->a_id);
        lkerr++;
    }
    if ((tap->a_flag&A_PAG) && (tap->a_size) &&
        ((tap->a_addr & 0xFFFFFF00) != ((addr-1) & 0xFFFFFF00)))
    {
        fprintf(stderr,
        "\n?ASlink-Warning-Paged Area %8s Boundary Error\n", tap->a_id);
        lkerr++;
    }
}

Addr_T lnksect2 (struct area *tap, int locIndex);
char idatamap[256];
unsigned long codemap[524288];
unsigned long xdatamap[131072];

/*Modified version of the functions for packing variables in internal data memory*/
VOID lnkarea2 (void)
{
    Addr_T rloc[4]={0, 0, 0, 0};
    Addr_T gs_size = 0;
    int  locIndex;
    char temp[NCPS];
    struct sym *sp;
    int j;
    struct area *dseg_ap = NULL;
    struct area *abs_ap = NULL;
    struct area *gs0_ap = NULL;
    struct sym *sp_dseg_s=NULL, *sp_dseg_l=NULL;

    for(j=0; j<256; j++) idatamap[j]=' ';
    memset(codemap, 0, sizeof(codemap));
    memset(xdatamap, 0, sizeof(xdatamap));

    /* first sort all absolute areas to the front */
    ap = areap;
    /* no need to check first area, it's in front anyway */
    while (ap && ap->a_ap)
    {
        if (ap->a_ap->a_flag & A_ABS)
        {/* next area is absolute, move it to front,
            reversed sequence is no problem for absolutes */
            abs_ap = ap->a_ap;
            ap->a_ap = abs_ap->a_ap;
            abs_ap->a_ap = areap;
            areap = abs_ap;
        }
        else
        {
            ap = ap->a_ap;
        }
    }

    /* next accumulate all GSINITx/GSFINAL area sizes
       into GSINIT so they stay together */
    ap = areap;
    while (ap)
    {
        if (!strncmp(ap->a_id, "GS", 2))
        {/* GSxxxxx area */
            if (ap->a_size == 0)
            {
                axp = ap->a_axp;
                while (axp)
                {
                    ap->a_size += axp->a_size;
                    axp = axp->a_axp;
                }
            }
            gs_size += ap->a_size;
            if (!strcmp(ap->a_id, "GSINIT0"))
            {/* GSINIT0 area */
                gs0_ap = ap;
            }
        }
        ap = ap->a_ap;
    }
    if (gs0_ap)
        gs0_ap->a_size = gs_size;

    ap = areap;
    while (ap)
    {
        /* Determine memory space */
             if (ap->a_flag & A_CODE)  locIndex = 1;
        else if (ap->a_flag & A_XDATA) locIndex = 2;
        else if (ap->a_flag & A_BIT)   locIndex = 3;
        else locIndex = 0;

        if (ap->a_flag & A_ABS) /* Absolute sections */
        {
            lnksect2(ap, locIndex);
        }
        else /* Relocatable sections */
        {
            if (ap->a_type == 0)
            {
                ap->a_addr = rloc[locIndex];
                ap->a_type = 1;
            }

            rloc[locIndex] = lnksect2(ap, locIndex);
        }

        /*
         * Create symbols called:
         *  s_<areaname>    the start address of the area
         *  l_<areaname>    the length of the area
         */

        if (! symeq(ap->a_id, _abs_))
        {
            strncpy(temp+2,ap->a_id,NCPS-2);
            *(temp+1) = '_';

            *temp = 's';
            sp = lkpsym(temp, 1);
            sp->s_addr = ap->a_addr ;
            sp->s_type |= S_DEF;
            if (!strcmp(ap->a_id, "DSEG")) sp_dseg_s=sp;

            *temp = 'l';
            sp = lkpsym(temp, 1);
            sp->s_addr = ap->a_size;
            sp->s_axp = NULL;
            sp->s_type |= S_DEF;
            if (!strcmp(ap->a_id, "DSEG")) sp_dseg_l=sp;
        }

        /*Since area BSEG is defined just before BSEG_BYTES, use the bit size of BSEG
        to compute the byte size of BSEG_BYTES: */
        if (!strcmp(ap->a_id, "BSEG"))
        {
            ap->a_ap->a_axp->a_size = ((ap->a_addr + ap->a_size + 7)/8); /*Bits to bytes*/
        }
        else if (!strcmp(ap->a_id, "DSEG"))
        {
            dseg_ap=ap; /*Need it later to set its correct size*/
        }
        ap = ap->a_ap;
    }

    /*Compute the size of DSEG*/
    if(dseg_ap!=NULL)
    {
        dseg_ap->a_addr=0;
        dseg_ap->a_size=0;
        for(j=0; j<0x80; j++) if(idatamap[j]!=' ') dseg_ap->a_size++;
    }
    if(sp_dseg_s!=NULL) sp_dseg_s->s_addr=0;
    if(sp_dseg_l!=NULL) sp_dseg_l->s_addr=dseg_ap->a_size;
}

static
Addr_T find_empty_space(Addr_T start, Addr_T size, unsigned long *map)
{
    int i, j, k;
    unsigned long mask, b;

    while (1)
    {
        Addr_T a = start;
        i = start >> 5;
        j = (start + size) >> 5;
        mask = -(1 << (start & 0x1F));

        while (i < j)
        {
            if (map[i] & mask)
            {
                k = 32;
                for (b=0x80000000; b!=0; b>>=1, k--)
                {
                    if (map[i] & b)
                      break;
                }
                start = a + k;
                break;
            }
            i++;
            mask = 0xFFFFFFFF;
            a += 32;
        }
        if (start > a)
          continue;

        mask &= (1 << ((start + size) & 0x1F)) - 1;
        if (map[i] & mask)
        {
            k = 32;
            for (b=0x80000000; b!=0; b>>=1, k--)
            {
                if (map[i] & b)
                    break;
            }
            start = (a & ~0x1F) + k;
        }
        if (start <= a)
          break;
    }
    return start;
}

static
Addr_T allocate_space(Addr_T start, Addr_T size, char* id, unsigned long *map)
{
    int i, j;
    unsigned long mask;
    Addr_T a = start;
    i = start >> 5;
    j = (start + size) >> 5;
    mask = -(1 << (start & 0x1F));

    while (i < j)
    {
        if (map[i] & mask)
        {
            fprintf(stderr, "memory overlap near 0x%X for %s\n", a, id);
        }
        map[i++] |= mask;
        mask = 0xFFFFFFFF;
        a += 32;
    }
    mask &= (1 << ((start + size) & 0x1F)) - 1;
    if (map[i] & mask)
    {
        fprintf(stderr, "memory overlap near 0x%X for %s\n", a, id);
    }
    map[i] |= mask;
    return start;
}

Addr_T lnksect2 (struct area *tap, int locIndex)
{
    register Addr_T size, addr;
    register struct areax *taxp;
    int j, k, ramlimit;
    char fchar=' ', dchar='a';
    char ErrMsg[]="?ASlink-Error-Could not get %d consecutive byte%s"
                  " in internal RAM for area %s.\n";

    tap->a_unaloc=0;

    /*Notice that only ISEG and SSEG can be in the indirectly addressable internal RAM*/
    if( (!strcmp(tap->a_id, "ISEG")) || (!strcmp(tap->a_id, "SSEG")) )
    {
        if((iram_size<=0)||(iram_size>0x100))
            ramlimit=0x100;
        else
            ramlimit=iram_size;
    }
    else
    {
        if((iram_size<=0)||(iram_size>0x80))
            ramlimit=0x80;
        else
            ramlimit=iram_size;
    }

    size = 0;
    addr = tap->a_addr;
#if 0
    if ((tap->a_flag&A_PAG) && (addr & 0xFF))
    {
        fprintf(stderr,
          "\n?ASlink-Warning-Paged Area %8s Boundary Error\n", tap->a_id);
        lkerr++;
    }
#endif
    taxp = tap->a_axp;

    /*Use a letter to identify each area in the internal RAM layout map*/
    if (locIndex==0)
    {
        /**/ if(!strcmp(tap->a_id, "DSEG"))
            fchar='D'; /*It will be converted to letters 'a' to 'z' later for each areax*/
        else if(!strcmp(tap->a_id, "ISEG"))
            fchar='I';
        else if(!strcmp(tap->a_id, "SSEG"))
            fchar='S';
        else if(!strcmp(tap->a_id, "OSEG"))
            fchar='Q';
        else if(!strcmp(tap->a_id, "REG_BANK_0"))
            fchar='0';
        else if(!strcmp(tap->a_id, "REG_BANK_1"))
            fchar='1';
        else if(!strcmp(tap->a_id, "REG_BANK_2"))
            fchar='2';
        else if(!strcmp(tap->a_id, "REG_BANK_3"))
            fchar='3';
        else if(!strcmp(tap->a_id, "BSEG_BYTES"))
            fchar='B';
        else if(!strcmp(tap->a_id, "BIT_BANK"))
            fchar='T';
        else
            fchar=' ';/*???*/
    }
    else if (locIndex == 1)
    {
        /**/ if(!strcmp(tap->a_id, "GSINIT"))
            fchar='G';
    }
    else if (locIndex == 2)
    {
        /**/ if(!strcmp(tap->a_id, "XSTK"))
            fchar='K';
    }

    if (tap->a_flag&A_OVR) /* Overlayed sections */
    {
        while (taxp)
        {
            if ( (fchar=='0')||(fchar=='1')||(fchar=='2')||(fchar=='3') ) /*Reg banks*/
            {
                addr=(fchar-'0')*8;
                taxp->a_addr=addr;
                size=taxp->a_size;
                for(j=addr; (j<(int)(addr+taxp->a_size)) && (j<ramlimit); j++)
                    idatamap[j]=fchar;
            }
            else if( (fchar=='S') || (fchar=='Q') ) /*Overlay and stack in internal RAM*/
            {
                /*Find the size of the space currently used for this areax overlay*/
                for(j=0, size=0; j<ramlimit; j++)
                    if(idatamap[j]==fchar) size++;

                if( (fchar=='S') && (stacksize==0) )
                {
                   /*Search for the largest space available and use it for stack*/
                    for(j=0, k=0, taxp->a_size=0; j<ramlimit; j++)
                    {
                        if(idatamap[j]==' ')
                        {
                            if((++k)>(int)taxp->a_size) taxp->a_size=k;
                        }
                        else
                        {
                            k=0;
                        }
                    }
                    stacksize=taxp->a_size;
                }

                /*If more space required, release the previously allocated areax in
                internal RAM and search for a bigger one*/
                if((int)taxp->a_size>size)
                {
                    size=(int)taxp->a_size;

                    for(j=0; j<ramlimit; j++)
                        if(idatamap[j]==fchar) idatamap[j]=' ';

                    /*Search for a space large enough in data memory for this overlay areax*/
                    for(j=0, k=0; j<ramlimit; j++)
                    {
                        if(idatamap[j]==' ')
                            k++;
                        else
                            k=0;
                        if(k==(int)taxp->a_size) break;
                    }

                    if(k==(int)taxp->a_size)
                    {
                        taxp->a_addr = j-k+1;
                        if(addr<(unsigned int)ramlimit)
                        {
                            for(j=ramlimit-1; (j>=0)&&(idatamap[j]==' '); j--);
                            if(j>=0) addr=j+1;
                        }
                    }

                    /*Mark the memory used for overlay*/
                    if(k==(int)taxp->a_size)
                    {
                        for(j=taxp->a_addr; (j<(int)(taxp->a_addr+taxp->a_size)) && (j<ramlimit); j++)
                            idatamap[j]=fchar;

                        /*Set the new size of the data memory area*/
                        size=ramlimit-addr;
                    }
                    else /*Couldn't find a chunk big enough: report the problem.*/
                    {
                        tap->a_unaloc=taxp->a_size;
                        fprintf(stderr, ErrMsg, taxp->a_size, taxp->a_size>1?"s":"", tap->a_id);
                        lkerr++;
                    }
                }

                for(j=0; j<ramlimit; j++)
                {
                    if (idatamap[j]==fchar)
                    {
                        addr=j;
                        tap->a_addr=addr;
                        taxp->a_addr=addr;
                        break;
                    }
                }
            }
            else if (fchar=='T') /*Bit addressable bytes in internal RAM*/
            {
                /*Find the size of the space currently used for this areax overlay*/
                for(j=0x20, size=0; j<0x30; j++)
                    if(idatamap[j]==fchar) size++;

                /*If more space required, release the previously allocated areax in
                internal RAM and search for a bigger one*/
                if((int)taxp->a_size>size)
                {
                    size=(int)taxp->a_size;

                    for(j=0x20; j<0x30; j++)
                        if(idatamap[j]==fchar) idatamap[j]=' ';

                    /*Search for a space large enough in data memory for this overlay areax*/
                    for(j=0x20, k=0; j<0x30; j++)
                    {
                        if(idatamap[j]==' ')
                            k++;
                        else
                            k=0;
                        if(k==(int)taxp->a_size) break;
                    }

                    if(k==(int)taxp->a_size)
                    {
                        taxp->a_addr = j-k+1;
                        if(addr<(unsigned int)0x30)
                        {
                            for(j=0x2F; (j>=0x20)&&(idatamap[j]==' '); j--);
                            if(j>=0x20) addr=j+1;
                        }
                    }

                    /*Mark the memory used for overlay*/
                    if(k==(int)taxp->a_size)
                    {
                        for(j=taxp->a_addr; (j<(int)(taxp->a_addr+taxp->a_size)) && (j<0x30); j++)
                            idatamap[j]=fchar;

                        /*Set the new size of the data memory area*/
                        size=ramlimit-addr;
                    }
                    else /*Couldn't find a chunk big enough: report the problem.*/
                    {
                        tap->a_unaloc=taxp->a_size;
                        fprintf(stderr, ErrMsg, taxp->a_size, taxp->a_size>1?"s":"", tap->a_id);
                        lkerr++;
                    }
                }

                for(j=0x20; j<0x30; j++)
                {
                    if (idatamap[j]==fchar)
                    {
                        addr=j;
                        tap->a_addr=addr;
                        taxp->a_addr=addr;
                        break;
                    }
                }
            }
            else /*Overlay areas not in internal ram*/
            {
                taxp->a_addr = addr;
                if (taxp->a_size > size) size = taxp->a_size;
            }
            taxp = taxp->a_axp;
        }
    }
    else if (tap->a_flag & A_ABS) /* Absolute sections */
    {
        while (taxp)
        {
            if (locIndex == 0)
            {
                for (j=taxp->a_addr; (j<(int)(taxp->a_addr+taxp->a_size)) && (j<ramlimit); j++)
                    idatamap[j] = 'A';
            }
            if (locIndex == 1)
            {
                allocate_space(taxp->a_addr, taxp->a_size, tap->a_id, codemap);
            }
            if (locIndex == 2)
            {
                allocate_space(taxp->a_addr, taxp->a_size, tap->a_id, xdatamap);
            }
            taxp->a_addr = 0; /* reset to zero so relative addresses become absolute */
            size += taxp->a_size;
            taxp = taxp->a_axp;
        }
    }
    else /* Concatenated sections */
    {
        if ((locIndex == 1) && tap->a_size)
        {
            addr = find_empty_space(addr, tap->a_size, codemap);
        }
        if ((locIndex == 2) && tap->a_size)
        {
            addr = find_empty_space(addr, tap->a_size, xdatamap);
        }
        while (taxp)
        {
            if( (fchar=='D') || (fchar=='I') )
            {
                if(taxp->a_size)
                {
                    /*Search for a space large enough in internal RAM for this areax*/
                    for(j=0, k=0; j<ramlimit; j++)
                    {
                        if(idatamap[j]==' ')
                            k++;
                        else
                            k=0;
                        if(k==(int)taxp->a_size) break;
                    }

                    if(k==(int)taxp->a_size)
                    {
                        taxp->a_addr = j-k+1;
                        if(addr<(unsigned int)ramlimit)
                        {
                            for(j=ramlimit-1; (j>=0)&&(idatamap[j]==' '); j--);
                            if(j>=0) addr=j+1;
                            size=ramlimit-addr;
                        }

                        for(j=taxp->a_addr; (j<(int)(taxp->a_addr+taxp->a_size)) && (j<ramlimit); j++)
                            idatamap[j]=(fchar=='D')?dchar:fchar;
                        if((taxp->a_size>0)&&(fchar=='D'))dchar++;
                        if((dchar<'a')||(dchar>'z')) dchar='D'; /*Ran out of letters?*/
                    }
                    else /*We are in trouble, there is not enough memory for an areax chunk*/
                    {
                        taxp->a_addr = addr;
                        addr += taxp->a_size;
                        size += taxp->a_size;
                        tap->a_unaloc+=taxp->a_size;
                        fprintf(stderr, ErrMsg, taxp->a_size, taxp->a_size>1?"s":"", tap->a_id);
                        lkerr++;
                    }
               }
               taxp = taxp->a_axp;
            }
            else if(fchar=='B')
            {
                if(taxp->a_size!=0)
                {
                    for(j=addr; j<((int)(addr+taxp->a_size)); j++)
                        idatamap[j]=fchar;
                }

                taxp->a_addr = addr;
                addr += taxp->a_size;
                size += taxp->a_size;
                taxp = taxp->a_axp;
            }
            else /*For concatenated BIT, CODE, and XRAM areax's*/
            {
                if((fchar=='K') && (taxp->a_size == 1))
                {
                    taxp->a_size = 256-(addr & 0xFF);
                }
                //find next unused address now
                if ((locIndex == 1) && taxp->a_size)
                {
                    addr = find_empty_space(addr, taxp->a_size, codemap);
                    allocate_space(addr, taxp->a_size, tap->a_id, codemap);
                }
                if ((locIndex == 2) && taxp->a_size)
                {
                    addr = find_empty_space(addr, taxp->a_size, xdatamap);
                    allocate_space(addr, taxp->a_size, tap->a_id, xdatamap);
                }
                taxp->a_addr = addr;
                addr += taxp->a_size;
                size += taxp->a_size;
                taxp = taxp->a_axp;
            }
        }
    }
    tap->a_size = size;

    if ((tap->a_flag&A_PAG) && (size > 256))
    {
        fprintf(stderr,
        "\n?ASlink-Warning-Paged Area %8s Length Error\n", tap->a_id);
        lkerr++;
    }
    if ((tap->a_flag&A_PAG) && (tap->a_size) &&
        ((tap->a_addr & 0xFFFFFF00) != ((addr-1) & 0xFFFFFF00)))
    {
        fprintf(stderr,
        "\n?ASlink-Warning-Paged Area %8s Boundary Error\n", tap->a_id);
        lkerr++;
    }
    return addr;
}
