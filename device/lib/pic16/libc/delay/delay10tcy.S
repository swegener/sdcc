
    include <p18fxxx.inc>

    global    _delay10tcy
    
    code

_delay10tcy:
      ; polynomial for 10tcy delay is f(x) = 10 * (x-1) + 10
      decf      WREG, f
      nop

      movf      WREG, w
      bz        @delay10_end

      bra       $+2

@delay10_loop:
      bra       $+2
      bra       $+2
      bra       $+2

      nop
      decfsz    WREG, f
      bra       @delay10_loop

@delay10_end:

      return

      end
