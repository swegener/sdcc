/** Test results of sizeof operator for unusual cases
    size: 1, 2
    test : 0, 1, 2, 3, 4
    vol : 0, 1
*/

#include <testfwk.h>
#ifdef __sun__
#include <inttypes.h>
#else
#include <stdint.h>
#endif

#define VAR_SIZE ({size})
#define TEST ({test})
#define TEST_VOLATILE ({vol})

// Test is split in several parts to be smaller and for debugging purposes
#if TEST == 0
  #define TEST_BASIC
  #define TEST_ASSING
  #define TEST_POINTERS
#elif TEST == 1
  #define TEST_LITERAL
#elif TEST == 2
  #define TEST_VARIABLE
#elif TEST == 3
  #define TEST_LITERAL_BOOL_OP
#elif TEST == 4
  #define TEST_VARIABLE_BOOL_OP
#else
  #error "Unkwnown test"
#endif

#if TEST_VOLATILE == 1
  #define VAR_QUALIFIER volatile
#else
  #define VAR_QUALIFIER
#endif

// Variables for the test.
VAR_QUALIFIER uint8_t c, c2;
VAR_QUALIFIER int16_t i, i2;

VAR_QUALIFIER uint8_t * VAR_QUALIFIER  pc;
VAR_QUALIFIER int16_t * VAR_QUALIFIER  pi;

// Main test macro, assigns the sizeof expression to the result and checks if result is correct.
#if defined (__SDCC_pdk13) || defined (__SDCC_pdk14) || defined (__SDCC_pdk15)
  // Keep the assignment out of the assert, same result, but less memory required by assert strings (worse for debugging).
  #define DO_TEST(expression, result, type) result = sizeof (expression); ASSERT(sizeof(type)==result)
#else
  #define DO_TEST(expression, result, type) ASSERT((result = sizeof (expression), sizeof (type) == result))
#endif

// Prepare macros depending on variable size to test.
#if VAR_SIZE == 1
  #define VAR_TYPE            uint8_t
  #define TEST_VAR            c
  #define TEST_PTR_VAR        pc
  #define TEST_VAR_OTHER      i
  
  #define TEST_OP(x)          DO_TEST(x, c2, int)
#else
  #define VAR_TYPE            int16_t
  #define TEST_VAR            i
  #define TEST_PTR_VAR        pi
  #define TEST_VAR_OTHER      c
  
  #define TEST_OP(x)          DO_TEST(x, i2, int)
#endif

#define TEST_OP_NOPROMO(x)  DO_TEST(x, c2, VAR_TYPE)    // Test without promotion, same type expected.
#define TEST_OP_CHAR_PTR(x) DO_TEST(x, c2, VAR_TYPE *)  // Test for pointers, pointer of type expected.
#define TEST_OP_INT_PTR(x)  DO_TEST(x, i2, VAR_TYPE *)  // Same using integer for the result.


// Check default address space to check the size of the default pointer size correctly
#if defined (__SDCC_pdk13) || defined (__SDCC_pdk14) || defined (__SDCC_pdk15) || defined (__SDCC_mcs51) || defined (__SDCC_ds390)
  #if defined (__SDCC_mcs51)
    #if defined (__SDCC_MODEL_SMALL) || (__SDCC_MODEL_MEDIUM)
      #define DEFAULT_ADDR_SPACE __near
    #elif defined (__SDCC_MODEL_LARGE)  || (__SDCC_MODEL_HUGE)
      #define DEFAULT_ADDR_SPACE __far
    #endif
  #elif defined(__SDCC_ds390)
    #define DEFAULT_ADDR_SPACE __far
  #else
    #define DEFAULT_ADDR_SPACE __data // pdk
  #endif
#endif

#ifdef DEFAULT_ADDR_SPACE // Tests for backends that need the default address space qualifier
  #define TEST_OP_CHAR_PTR_DAS(x) DO_TEST(x, c2, VAR_TYPE DEFAULT_ADDR_SPACE *) // Test for pointers, pointer of type default address space expected.
  #define TEST_OP_INT_PTR_DAS(x)  DO_TEST(x, i2, VAR_TYPE DEFAULT_ADDR_SPACE *) // Same using integer for the result
#else
  #define TEST_OP_CHAR_PTR_DAS TEST_OP_CHAR_PTR
  #define TEST_OP_INT_PTR_DAS  TEST_OP_INT_PTR    
#endif


static void
testSizeof(void)
{
#ifdef TEST_BASIC // Macro self test
  TEST_OP_NOPROMO (VAR_TYPE);
  TEST_OP_CHAR_PTR (VAR_TYPE *);
  TEST_OP_INT_PTR (VAR_TYPE *);
  #ifdef DEFAULT_ADDR_SPACE
    TEST_OP_CHAR_PTR_DAS (VAR_TYPE DEFAULT_ADDR_SPACE *);
    TEST_OP_INT_PTR_DAS (VAR_TYPE DEFAULT_ADDR_SPACE *);
  #endif
#endif
  
#ifdef TEST_ASSING
  TEST_OP_NOPROMO (TEST_VAR = 1);
  TEST_OP_NOPROMO (TEST_VAR = 1 + 1);
  
  TEST_OP_NOPROMO (TEST_VAR += 1);
  TEST_OP_NOPROMO (TEST_VAR -= 1);
  TEST_OP_NOPROMO (TEST_VAR &= 1);
  TEST_OP_NOPROMO (TEST_VAR |= 1);
  TEST_OP_NOPROMO (TEST_VAR ^= 1);
  TEST_OP_NOPROMO (TEST_VAR *= 5);
  TEST_OP_NOPROMO (TEST_VAR /= 5);
  TEST_OP_NOPROMO (TEST_VAR /= 2);
  
  TEST_OP_NOPROMO (TEST_VAR <<= 2);
  TEST_OP_NOPROMO (TEST_VAR >>= 2);
  
  TEST_OP_NOPROMO ((TEST_VAR_OTHER++, TEST_VAR += 1));
#endif
    
#ifdef TEST_POINTERS

  TEST_OP_CHAR_PTR(VAR_TYPE *);
  TEST_OP_INT_PTR (VAR_TYPE *);

  TEST_OP (+(*TEST_PTR_VAR));
  TEST_OP (-(*TEST_PTR_VAR));
  TEST_OP (~(*TEST_PTR_VAR));

  TEST_OP_CHAR_PTR(TEST_PTR_VAR);
  TEST_OP_INT_PTR (TEST_PTR_VAR);
  
  TEST_OP_CHAR_PTR(TEST_PTR_VAR + 1);
  TEST_OP_INT_PTR (TEST_PTR_VAR + 1);
  
  TEST_OP_CHAR_PTR(TEST_PTR_VAR - 1);
  TEST_OP_INT_PTR (TEST_PTR_VAR - 1);
  
  TEST_OP_CHAR_PTR(TEST_PTR_VAR++);
  TEST_OP_INT_PTR (TEST_PTR_VAR--);
  
  TEST_OP_CHAR_PTR(TEST_PTR_VAR += 1);
  TEST_OP_INT_PTR (TEST_PTR_VAR -= 1);
  
  TEST_OP_CHAR_PTR(TEST_PTR_VAR = 1);
  TEST_OP_INT_PTR (TEST_PTR_VAR = 1);
  
  TEST_OP_CHAR_PTR_DAS(&TEST_VAR);
  TEST_OP_INT_PTR_DAS (&TEST_VAR);
  
  TEST_OP_CHAR_PTR_DAS(&TEST_VAR + 1);
  TEST_OP_INT_PTR_DAS (&TEST_VAR + 1);
  TEST_OP_CHAR_PTR_DAS(&TEST_VAR - 1);
  TEST_OP_INT_PTR_DAS (&TEST_VAR - 1);
#endif

#ifdef TEST_LITERAL
  TEST_OP ( 8);
  TEST_OP (+8);
  TEST_OP (-8);
  TEST_OP (~0);

  TEST_OP (1 ? 0 : 0);
  TEST_OP (0 ? 0 : 0);
  TEST_OP (1 ? 0 : 1);
  TEST_OP (0 ? 0 : 1);
  TEST_OP (1 ? 1 : 0);
  TEST_OP (0 ? 1 : 0);
  TEST_OP (1 ? 1 : 1);
  TEST_OP (0 ? 1 : 1);

  TEST_OP (8 << 1);
  TEST_OP (8 >> 1);
  TEST_OP (8 & 8);
  TEST_OP (8 | 8);
  TEST_OP (8 ^ 8);
  TEST_OP (8 + 8);
  TEST_OP (8 - 8);
  TEST_OP (8 * 8);
  TEST_OP (8 / 8);
  TEST_OP (8 % 2);
#endif

#ifdef TEST_LITERAL_BOOL_OP
  TEST_OP (!8);
  TEST_OP (!0);

  TEST_OP (8 > 1);
  TEST_OP (8 < 1);
  TEST_OP (8 >= 1);
  TEST_OP (8 <= 1);
  TEST_OP (8 == 1);
  TEST_OP (8 != 1);

  TEST_OP (1 > 8);
  TEST_OP (1 < 8);
  TEST_OP (1 >= 8);
  TEST_OP (1 <= 8);
  TEST_OP (1 == 8);
  TEST_OP (1 != 8);


  TEST_OP (1 && 1);
  TEST_OP (1 && 0);
  TEST_OP (0 && 0);

  TEST_OP (1 || 1);
  TEST_OP (1 || 0);
  TEST_OP (0 || 0);
#endif

#ifdef TEST_VARIABLE
  TEST_OP_NOPROMO (TEST_VAR);
  TEST_OP (+TEST_VAR);
  TEST_OP (-TEST_VAR);
  TEST_OP (~TEST_VAR);

  TEST_OP_NOPROMO (TEST_VAR++);
  TEST_OP_NOPROMO (TEST_VAR--);

  TEST_OP_NOPROMO (++TEST_VAR);
  TEST_OP_NOPROMO (--TEST_VAR);

  TEST_OP (TEST_VAR ? 0 : 0);
  TEST_OP (TEST_VAR ? 0 : 1);
  TEST_OP (TEST_VAR ? 1 : 0);
  TEST_OP (TEST_VAR ? 1 : 1);
  TEST_OP (1 ? TEST_VAR : 1);
  TEST_OP (1 ? TEST_VAR : 1);
  TEST_OP (1 ? 1 : TEST_VAR);
  TEST_OP (1 ? 1 : TEST_VAR);
  TEST_OP (0 ? TEST_VAR : 1);
  TEST_OP (0 ? TEST_VAR : 1);
  TEST_OP (0 ? 1 : TEST_VAR);
  TEST_OP (0 ? 1 : TEST_VAR);

  TEST_OP (TEST_VAR << 1);
  TEST_OP (TEST_VAR << (2 + 1));
  TEST_OP (TEST_VAR << (2 << 1));

  TEST_OP (TEST_VAR >> 1);
  TEST_OP (TEST_VAR >> (2 + 1));
  TEST_OP (TEST_VAR >> (2 << 1));

  TEST_OP (TEST_VAR & 8);
  TEST_OP (TEST_VAR & (2 + 1));
  TEST_OP (TEST_VAR & (2 << 1));

  TEST_OP (TEST_VAR | 8);
  TEST_OP (TEST_VAR | (2 + 1));
  TEST_OP (TEST_VAR | (2 << 1));

  TEST_OP (TEST_VAR ^ 8);
  TEST_OP (TEST_VAR ^ (2 + 1));
  TEST_OP (TEST_VAR ^ (2 << 1));

  TEST_OP (TEST_VAR + 8);
  TEST_OP (TEST_VAR + (2 + 1));
  TEST_OP (TEST_VAR + (2 << 1));

  TEST_OP (TEST_VAR - 8);
  TEST_OP (TEST_VAR - (2 + 1));
  TEST_OP (TEST_VAR - (2 << 1));

  TEST_OP (TEST_VAR * 8);
  TEST_OP (TEST_VAR * (2 + 1));
  TEST_OP (TEST_VAR * (2 << 1));
#endif

#ifdef TEST_VARIABLE_BOOL_OP
  TEST_OP (!TEST_VAR);
  
  TEST_OP (TEST_VAR > 0);
  TEST_OP (TEST_VAR < 0);
  TEST_OP (TEST_VAR >= 1);
  TEST_OP (TEST_VAR <= 1);
  TEST_OP (TEST_VAR == 1);
  TEST_OP (TEST_VAR != 1);

  TEST_OP (1 > c);
  TEST_OP (1 < c);
  TEST_OP (1 >= c);
  TEST_OP (1 <= c);
  TEST_OP (1 == c);
  TEST_OP (1 != c);

  TEST_OP (c && c);
  TEST_OP (c && 0);
  TEST_OP (0 && c);
  TEST_OP (c || c);
  TEST_OP (c || 0);
  TEST_OP (0 || c);
#endif
} 

