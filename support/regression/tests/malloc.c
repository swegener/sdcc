/* Simple malloc tests.
 */
#include <testfwk.h>
#include <stdlib.h>

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
  char *p;
  unsigned char i;

#if !defined(__gbz80) && !defined(__z80) && !defined(__GNUC__)
  init_dynamic_memory((MEMHEADER XDATA *)heap, sizeof(heap));

  p1 = malloc(200);
  ASSERT(p1 == NULL);
  LOG(("p1 == NULL when out of memory\n"));
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

  p = (char*)p2;
  for (i=0; i<20; i++, p++)
    *p = i;

  p2 = realloc(p2, 25);
  ASSERT(p2 != NULL);
#ifdef PORT_HOST
  LOG(("p2, after expanding realloc: %p\n", p2));
#else
  LOG(("p2, after expanding realloc: %u\n", (unsigned) p2));
#endif

  p = (char*)p2;
  for (i=0; i<20; i++, p++)
    ASSERT(*p == i);

  p2 = realloc(p2, 15);
  ASSERT(p2 != NULL);
#ifdef PORT_HOST
  LOG(("p2, after shrinking realloc: %p\n", p2));
#else
  LOG(("p2, after shrinking realloc: %u\n", (unsigned) p2));
#endif

  p = (char*)p2;
  for (i=0; i<15; i++, p++)
    ASSERT(*p == i);

  free(p2);

  p3 = malloc(10);
  ASSERT(p3 != NULL);
#ifdef PORT_HOST
  LOG(("p3, after freeing p2: %p\n", p3));
#else
  LOG(("p3, after freeing p2: %u\n", (unsigned) p3));
#endif
}
