enable_51	= @enable_51@
enable_avr	= @enable_avr@
enable_z80	= @enable_z80@
enable_xa	= @enable_xa@

ifeq ($(enable_51),yes)
S51		= s51.src
else
S51		=
endif

ifeq ($(enable_avr),yes)
SAVR		= avr.src
else
SAVR		=
endif

ifeq ($(enable_z80),yes)
SZ80		= z80.src
else
SZ80		=
endif

ifeq ($(enable_xa),yes)
XA		= xa.src
else
XA		=
endif

PKGS		= cmd.src sim.src gui.src $(S51) $(SAVR) $(SZ80) $(XA) doc
