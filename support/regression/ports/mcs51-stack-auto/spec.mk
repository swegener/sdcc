# Port specification for the mcs51 port running with uCsim
#
# model small stack-auto

LIBSRCDIR = ../../device/lib
LIBDIR    = gen/$(PORT)/lib

LIBSDCCFLAGS+=--stack-auto --int-long-reent --float-reent
SDCCFLAGS   +=$(LIBSDCCFLAGS) --nostdlib -L$(LIBDIR) -llibsdcc -llibint \
              -lliblong -llibfloat -lmcs51

# copy support.c
$(PORTS_DIR)/$(PORT)/%.c: $(PORTS_DIR)/mcs51/%.c
	cp $< $@

include $(PORTS_DIR)/mcs51/spec.mk

SOURCES = _atoi.c _atol.c _autobaud.c _bp.c _schar2fs.c \
          _decdptr.c _divsint.c _divslong.c _divuint.c \
          _divulong.c _fs2schar.c _fs2sint.c _fs2slong.c \
          _fs2uchar.c _fs2uint.c _fs2ulong.c _fsadd.c \
          _fsdiv.c _fseq.c _fsgt.c _fslt.c _fsmul.c \
          fabsf.c sqrtf.c logf.c log10f.c powf.c tanf.c \
          errno.c frexpf.c ldexpf.c tancotf.c \
          _fsneq.c _fssub.c _gptrget.c _gptrput.c \
          _sint2fs.c _iscntrl.c _isdigit.c _isgraph.c \
          _islower.c _isprint.c _ispunct.c _isspace.c \
          _isupper.c _isxdigit.c _slong2fs.c \
          _memcmp.c _memcpy.c _memmove.c _memset.c \
          _modsint.c _modslong.c _moduint.c _modulong.c \
          _mulint.c _mullong.c \
          _ser.c _setjmp.c \
          _spx.c _startup.c _strchr.c _strcmp.c _strcpy.c \
          _strcspn.c _strlen.c _strncat.c _strncmp.c \
          _strncpy.c _strpbrk.c _strrchr.c _strspn.c \
          _strstr.c _strtok.c \
          _uchar2fs.c _uint2fs.c _ulong2fs.c \
          calloc.c malloc.c realloc.c free.c \
          serial.c ser_ir.c printfl.c \
          printf_large.c vprintf.c puts.c gets.c \
          assert.c _strcat.c time.c printf_fast.c bpx.c

OBJECTS = $(patsubst %.c,$(LIBDIR)/%.rel,$(SOURCES))
MODULES = $(patsubst %.c,%,$(SOURCES))

.PHONY: make-library
make-library: $(LIBDIR) $(OBJECTS) lib-files


$(LIBDIR):
	mkdir -p $(LIBDIR)

$(LIBDIR)/%.rel: $(LIBSRCDIR)/%.c
	-$(SDCC) -I../../device/include $(LIBSDCCFLAGS) -c $< -o $@

.PHONY: lib-files
lib-files:
	make -C $(LIBSRCDIR)/mcs51 all
	cp $(LIBSRCDIR)/mcs51/*.rel $(LIBSRCDIR)/mcs51/mcs51.lib $(LIBDIR)
	echo $(MODULES) | tr ' ' '\n' > $(LIBDIR)/libsdcc.lib
	touch $(LIBDIR)/libfloat.lib $(LIBDIR)/libint.lib $(LIBDIR)/liblong.lib
