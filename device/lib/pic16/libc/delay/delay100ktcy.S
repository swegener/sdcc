
        include <p18fxxx.inc>

        extern  _delay10tcy
        extern  _delay1ktcy

        global  _delay100ktcy
    
        code

_delay100ktcy:
        ; polynomial for 100ktcy delay is f(x) = 100000 * (x-1) + 100000
        decf    WREG, f
    
        movwf    POSTDEC1
        movlw    99
        call    _delay1ktcy

        movlw       99
        call        _delay10tcy

        movf    PREINC1, w
 
        bz        @delay100k_end
        bra        $+2
    
@delay100k_loop:
        movwf    POSTDEC1

        movlw    99
        call    _delay1ktcy

        movlw       99
        call        _delay10tcy

        bra        $+2
        bra        $+2
        nop                         
        movf    PREINC1, w
        decfsz    WREG, f
        bra        @delay100k_loop

@delay100k_end:
        return

        end
