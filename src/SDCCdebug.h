
#ifndef _DEBUG_INCLUDE_
#define _DEBUG_INCLUDE_

typedef struct DebugFile
{
  int (*openFile)(char *file);
  int (*closeFile)(void);
  int (*writeModule)(char *name);
  int (*writeFunction)(symbol *pSym);
  int (*writeSymbol)(symbol *pSym);
  int (*writeType)(structdef *sdef, int block, int inStruct, char *tag);
  int (*writeCLine)(char *module, int Line, int Level, int Block);  
  int (*writeALine)(char *module, int Line);

}DEBUGFILE;

extern DEBUGFILE *debugFile;

void outputDebugStackSymbols(void);
void outputDebugSymbols(void);
void dumpSymInfo(char *pcName, memmap *memItem);

#endif
