#define DS1621_ID 0x90

// we have no decent float support in sdcc yet
#define USE_FLOAT 0

#if USE_FLOAT
extern float 
#else
extern char *
#endif

ReadDS1621(char address);
