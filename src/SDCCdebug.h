
#ifndef _DEBUG_INCLUDE_
#define _DEBUG_INCLUDE_

typedef struct DebugFile
{
  int (*openFile)(char *file);
  int (*closeFile)(void);
  int (*writeModule)(char *name);
  int (*writeFunction)(symbol *pSym, iCode *ic);
  int (*writeEndFunction)(symbol *pSym, iCode *ic, int offset);
  int (*writeLabel)(symbol *pSym, iCode *ic);
  int (*writeScope)(iCode *ic);
  int (*writeSymbol)(symbol *pSym);
  int (*writeType)(structdef *sdef, int block, int inStruct, char *tag);
  int (*writeCLine)(iCode *ic);
  int (*writeALine)(char *module, int Line);
  int (*writeFrameAddress)(char *variable, struct regs *reg, int offset);
}DEBUGFILE;

extern DEBUGFILE *debugFile;

void outputDebugStackSymbols(void);
void outputDebugSymbols(void);
void dumpSymInfo(char *pcName, memmap *memItem);
void emitDebuggerSymbol (char * debugSym);

#endif
