        ;; Stubs to match between function names
        .area _CODE

        .globl  __mulslong
        .globl  __mululong
        .globl  __modslong
        .globl  __modulong
        .globl  __divslong
        .globl  __divulong
                
__mulslong_rr_s::        
        jp      __mulslong

__mululong_rr_s::       
        jp      __mululong
        
__modslong_rr_s::
        jp      __modslong
        
__modulong_rr_s::
        jp      __modulong
        
__divslong_rr_s::
        jp      __divslong
        
__divulong_rr_s::
        jp      __divulong
