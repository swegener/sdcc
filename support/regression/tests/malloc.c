/* Simple malloc tests.
 */
#include <testfwk.h>
#include <malloc.h>

/* PENDING */
#if defined(__gbz80) || defined(__z80) || defined(__GNUC__)
#define XDATA
#else
#define XDATA xdata
#endif

XDATA char heap[100];

void
testMalloc(void)
{
  void XDATA *p1, *p2, *p3;
  
#if !defined(__gbz80) && !defined(__z80) && !defined(__GNUC__)
  init_dynamic_memory((MEMHEADER xdata *)heap, sizeof(heap));
#endif

  p1 = malloc(5);
  ASSERT(p1 != NULL);
#ifdef PORT_HOST
  LOG(("p1: %p\n", p1));
#else
  LOG(("p1: %u\n", (unsigned) p1));
#endif

  p2 = malloc(20);
  ASSERT(p2 != NULL);
#ifdef PORT_HOST
  LOG(("p2: %p\n", p2));
#else
  LOG(("p2: %u\n", (unsigned) p2));
#endif

  free(p2);

  p3 = malloc(10);
  ASSERT(p3 != NULL);
#ifdef PORT_HOST
  LOG(("p3, after freeing p2: %p\n", p3));
#else
  LOG(("p3, after freeing p2: %u\n", (unsigned) p3));
#endif
}
