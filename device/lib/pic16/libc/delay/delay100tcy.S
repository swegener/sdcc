
        include <p18fxxx.inc>

        extern  _delay10tcy

        global  _delay100tcy
    
        code

_delay100tcy:
        ; polynomial for 100tcy delay is f(x) = 100 * (x-1) + 100
        decf    WREG, f
        movwf   POSTDEC1
        movlw   9
        call    _delay10tcy

        movf    PREINC1, w

        bz      @delay100_end
        bra     $+2

@delay100_loop:
        movwf   POSTDEC1

        movlw   9
        call    _delay10tcy

        bra     $+2

        movf    PREINC1, w
        nop
        bra     $+2
        decfsz  WREG, f
        bra     @delay100_loop

@delay100_end:
        return

        end
