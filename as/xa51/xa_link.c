/* This is a cheap hack. The xa51 has a couple of ways to scramble
   relocation info into it's opcode that the standard linker can't
   handle. 

   The relocatable format looks like the known one, BUT ISN'T.

   The only things that are handled now are:

   "SDCCXA rel, version %f" must be the first line
   "H %d areas %d global symbols" defines the # of areas and globals
   "S <symbol> [Ref000 | DefXXXX]" Def's are supposed to be defined in
     their own area/segment
   "A <seg> size %d flags %d" switch to another segment. this can happen
     multiple times and should be equal. flags is ignored
   "T xx xx bb bb ..." where x is the address within the current segment
     and bb are the bytes
   "R xx <how> <symbol>" the relocation tag. xx is the offset within the
     previous "T .." line. <how> could be something like REL_FF, REL_FFFF, 
     ABS_F0FF. and symbol is the previous defined symbol it refers to

   Thats all for now.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "xa_version.h"

static char outFileName[PATH_MAX];

void baseName(char *name, char*base) {
  int i, first, last;

  // find the last path seperator in name
  for (first=strlen(name)-1; 
       (name[first]!='/' && name[first]!='\\') && first;
       first--);
  if (name[first]=='/' || name[first]=='\\') {
    first++;
  }

  // find the last ext seperator in name
  for (last=strlen(name)-1; 
       (name[last]!='.' && last);
       last--);
  if (!last) {
    last=strlen(name);
  }

  printf ("baseName: %s %d %d\n", name, first, last);
  // fill the base with the baseName
  for (i=first; i<last; i++) {
    base[i-first]=name[i];
  }
  base[i]='\0';
}
  
void readModule(char *module) {
  double thisVersion;
  char line[132];
  FILE *relModule;

  if ((relModule=fopen(module, "r"))==NULL) {
    perror (module);
    exit (1);
  }
  printf ("ReadModule: %s\n", module);
  // first we need to check if this is a valid file
  if (sscanf(line, "SDCCXA rel, version %lf", &thisVersion)!=1) {
    fprintf (stderr, "%s is not a valid input file\n", module);
    exit (1);
  }
  if (thisVersion!=version) {
    fprintf (stderr, "version conflict; we: %f != module: %f\n", 
	     version, thisVersion);
  }

  fclose (relModule);
}

void writeModule(char *module) {
  if (!outFileName[0]) {
    sprintf (outFileName, "%s.bin", module);
  }
  printf ("WriteModule: %s\n", outFileName);
}

void usage (char * progName, int errNo) {
  fprintf (stderr, "usage: %s f.rel [f1.rel [f2.rel [...]]]\n", progName);
  if (errNo) {
    exit (errNo);
  }
}

int main(int argc, char **argv) {
  char outputName[PATH_MAX];
  int i;

  // no options yet
  if (argc<2) {
    usage(argv[0], 1);
  }
  baseName(argv[1], outputName);
  printf ("using baseName: %s\n", outputName);
  for (i=1; i<argc; i++) {
    readModule(argv[i]);
  }
  writeModule (outputName);
  return 0;
}
     
