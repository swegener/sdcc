#ifndef MDU517_HEADER
#define MDU517_HEADER

#include <stdint.h>

extern uint8_t mdu_32div16(uint32_t op1, uint16_t op2,
			   uint32_t *res, uint16_t *rem);
extern uint8_t mdu_16div16(uint16_t op1, uint16_t op2,
			   uint16_t *res, uint16_t *rem);
extern uint8_t mdu_16mul16(uint16_t op1, uint16_t op2,
			   uint32_t *res);
extern uint8_t mdu_norm(uint32_t op,
			uint32_t *res, uint8_t *nuof_shifts);
extern uint8_t mdu_shift(uint32_t op, uint8_t shifts, uint8_t right,
			 uint32_t *res);
extern uint8_t mdu_lshift(uint32_t op, uint8_t shifts,
			  uint32_t *res);
extern uint8_t mdu_rshift(uint32_t op, uint8_t shifts,
			  uint32_t *res);

#endif
