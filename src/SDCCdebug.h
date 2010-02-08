
#ifndef _DEBUG_INCLUDE_
#define _DEBUG_INCLUDE_

typedef struct DebugFile
{
  int (*openFile) (const char *file);
  int (*closeFile) (void);
  int (*writeModule) (const char *name);
  int (*writeFunction) (symbol *pSym, iCode *ic);
  int (*writeEndFunction) (symbol *pSym, iCode *ic, int offset);
  int (*writeLabel) (symbol *pSym, iCode *ic);
  int (*writeScope) (iCode *ic);
  int (*writeSymbol) (symbol *pSym);
  int (*writeType) (structdef *sdef, int block, int inStruct, const char *tag);
  int (*writeCLine) (iCode *ic);
  int (*writeALine) (const char *module, int Line);
  int (*writeFrameAddress) (const char *variable, struct regs *reg, int offset);
}DEBUGFILE;

extern DEBUGFILE *debugFile;

void outputDebugStackSymbols (void);
void outputDebugSymbols (void);
void dumpSymInfo (const char *pcName, memmap *memItem);
void emitDebuggerSymbol (const char * debugSym);

#endif
