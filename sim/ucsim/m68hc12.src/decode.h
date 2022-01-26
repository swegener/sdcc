#include "gen.h"

DEC(0, 89,	adc	, _A_i8  	, "ADC A,%b"	, ' ', 2)
DEC(0, 99,	adc	, _A_dop	, "ADC A,%d"	, ' ', 2)
DEC(0, a9,	adc	, _A_xbop8	, "ADC A,%p"	, ' ', -2)
DEC(0, b9,	adc	, _A_eop 	, "ADC A,%e"	, ' ', 3)

DEC(0, b7,	exec_b7	, _NONE		, "%T"		, ' ', 2)
