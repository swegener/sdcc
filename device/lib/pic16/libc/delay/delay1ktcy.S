
        include <p18fxxx.inc>

        extern  _delay10tcy

        global  _delay1ktcy
    
        code

_delay1ktcy:
        ; polynomial for 1ktcy delay is f(x) = 1000 * (x-1) + 1000
        decf    WREG, f

        movwf   POSTDEC1
        movlw    99
        call    _delay10tcy

        movf    PREINC1, w
 
        bz      @delay1k_end
        bra     $+2
    
@delay1k_loop:
        movwf   POSTDEC1
        movlw   99
        call    _delay10tcy

        bra     $+2
        bra     $+2
        nop

        movf    PREINC1, w
        decfsz  WREG, f
        bra     @delay1k_loop

@delay1k_end:
        return

        end
