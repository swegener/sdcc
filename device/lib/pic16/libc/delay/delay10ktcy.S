
        include <p18fxxx.inc>

        extern  _delay10tcy
        extern  _delay1ktcy

        global  _delay10ktcy
    
        code

_delay10ktcy:
        ; polynomial for 10ktcy delay is f(x) = 10000 * (x-1) + 10000
        decf    WREG, f
    
        movwf    POSTDEC1
        movlw    9
        call    _delay1ktcy

        movlw       99
        call        _delay10tcy

        movf    PREINC1, w
 
        bz        @delay10k_end
        bra        $+2
    
@delay10k_loop:
        movwf    POSTDEC1

        movlw    9
        call    _delay1ktcy

        movlw       99
        call        _delay10tcy

        bra        $+2
        bra        $+2
        nop                         
        movf    PREINC1, w
        decfsz    WREG, f
        bra        @delay10k_loop

@delay10k_end:
        return

        end
