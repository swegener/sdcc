#ifndef MAIN_INCLUDE
#define MAIN_INCLUDE

typedef struct asmLineNode
  {
    int size;
    bitVect *regsRead;
    bitVect *regsWritten;
  }
asmLineNode;

bool x_parseOptions (char **argv, int *pargc);
void x_setDefaultOptions (void);
void x_finaliseOptions (void);

#endif
