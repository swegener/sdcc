## Process this file with Automake to create Makefile.in

noinst_LIBRARIES = libm.a libme.a

libm_a_SOURCES =
libm_a_SOURCES += acosf.c
libm_a_SOURCES += asincosf.c
libm_a_SOURCES += asinf.c
libm_a_SOURCES += atan2f.c
libm_a_SOURCES += atanf.c
libm_a_SOURCES += ceilf.c
libm_a_SOURCES += cosf.c
libm_a_SOURCES += coshf.c
libm_a_SOURCES += cotf.c
libm_a_SOURCES += errno.c
libm_a_SOURCES += expf.c
libm_a_SOURCES += fabsf.c
libm_a_SOURCES += floorf.c
libm_a_SOURCES += frexpf.c
libm_a_SOURCES += ldexpf.c
libm_a_SOURCES += log10f.c
libm_a_SOURCES += logf.c
libm_a_SOURCES += modff.c
libm_a_SOURCES += powf.c
libm_a_SOURCES += sincosf.c
libm_a_SOURCES += sincoshf.c
libm_a_SOURCES += sinf.c
libm_a_SOURCES += sinhf.c
libm_a_SOURCES += sqrtf.c
libm_a_SOURCES += tancotf.c
libm_a_SOURCES += tanf.c
libm_a_SOURCES += tanhf.c

libm_a_CFLAGS = -p$(ARCH) $(AM_CFLAGS)

libme_a_SOURCES = $(libm_a_SOURCES)

libme_a_CFLAGS = -p$(EARCH) $(AM_CFLAGS)

if HAVE_NEW_PICS
# add new pics libraries to be built here
noinst_LIBRARIES +=

## replace <new_pics> with the new library name,
## add sources for new libraries and
## uncomment the following line
#<new_pics>_a_SOURCES = $(libm_a_SOURCES)

## replace <new_pics> with the new library name,
## add CFLAGS for compiling new libraries and
## uncomment the following line
#<new_pics>_a_CFLAGS = -p$(EARCH) $(AM_CFLAGS)
endif

include $(top_srcdir)/Makefile.common
