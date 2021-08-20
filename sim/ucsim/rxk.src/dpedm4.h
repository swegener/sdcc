/*
 * Simulator of microcontrollers (dpedm4.h)
 *
 * Copyright (C) 2020,2021 Drotos Daniel, Talker Bt.
 * 
 * To contact author send email to drdani@mazsola.iit.uni-miskolc.hu
 *
 */

/* This file is part of microcontroller simulator: ucsim.

UCSIM is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

UCSIM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with UCSIM; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA. */
/*@1@*/

#ifndef DPEDM4_HEADER
#define DPEDM4_HEADER

#define CBM_N		instruction_ed_00
#define LD_PW_iHTR_HL	instruction_ed_01
#define LD_PX_iHTR_HL	instruction_ed_11
#define LD_PY_iHTR_HL	instruction_ed_21
#define LD_PZ_iHTR_HL	instruction_ed_31
#define SBOX_A		instruction_ed_02
#define IBOX_A		instruction_ed_12
#define DWJNZ		instruction_ed_10
#define CP_HL_DE	instruction_ed_48
#define TEST_BC		instruction_ed_4c
#define LLJP_GT_LXPC_MN	instruction_ed_a2
#define LLJP_GTU_LXPC_MN instruction_ed_aa
#define LLJP_LT_LXPC_MN	instruction_ed_b2
#define LLJP_V_LXPC_MN	instruction_ed_ba
#define LLJP_NZ_LXPC_MN	instruction_ed_c2
#define LLJP_Z_LXPC_MN	instruction_ed_ca
#define LLJP_NC_LXPC_MN	instruction_ed_d2
#define LLJP_C_LXPC_MN	instruction_ed_da
#define PUSH_MN		instruction_ed_a5
#define JRE_GT_EE	instruction_ed_a3
#define JRE_LT_EE	instruction_ed_b3
#define JRE_GTU_EE	instruction_ed_ab
#define JRE_V_EE	instruction_ed_bb
#define JRE_NZ_EE	instruction_ed_c3
#define JRE_Z_EE	instruction_ed_cb
#define JRE_NC_EE	instruction_ed_d3
#define JRE_C_EE	instruction_ed_db
#define FLAG_NZ_HL	instruction_ed_c4
#define FLAG_Z_HL	instruction_ed_cc
#define FLAG_NC_HL	instruction_ed_d4
#define FLAG_C_HL	instruction_ed_dc
#define FLAG_GT_HL	instruction_ed_a4
#define FLAG_LT_HL	instruction_ed_b4
#define FLAG_GTU_HL	instruction_ed_ac
#define FLAG_V_HL	instruction_ed_bc

#endif

/* End of rxk.src/dpedm4.h */
