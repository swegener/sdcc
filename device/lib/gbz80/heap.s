        ;; Stubs that hook the heap in
        .globl  __sdcc_heap_init

        .area   _GSINIT
        call    __sdcc_heap_init

        .area   _HEAP
__sdcc_heap_start::
        ;; For now just allocate 1k of heap.
        .ds     1023

        .area   _HEAP_END
__sdcc_heap_end::
        .ds     1
