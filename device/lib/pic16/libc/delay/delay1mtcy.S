
        include <p18fxxx.inc>

        extern  _delay10tcy
        extern  _delay100tcy
        extern  _delay10ktcy

        global  _delay1mtcy
    
        code

_delay1mtcy:
        ; polynomial for 1mtcy delay is f(x) = 100000 * (x-1) + 100000
        decf    WREG, f
    
        movwf   POSTDEC1
        movlw   99
        call    _delay10ktcy

        movlw   99
        call    _delay100tcy
        
        movlw   9
        call    _delay10tcy

        movf    PREINC1, w
 
        bz      @delay1m_end
        bra     $+2
    
@delay1m_loop:
        movwf    POSTDEC1

        movlw   99
        call    _delay10ktcy
        
        movlw   99
        call    _delay100tcy
        
        movlw   9
        call    _delay10tcy

        bra     $+2
        bra     $+2
        nop                         
        movf    PREINC1, w
        decfsz  WREG, f
        bra     @delay1m_loop

@delay1m_end:
        return

        end
