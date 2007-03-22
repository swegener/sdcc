/*
 * shadowregs.c - provide shadow register for use during interrupts
 *
 * (c) 2007 by Raphael Neider <rneider @ web.de>
 * 
 * This file is part of SDCC's pic14 library and distributed under
 * the terms of the GPLv2 with linking exception; see COPYING in some
 * parent directory for details.
 */

/* 
 * We should make sure these always reside in the same bank,
 * so that we can save two BANKSELs in the interrupt entry code.
 */

unsigned char __sdcc_saved_fsr;
//unsigned char __sdcc_saved_pclath;
//unsigned char __sdcc_saved_status;

