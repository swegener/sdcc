/*-----------------------------------------------------------------
    SDCChast.h - contains support routines for hashtables/sets .

    Written By - Sandeep Dutta . sandeep.dutta@usa.net (1998)

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2, or (at your option) any
    later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
    
    In other words, you are welcome to use, share and improve this program.
    You are forbidden to forbid anyone else to use, share and improve
    what you give them.   Help stamp out software-hoarding!  
-------------------------------------------------------------------------*/


#ifndef SDCCHASHT_H
#define SDCCHASHT_H

#ifdef _NO_GC

#define GC_malloc malloc
#define GC_free free
#define GC_realloc realloc
#define GC_malloc_atomic malloc

#else

#include "./gc/gc.h" 

#endif

#ifndef ALLOC

#define  ALLOC(x,sz) if (!(x = GC_malloc(sz)))                          \
         {                                                           \
            fprintf(stderr,"out of virtual memory %s , %d",__FILE__,__LINE__);\
            exit (1);                                                \
         }

#define  ALLOC_ATOMIC(x,sz) if (!(x = GC_malloc_atomic(sz)))                          \
         {                                                           \
            fprintf(stderr,"out of virtual memory %s , %d",__FILE__,__LINE__);\
            exit (1);                                                \
         }



#endif


/* hashtable item */
typedef struct hashtItem
{
    int key ;
    void *item ;
    struct hashtItem *next ;
} hashtItem ;

/* hashtable */
typedef struct hTab
{
    int size  ;             /* max number of items */
    int minKey;             /* minimum key value   */
    int maxKey ;            /* maximum key value */
    hashtItem **table ;     /* the actual table  */
    int currKey  ;          /* used for iteration */
    hashtItem *currItem ;   /* current item within the list */
    int nItems ;
} hTab ;



enum {
    DELETE_CHAIN = 1,
    DELETE_ITEM };


/*-----------------------------------------------------------------*/
/*	     Forward   definition    for   functions		   */
/*-----------------------------------------------------------------*/

/* hashtable related functions */
hTab        *newHashTable      (int);
void         hTabAddItem (hTab **, int , void * );
void         hTabDeleteItem (hTab **, int  ,
				  void *, int  ,
				  int (*compareFunc)(void *,void *));
int          hTabIsInTable (hTab *, int , void * , 
				 int (*compareFunc)(void *,void *));
void         *hTabFirstItem (hTab *, int *);
void         *hTabNextItem (hTab *, int *);
hTab         *hTabFromTable (hTab *);
int          isHtabsEqual (hTab *,hTab *, int (*compareFunc)(void *,void *));
hashtItem    *hTabSearch  (hTab *, int );
void         *hTabItemWithKey(hTab *,int );
void         hTabAddItemIfNotP(hTab **,int, void *);
void         hTabDeleteAll(hTab *);
void        *hTabFirstItemWK (hTab *htab, int wk);
void        *hTabNextItemWK (hTab *htab );
void         hTabClearAll (hTab *htab);


#endif
