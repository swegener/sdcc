        ;; Stubs that hook the heap in
        .globl  __sdcc_heap_init
        
        .area   GSINIT
        call    __sdcc_heap_init
        
        .area   HEAP
__sdcc_heap_start::
        ;; For now just allocate 1k of heap.
        .ds     1023
        
        .area   HEAP_END
__sdcc_heap_end::
        .ds     1
