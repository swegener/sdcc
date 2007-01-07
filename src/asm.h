#ifndef ASM_PORT_INCLUDE
#define ASM_PORT_INCLUDE

#include "dbuf.h"

void tfprintf (FILE * fp, const char *szFormat, ...);
void tsprintf (char *buffer, size_t len, const char *szFormat, ...);
void dbuf_tprintf (struct dbuf_s *dbuf, const char *szFormat, ...);
void dbuf_tvprintf (struct dbuf_s *dbuf, const char *szFormat, va_list ap);

typedef struct
  {
    const char *szKey;
    const char *szValue;
  }
ASM_MAPPING;

typedef struct _ASM_MAPPINGS ASM_MAPPINGS;

/* PENDING: could include the peephole rules here as well.
 */
struct _ASM_MAPPINGS
  {
    const ASM_MAPPINGS *pParent;
    const ASM_MAPPING *pMappings;
  };

/* The default asxxxx token mapping.
 */
extern const ASM_MAPPINGS asm_asxxxx_mapping;
extern const ASM_MAPPINGS asm_gas_mapping;
extern const ASM_MAPPINGS asm_a390_mapping;
extern const ASM_MAPPINGS asm_xa_asm_mapping;

/** Last entry has szKey = NULL.
 */
void asm_addTree (const ASM_MAPPINGS * pMappings);

char *FileBaseName (char *fileFullName);

char *printILine (iCode *ic);
char *printCLine (char *srcFile, int lineno);
#endif
