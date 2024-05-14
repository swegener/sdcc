EXEEXT		= @EXEEXT@

enable_mcs51_port   = @enable_mcs51_port@
enable_avr_port     = @enable_avr_port@
enable_z80_port     = @enable_z80_port@
enable_tlcs_port    = @enable_tlcs_port@
enable_xa_port      = @enable_xa_port@
enable_m68hc08_port = @enable_m68hc08_port@
enable_s08_port     = @enable_s08_port@
enable_stm8_port    = @enable_stm8_port@
enable_st7_port     = @enable_st7_port@
enable_pdk_port     = @enable_pdk_port@
enable_p1516_port   = @enable_p1516_port@
enable_m6809_port   = @enable_m6809_port@
enable_m6800_port   = @enable_m6800_port@
enable_m68hc11_port = @enable_m68hc11_port@
enable_m68hc12_port = @enable_m68hc12_port@
enable_mos6502_port = @enable_mos6502_port@
enable_rxk_port	    = @enable_rxk_port@
enable_pblaze_port  = @enable_pblaze_port@
enable_i8085_port   = @enable_i8085_port@
enable_f8_port      = @enable_f8_port@
enable_i8048_port   = @enable_i8048_port@

enable_serio        = @enable_serio@
enable_ucsim        = @enable_ucsim@

ifeq ($(enable_mcs51_port),yes)
S51		= s51.src
else
S51		=
endif

ifeq ($(enable_avr_port),yes)
SAVR		= avr.src
else
SAVR		=
endif

ifeq ($(enable_z80_port),yes)
SZ80		= z80.src
else
SZ80		=
endif

ifeq ($(enable_tlcs_port),yes)
TLCS		= tlcs.src
else
TLCS		=
endif

ifeq ($(enable_m68hc08_port),yes)
SM68HC08	= m68hc08.src
else
ifeq ($(enable_s08_port),yes)
SM68HC08	= m68hc08.src
else
SM68HC08	=
endif
endif

ifeq ($(enable_xa_port),yes)
XA		= xa.src
else
XA		=
endif

ifeq ($(enable_stm8_port),yes)
STM8		= stm8.src
else
STM8		=
endif

ifeq ($(enable_st7_port),yes)
ST7      	= st7.src
else
ST7      =
endif

ifeq ($(enable_pdk_port),yes)
PDK    = pdk.src
else
PDKD   =
endif

ifeq ($(enable_p1516_port),yes)
P1516      	= p1516.src
else
P1516      =
endif

ifeq ($(enable_m6809_port),yes)
M6809      	= m6809.src
else
M6809      =
endif

ifeq ($(enable_m6800_port),yes)
M6800      	= m6800.src
else
M6800      =
endif

ifeq ($(enable_m68hc11_port),yes)
M68HC11      	= m68hc11.src
else
M68HC11    =
endif

ifeq ($(enable_m68hc12_port),yes)
M68HC12      	= m68hc12.src
else
M68HC12    =
endif

ifeq ($(enable_mos6502_port),yes)
MOS6502      	= mos6502.src
else
MOS6502      =
endif

ifeq ($(enable_rxk_port),yes)
RXK      	= rxk.src
else
RXK      =
endif

ifeq ($(enable_pblaze_port),yes)
PBLAZE      	= pblaze.src
else
PBLAZE	=
endif

ifeq ($(enable_i8085_port),yes)
I8085      	= i8085.src
else
I8085	=
endif

ifeq ($(enable_f8_port),yes)
F8      	= f8.src
else
F8		=
endif

ifeq ($(enable_i8048_port),yes)
I8048      	= i8048.src
else
I8048		=
endif

PKGS		= $(P1516) $(F8) \
		  $(S51) $(I8048) $(I8085) $(XA) \
		  $(SAVR) \
		  $(SZ80) $(TLCS) $(RXK) \
		  $(STM8) $(ST7)  \
		  $(M6800) $(SM68HC08) $(M6809) $(M68HC11) $(M68HC12) \
		  $(MOS6502) \
		  $(PBLAZE) $(PDK)

PKGS_ALL	= s51.src avr.src z80.src tlcs.src xa.src \
		  m68hc08.src stm8.src st7.src pdk.src p1516.src \
		  m6809.src m6800.src m68hc11.src m68hc12.src mos6502.src \
		  rxk.src pblaze.src i8085.src f8.src i8048.src

curses_ok	= @curses_ok@

ifeq ($(enable_serio),yes)
ifeq ($(curses_ok),yes)
SERIO		= serio.src
else
SERIO		=
endif
else
SERIO		=
endif

ifeq ($(enable_ucsim),yes)
UCSIM		= ucsim.src
else
UCSIM		=
endif

APPS		= $(SERIO) $(UCSIM) relay.src

APPS_ALL	= serio.src ucsim.src relay.src
