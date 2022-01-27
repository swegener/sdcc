
#define GEN_WRAPPER(PAGE,CODE,INST,ADDR_MODE)			\
  int wrap_ ##PAGE ##_ ##CODE(class CL12 *uc, t_mem code)	\
  {								\
    return uc-> INST ( ADDR_MODE );				\
  }

#define GEN_DECL(PAGE,CODE)						\
  extern int wrap_ ## PAGE ## _ ## CODE (class CL12 *uc, t_mem code);

#define GEN_FILLER_METHOD(PAGE,CODE,MNEMO,BRANCH,LEN,TICKS)	\
  virtual void fill_##PAGE ##_##CODE()				\
  {								\
    page##PAGE[0x## CODE ]= wrap_ ##PAGE ##_ ##CODE;		\
    set_disass(PAGE, 0x## CODE, MNEMO, BRANCH, LEN);		\
    set_ticks(PAGE, 0x## CODE, TICKS);				\
  }


#define DEC(PAGE,CODE,INST,ADDR_MODE,MNEMO,BRANCH,LEN,TICKS)	"NL"	\
  GEN_WRAPPER(PAGE,CODE,INST,ADDR_MODE) "NL"				\
  GEN_DECL(PAGE,CODE)			"NL"				\
  GEN_FILLER_METHOD(PAGE,CODE,MNEMO,BRANCH,LEN,TICKS)
