	;; ****************************************
        ;; Minimal crt0 and support functions for the sdcc test suite.
	;; Beginning of module
	.title	"Test runtime"
	.module	Runtime

        .globl _main
        .STACK = 0xE000

       	.area	_INIT (ABS)
        .org    0x0
        jp      0x100
                                
       	.org	0x100
__init::
	;; Beginning of the code
	DI			; Disable interrupts
	LD	SP,#.STACK
	;; Call the main function
	CALL	_main
        ld      a, #0
        rst     8

__putchar::
        ld      a,#1
        rst     #8
        ret

        ;; Segment order
        .org    0x200
	.area _HOME
        .area _CODE
	.area _OVERLAY
	.area _ISEG
	.area _BSEG
	.area _XSEG
	.area _GSINIT
	.area _GSFINAL
	.area _GSINIT
	.area _CODE

	.area _DATA
