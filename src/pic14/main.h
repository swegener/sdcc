#ifndef MAIN_INCLUDE
#define MAIN_INCLUDE

typedef struct {
  unsigned int isLibrarySource:1;
  int disable_df;
} pic14_options_t;

extern pic14_options_t pic14_options;
extern int debug_verbose;

#endif

