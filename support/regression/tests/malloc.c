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
  LOG(("p1: %u\n", p1));

  p2 = malloc(20);
  ASSERT(p2 != NULL);
  LOG(("p2: %u\n", p2));

  free(p2);

  p3 = malloc(10);
  ASSERT(p3 != NULL);
  LOG(("p3, after freeing p2: %u\n", p3));
}
