/*-------------------------------------------------------------------------

   pcode.c - post code generation
   Written By -  Scott Dattalo scott@dattalo.com
   Ported to PIC16 By -  Martin Dubuc m.dubuc@rogers.com

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
-------------------------------------------------------------------------*/

#include <stdio.h>

#include "common.h"   // Include everything in the SDCC src directory
#include "newalloc.h"


#include "pcode.h"
#include "pcodeflow.h"
#include "ralloc.h"
#include "device.h"

#if defined(__BORLANDC__) || defined(_MSC_VER)
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

/****************************************************************/
/****************************************************************/

static peepCommand peepCommands[] = {

  {NOTBITSKIP, "_NOTBITSKIP_"},
  {BITSKIP, "_BITSKIP_"},
  {INVERTBITSKIP, "_INVERTBITSKIP_"},

  {-1, NULL}
};



// Eventually this will go into device dependent files:
pCodeOpReg pic16_pc_status    = {{PO_STATUS,  "_STATUS"}, -1, NULL,0,NULL};
pCodeOpReg pic16_pc_indf0     = {{PO_INDF0,   "INDF0"}, -1, NULL,0,NULL};
pCodeOpReg pic16_pc_fsr0      = {{PO_FSR0,    "FSR0"}, -1, NULL,0,NULL};
pCodeOpReg pic16_pc_intcon    = {{PO_INTCON,  ""}, -1, NULL,0,NULL};
pCodeOpReg pic16_pc_pcl       = {{PO_PCL,     "PCL"}, -1, NULL,0,NULL};
pCodeOpReg pic16_pc_pclath    = {{PO_PCLATH,  "_PCLATH"}, -1, NULL,0,NULL};
pCodeOpReg pic16_pc_wreg      = {{PO_WREG,    "WREG"}, -1, NULL,0,NULL};
pCodeOpReg pic16_pc_bsr       = {{PO_BSR,     "BSR"}, -1, NULL,0,NULL};

pCodeOpReg pic16_pc_kzero     = {{PO_GPR_REGISTER,  "KZ"}, -1, NULL,0,NULL};
pCodeOpReg pic16_pc_wsave     = {{PO_GPR_REGISTER,  "WSAVE"}, -1, NULL,0,NULL};
pCodeOpReg pic16_pc_ssave     = {{PO_GPR_REGISTER,  "SSAVE"}, -1, NULL,0,NULL};

static int mnemonics_initialized = 0;


static hTab *pic16MnemonicsHash = NULL;
static hTab *pic16pCodePeepCommandsHash = NULL;

static pFile *the_pFile = NULL;
static pBlock *pb_dead_pcodes = NULL;

/* Hardcoded flags to change the behavior of the PIC port */
static int peepOptimizing = 1;        /* run the peephole optimizer if nonzero */
static int functionInlining = 1;      /* inline functions if nonzero */
int pic16_debug_verbose = 1;                /* Set true to inundate .asm file */

//static int GpCodeSequenceNumber = 1;
static int GpcFlowSeq = 1;

extern void pic16_RemoveUnusedRegisters(void);
extern void pic16_RegsUnMapLiveRanges(void);
extern void pic16_BuildFlowTree(pBlock *pb);
extern void pic16_pCodeRegOptimizeRegUsage(int level);
extern int pic16_picIsInitialized(void);
#if !OPT_DISABLE_PIC || !OPT_DISABLE_PIC16
// From pic/pcode.c:
extern void SAFE_snprintf(char **str, size_t *size, const char *format, ...);
extern int mnem2key(char const *mnem);
#endif // OPT_DISABLE_PIC || !OPT_DISABLE_PIC16

/****************************************************************/
/*                      Forward declarations                    */
/****************************************************************/

void pic16_unlinkpCode(pCode *pc);
#if 0
static void genericAnalyze(pCode *pc);
static void AnalyzeGOTO(pCode *pc);
static void AnalyzeSKIP(pCode *pc);
static void AnalyzeRETURN(pCode *pc);
#endif

static void genericDestruct(pCode *pc);
static void genericPrint(FILE *of,pCode *pc);

static void pCodePrintLabel(FILE *of, pCode *pc);
static void pCodePrintFunction(FILE *of, pCode *pc);
static void pCodeOpPrint(FILE *of, pCodeOp *pcop);
static char *pic16_get_op_from_instruction( pCodeInstruction *pcc);
char *pic16_get_op(pCodeOp *pcop,char *buff,size_t buf_size);
int pCodePeepMatchLine(pCodePeep *peepBlock, pCode *pcs, pCode *pcd);
int pic16_pCodePeepMatchRule(pCode *pc);
static void pBlockStats(FILE *of, pBlock *pb);
static pBlock *newpBlock(void);
extern void pic16_pCodeInsertAfter(pCode *pc1, pCode *pc2);
extern pCodeOp *pic16_popCopyReg(pCodeOpReg *pc);
pCodeOp *pic16_popCopyGPR2Bit(pCodeOp *pc, int bitval);
void pic16_pCodeRegMapLiveRanges(pBlock *pb);


/****************************************************************/
/*                    PIC Instructions                          */
/****************************************************************/

pCodeInstruction pic16_pciADDWF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_ADDWF,
  "ADDWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER),   // inCond
  (PCC_REGISTER | PCC_Z) // outCond
};

pCodeInstruction pic16_pciADDFW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_ADDFW,
  "ADDWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER),   // inCond
  (PCC_W | PCC_Z) // outCond
};

pCodeInstruction pic16_pciADDWFC = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_ADDWFC,
  "ADDWFC",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER | PCC_C),   // inCond
  (PCC_REGISTER | PCC_Z) // outCond
};

pCodeInstruction pic16_pciADDFWC = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_ADDFWC,
  "ADDWFC",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER | PCC_C),   // inCond
  (PCC_W | PCC_Z) // outCond
};

pCodeInstruction pic16_pciADDLW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_ADDLW,
  "ADDLW",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  1,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_LITERAL),   // inCond
  (PCC_W | PCC_Z | PCC_C | PCC_DC | PCC_OV | PCC_N) // outCond
};

pCodeInstruction pic16_pciANDLW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_ANDLW,
  "ANDLW",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  1,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_LITERAL),   // inCond
  (PCC_W | PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciANDWF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_ANDWF,
  "ANDWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER),   // inCond
  (PCC_REGISTER | PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciANDFW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_ANDFW,
  "ANDWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER),   // inCond
  (PCC_W | PCC_Z) // outCond
};

pCodeInstruction pic16_pciBC = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BC,
  "BC",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_REL_ADDR | PCC_C),   // inCond
  PCC_NONE    // outCond
};

pCodeInstruction pic16_pciBCF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BCF,
  "BCF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,1,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_BSF,
  (PCC_REGISTER | PCC_EXAMINE_PCOP),   // inCond
  PCC_REGISTER // outCond
};

pCodeInstruction pic16_pciBN = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BN,
  "BN",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_REL_ADDR | PCC_N),   // inCond
  PCC_NONE    // outCond
};

pCodeInstruction pic16_pciBNC = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BNC,
  "BNC",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_REL_ADDR | PCC_C),   // inCond
  PCC_NONE    // outCond
};

pCodeInstruction pic16_pciBNN = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BNN,
  "BNN",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_REL_ADDR | PCC_N),   // inCond
  PCC_NONE    // outCond
};

pCodeInstruction pic16_pciBNOV = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BNOV,
  "BNOV",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_REL_ADDR | PCC_OV),   // inCond
  PCC_NONE    // outCond
};

pCodeInstruction pic16_pciBNZ = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BNZ,
  "BNZ",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_REL_ADDR | PCC_Z),   // inCond
  PCC_NONE    // outCond
};

pCodeInstruction pic16_pciBOV = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BOV,
  "BOV",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_REL_ADDR | PCC_OV),   // inCond
  PCC_NONE  // outCond
};

pCodeInstruction pic16_pciBRA = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BRA,
  "BRA",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REL_ADDR,   // inCond
  PCC_NONE    // outCond
};

pCodeInstruction pic16_pciBSF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BSF,
  "BSF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,1,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_BCF,
  (PCC_REGISTER | PCC_EXAMINE_PCOP),   // inCond
  (PCC_REGISTER | PCC_EXAMINE_PCOP) // outCond
};

pCodeInstruction pic16_pciBTFSC = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   AnalyzeSKIP,
   genericDestruct,
   genericPrint},
  POC_BTFSC,
  "BTFSC",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,1,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_BTFSS,
  (PCC_REGISTER | PCC_EXAMINE_PCOP),   // inCond
  PCC_EXAMINE_PCOP // outCond
};

pCodeInstruction pic16_pciBTFSS = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   AnalyzeSKIP,
   genericDestruct,
   genericPrint},
  POC_BTFSS,
  "BTFSS",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,1,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_BTFSC,
  (PCC_REGISTER | PCC_EXAMINE_PCOP),   // inCond
  PCC_EXAMINE_PCOP // outCond
};

pCodeInstruction pic16_pciBTG = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BTG,
  "BTG",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,1,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_REGISTER | PCC_EXAMINE_PCOP),   // inCond
  (PCC_REGISTER | PCC_EXAMINE_PCOP) // outCond
};

pCodeInstruction pic16_pciBZ = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_BZ,
  "BZ",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_Z,   // inCond
  PCC_NONE // outCond
};

pCodeInstruction pic16_pciCALL = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_CALL,
  "CALL",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  1,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_NONE, // inCond
  PCC_NONE  // outCond
};

pCodeInstruction pic16_pciCOMF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_COMF,
  "COMF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,  // inCond
  PCC_REGISTER   // outCond
};

pCodeInstruction pic16_pciCOMFW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_COMFW,
  "COMF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,  // inCond
  PCC_W   // outCond
};

pCodeInstruction pic16_pciCLRF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_CLRF,
  "CLRF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER, // inCond
  PCC_REGISTER  // outCond
};

pCodeInstruction pic16_pciCLRWDT = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_CLRWDT,
  "CLRWDT",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  0,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_NONE, // inCond
  PCC_NONE  // outCond
};

pCodeInstruction pic16_pciCPFSEQ = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_CPFSEQ,
  "CPFSEQ",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  0,0,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER), // inCond
  PCC_NONE  // outCond
};

pCodeInstruction pic16_pciCPFSGT = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_CPFSGT,
  "CPFSGT",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  0,0,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER), // inCond
  PCC_NONE  // outCond
};

pCodeInstruction pic16_pciCPFSLT = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_CPFSLT,
  "CPFSLT",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  1,0,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER), // inCond
  PCC_NONE  // outCond
};

pCodeInstruction pic16_pciDAW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_DAW,
  "DAW",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  0,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_W, // inCond
  (PCC_W | PCC_C) // outCond
};

pCodeInstruction pic16_pciDCFSNZ = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_DCFSNZ,
  "DCFSNZ",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER, // inCond
  PCC_REGISTER  // outCond
};

pCodeInstruction pic16_pciDCFSNZW = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_DCFSNZW,
  "DCFSNZ",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER, // inCond
  PCC_W  // outCond
};

pCodeInstruction pic16_pciDECF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_DECF,
  "DECF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  PCC_REGISTER    // outCond
};

pCodeInstruction pic16_pciDECFW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_DECFW,
  "DECF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  PCC_W    // outCond
};

pCodeInstruction pic16_pciDECFSZ = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   AnalyzeSKIP,
   genericDestruct,
   genericPrint},
  POC_DECFSZ,
  "DECFSZ",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  PCC_REGISTER    // outCond
};

pCodeInstruction pic16_pciDECFSZW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   AnalyzeSKIP,
   genericDestruct,
   genericPrint},
  POC_DECFSZW,
  "DECFSZ",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  PCC_W           // outCond
};

pCodeInstruction pic16_pciGOTO = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   AnalyzeGOTO,
   genericDestruct,
   genericPrint},
  POC_GOTO,
  "GOTO",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REL_ADDR,   // inCond
  PCC_NONE    // outCond
};

pCodeInstruction pic16_pciINCF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_INCF,
  "INCF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  (PCC_REGISTER | PCC_C | PCC_DC | PCC_Z | PCC_OV | PCC_N) // outCond
};

pCodeInstruction pic16_pciINCFW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_INCFW,
  "INCF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  PCC_W    // outCond
};

pCodeInstruction pic16_pciINCFSZ = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   AnalyzeSKIP,
   genericDestruct,
   genericPrint},
  POC_INCFSZ,
  "INCFSZ",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  PCC_REGISTER    // outCond
};

pCodeInstruction pic16_pciINCFSZW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   AnalyzeSKIP,
   genericDestruct,
   genericPrint},
  POC_INCFSZW,
  "INCFSZ",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  PCC_W           // outCond
};

pCodeInstruction pic16_pciINFSNZ = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   AnalyzeSKIP,
   genericDestruct,
   genericPrint},
  POC_INFSNZ,
  "INCFSNZ",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  PCC_REGISTER    // outCond
};

pCodeInstruction pic16_pciIORWF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_IORWF,
  "IORWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER),   // inCond
  (PCC_REGISTER | PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciIORFW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_IORFW,
  "IORWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER),   // inCond
  (PCC_W | PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciIORLW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_IORLW,
  "IORLW",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  1,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_LITERAL),   // inCond
  (PCC_W | PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciLFSR = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_LFSR,
  "LFSR",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  1,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_REGISTER | PCC_LITERAL), // mdubuc - Should we use a special syntax for
                                // f (identifies FSRx)?
  PCC_REGISTER // outCond
};

pCodeInstruction pic16_pciMOVF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_MOVF,
  "MOVF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  (PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciMOVFW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_MOVFW,
  "MOVF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  (PCC_W | PCC_Z) // outCond
};

pCodeInstruction pic16_pciMOVFF = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_MOVFF,
  "MOVFF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  1,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  PCC_REGISTER2 // outCond
};

pCodeInstruction pic16_pciMOVLB = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   genericDestruct,
   genericPrint},
  POC_MOVLB,
  "MOVLB",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  1,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_NONE | PCC_LITERAL),   // inCond
  PCC_REGISTER // outCond - BSR
};

pCodeInstruction pic16_pciMOVLW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   genericDestruct,
   genericPrint},
  POC_MOVLW,
  "MOVLW",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  1,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_NONE | PCC_LITERAL),   // inCond
  PCC_W // outCond
};

pCodeInstruction pic16_pciMOVWF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_MOVWF,
  "MOVWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  PCC_W // outCond
};

pCodeInstruction pic16_pciMULLW = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   genericDestruct,
   genericPrint},
  POC_MULLW,
  "MULLW",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  1,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_LITERAL),   // inCond
  PCC_REGISTER // outCond - PROD
};

pCodeInstruction pic16_pciMULWF = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   genericDestruct,
   genericPrint},
  POC_MULWF,
  "MULWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER),   // inCond
  PCC_REGISTER // outCond - PROD
};

pCodeInstruction pic16_pciNEGF = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   genericDestruct,
   genericPrint},
  POC_NEGF,
  "NEGF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER, // inCond
  (PCC_REGISTER | PCC_C | PCC_DC | PCC_OV | PCC_N) // outCond
};

pCodeInstruction pic16_pciNOP = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   genericDestruct,
   genericPrint},
  POC_NOP,
  "NOP",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  0,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_NONE,   // inCond
  PCC_NONE // outCond
};

pCodeInstruction pic16_pciPOP = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   genericDestruct,
   genericPrint},
  POC_POP,
  "POP",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  0,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_NONE,  // inCond
  PCC_NONE   // outCond
};

pCodeInstruction pic16_pciPUSH = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   genericDestruct,
   genericPrint},
  POC_PUSH,
  "PUSH",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  0,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_NONE,  // inCond
  PCC_NONE   // outCond
};

pCodeInstruction pic16_pciRCALL = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   genericDestruct,
   genericPrint},
  POC_RCALL,
  "RCALL",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REL_ADDR,  // inCond
  PCC_NONE   // outCond
};

pCodeInstruction pic16_pciRETFIE = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   AnalyzeRETURN,
   genericDestruct,
   genericPrint},
  POC_RETFIE,
  "RETFIE",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  1,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_NONE,   // inCond
  PCC_NONE    // outCond (not true... affects the GIE bit too)
};

pCodeInstruction pic16_pciRETLW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   AnalyzeRETURN,
   genericDestruct,
   genericPrint},
  POC_RETLW,
  "RETLW",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  1,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_LITERAL,   // inCond
  PCC_W // outCond
};

pCodeInstruction pic16_pciRETURN = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   AnalyzeRETURN,
   genericDestruct,
   genericPrint},
  POC_RETURN,
  "RETURN",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  1,0,  // branch, skip
  0,    // literal operand
  0,    // RAM access bit
  1,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_NONE,   // inCond
  PCC_NONE // outCond
};
pCodeInstruction pic16_pciRLCF = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_RLCF,
  "RLCF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_C | PCC_REGISTER),   // inCond
  (PCC_REGISTER | PCC_C | PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciRLCFW = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_RLCFW,
  "RLCF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_C | PCC_REGISTER),   // inCond
  (PCC_W | PCC_C | PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciRLNCF = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_RLNCF,
  "RLNCF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  (PCC_REGISTER | PCC_Z | PCC_N) // outCond
};
pCodeInstruction pic16_pciRLNCFW = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_RLNCFW,
  "RLNCF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  (PCC_W | PCC_Z | PCC_N) // outCond
};
pCodeInstruction pic16_pciRRCF = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_RRCF,
  "RRCF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_C | PCC_REGISTER),   // inCond
  (PCC_REGISTER | PCC_C | PCC_Z | PCC_N) // outCond
};
pCodeInstruction pic16_pciRRCFW = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_RRCFW,
  "RRCF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_C | PCC_REGISTER),   // inCond
  (PCC_W | PCC_C | PCC_Z | PCC_N) // outCond
};
pCodeInstruction pic16_pciRRNCF = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_RRNCF,
  "RRNCF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  (PCC_REGISTER | PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciRRNCFW = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_RRNCFW,
  "RRNCF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  (PCC_W | PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciSETF = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_SETF,
  "SETF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,  // inCond
  PCC_REGISTER   // outCond
};

pCodeInstruction pic16_pciSUBLW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_SUBLW,
  "SUBLW",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  1,    // literal operand
  0,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_LITERAL),   // inCond
  (PCC_W | PCC_C | PCC_DC | PCC_Z | PCC_OV | PCC_N) // outCond
};

pCodeInstruction pic16_pciSUBFWB = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_SUBFWB,
  "SUBFWB",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER | PCC_C),   // inCond
  (PCC_W | PCC_C | PCC_DC | PCC_Z | PCC_OV | PCC_N) // outCond
};

pCodeInstruction pic16_pciSUBWF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_SUBWF,
  "SUBWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER),   // inCond
  (PCC_REGISTER | PCC_Z) // outCond
};

pCodeInstruction pic16_pciSUBFW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_SUBFW,
  "SUBWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER),   // inCond
  (PCC_W | PCC_Z | PCC_OV | PCC_N) // outCond
};

pCodeInstruction pic16_pciSUBFWB_D1 = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_SUBFWB_D1,
  "SUBFWB",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER | PCC_C),   // inCond
  (PCC_REGISTER | PCC_Z | PCC_C | PCC_DC | PCC_OV | PCC_N) // outCond
};

pCodeInstruction pic16_pciSUBFWB_D0 = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_SUBFWB_D0,
  "SUBFWB",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER | PCC_C),   // inCond
  (PCC_W | PCC_Z | PCC_C | PCC_DC | PCC_OV | PCC_N) // outCond
};

pCodeInstruction pic16_pciSUBWFB_D1 = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_SUBWFB_D1,
  "SUBWFB",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER | PCC_C),   // inCond
  (PCC_REGISTER | PCC_Z | PCC_C | PCC_DC | PCC_OV | PCC_N) // outCond
};

pCodeInstruction pic16_pciSUBWFB_D0 = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_SUBWFB_D0,
  "SUBWFB",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER | PCC_C),   // inCond
  (PCC_W | PCC_Z | PCC_C | PCC_DC | PCC_OV | PCC_N) // outCond
};

pCodeInstruction pic16_pciSWAPF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_SWAPF,
  "SWAPF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_REGISTER),   // inCond
  (PCC_REGISTER) // outCond
};

pCodeInstruction pic16_pciSWAPFW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_SWAPFW,
  "SWAPF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_REGISTER),   // inCond
  (PCC_W) // outCond
};


#if 0
// mdubuc - Remove TRIS

pCodeInstruction pic16_pciTRIS = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_TRIS,
  "TRIS",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  0,	// second memory operand
  POC_NOP,
  PCC_NONE,   // inCond
  PCC_REGISTER // outCond
};
#endif

pCodeInstruction pic16_pciTSTFSZ = { // mdubuc - New
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_TSTFSZ,
  "TSTFSZ",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  2,    // num ops
  0,0,  // dest, bit instruction
  1,1,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  PCC_REGISTER,   // inCond
  PCC_NONE // outCond
};

pCodeInstruction pic16_pciXORWF = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_XORWF,
  "XORWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  1,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER),   // inCond
  (PCC_REGISTER | PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciXORFW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_XORFW,
  "XORWF",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  3,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  0,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_REGISTER),   // inCond
  (PCC_W | PCC_Z | PCC_N) // outCond
};

pCodeInstruction pic16_pciXORLW = {
  {PC_OPCODE, NULL, NULL, 0, NULL, 
   //   genericAnalyze,
   genericDestruct,
   genericPrint},
  POC_XORLW,
  "XORLW",
  NULL, // from branch
  NULL, // to branch
  NULL, // label
  NULL, // operand
  NULL, // flow block
  NULL, // C source 
  1,    // num ops
  0,0,  // dest, bit instruction
  0,0,  // branch, skip
  1,    // literal operand
  1,    // RAM access bit
  0,    // fast call/return mode select bit
  0,	// second memory operand
  POC_NOP,
  (PCC_W | PCC_LITERAL),   // inCond
  (PCC_W | PCC_Z | PCC_C | PCC_DC | PCC_N) // outCond
};


#define MAX_PIC16MNEMONICS 100
pCodeInstruction *pic16Mnemonics[MAX_PIC16MNEMONICS];

#if OPT_DISABLE_PIC
/* This definition needs to be part of configure.in */
// #define USE_VSNPRINTF

#ifdef USE_VSNPRINTF
  // Alas, vsnprintf is not ANSI standard, and does not exist
  // on Solaris (and probably other non-Gnu flavored Unixes).

/*-----------------------------------------------------------------*/
/* SAFE_snprintf - like snprintf except the string pointer is      */
/*                 after the string has been printed to. This is   */
/*                 useful for printing to string as though if it   */
/*                 were a stream.                                  */
/*-----------------------------------------------------------------*/
void SAFE_snprintf(char **str, size_t *size, const  char  *format, ...)
{
  va_list val;
  int len;

  if(!str || !*str)
    return;

  va_start(val, format);

  vsnprintf(*str, *size, format, val);

  va_end (val);

  len = strlen(*str);
  if(len > *size) {
    fprintf(stderr,"WARNING, it looks like %s has overflowed\n",__FUNCTION__);
    fprintf(stderr,"len = %d is > str size %d\n",len,*size);
  }

  *str += len;
  *size -= len;

}

#else  //  USE_VSNPRINTF

// This version is *not* safe, despite the name.

void SAFE_snprintf(char **str, size_t *size, const  char  *format, ...)
{
  va_list val;
  int len;
  static char buffer[1024]; /* grossly conservative, but still not inherently safe */

  if(!str || !*str)
    return;

  va_start(val, format);

  vsprintf(buffer, format, val);
  va_end (val);

  len = strlen(buffer);
  if(len > *size) {
    fprintf(stderr,"WARNING, it looks like %s has overflowed\n",__FUNCTION__);
    fprintf(stderr,"len = %d is > str size %d\n",len, (int) *size);
  }

  strcpy(*str, buffer);
  *str += len;
  *size -= len;

}

#endif    //  USE_VSNPRINTF
    
#endif // OPT_DISABLE_PIC


extern  void pic16_initStack(int base_address, int size);
extern regs *pic16_allocProcessorRegister(int rIdx, char * name, short po_type, int alias);
extern regs *pic16_allocInternalRegister(int rIdx, char * name, short po_type, int alias);
extern void pic16_init_pic(char *);

void  pic16_pCodeInitRegisters(void)
{
  static int initialized=0;

  if(initialized)
    return;
  initialized = 1;

  pic16_initStack(0xfff, 8);
  pic16_init_pic(port->processor);

  pic16_pc_status.r = pic16_allocProcessorRegister(IDX_STATUS,"_STATUS", PO_STATUS, 0x80);
  pic16_pc_pcl.r = pic16_allocProcessorRegister(IDX_PCL,"PCL", PO_PCL, 0x80);
  pic16_pc_pclath.r = pic16_allocProcessorRegister(IDX_PCLATH,"_PCLATH", PO_PCLATH, 0x80);
  pic16_pc_fsr0.r = pic16_allocProcessorRegister(IDX_FSR0,"FSR0", PO_FSR0, 0x80);
  pic16_pc_indf0.r = pic16_allocProcessorRegister(IDX_INDF0,"INDF0", PO_INDF0, 0x80);
  pic16_pc_intcon.r = pic16_allocProcessorRegister(IDX_INTCON,"INTCON", PO_INTCON, 0x80);
  pic16_pc_wreg.r = pic16_allocProcessorRegister(IDX_WREG,"WREG", PO_WREG, 0x80);

  pic16_pc_status.rIdx = IDX_STATUS;
  pic16_pc_fsr0.rIdx = IDX_FSR0;
  pic16_pc_indf0.rIdx = IDX_INDF0;
  pic16_pc_intcon.rIdx = IDX_INTCON;
  pic16_pc_pcl.rIdx = IDX_PCL;
  pic16_pc_pclath.rIdx = IDX_PCLATH;
  pic16_pc_wreg.rIdx = IDX_WREG;

  pic16_pc_kzero.r = pic16_allocInternalRegister(IDX_KZ,"KZ",PO_GPR_REGISTER,0);
  pic16_pc_ssave.r = pic16_allocInternalRegister(IDX_SSAVE,"SSAVE", PO_GPR_REGISTER, 0x80);
  pic16_pc_wsave.r = pic16_allocInternalRegister(IDX_WSAVE,"WSAVE", PO_GPR_REGISTER, 0);

  pic16_pc_kzero.rIdx = IDX_KZ;
  pic16_pc_wsave.rIdx = IDX_WSAVE;
  pic16_pc_ssave.rIdx = IDX_SSAVE;

  /* probably should put this in a separate initialization routine */
  pb_dead_pcodes = newpBlock();

}

#if OPT_DISABLE_PIC
/*-----------------------------------------------------------------*/
/*  mnem2key - convert a pic mnemonic into a hash key              */
/*   (BTW - this spreads the mnemonics quite well)                 */
/*                                                                 */
/*-----------------------------------------------------------------*/

int mnem2key(char const *mnem)
{
  int key = 0;

  if(!mnem)
    return 0;

  while(*mnem) {

    key += toupper(*mnem++) +1;

  }

  return (key & 0x1f);

}
#endif // OPT_DISABLE_PIC

void pic16initMnemonics(void)
{
  int i = 0;
  int key;
  //  char *str;
  pCodeInstruction *pci;

  if(mnemonics_initialized)
    return;

  // NULL out the array before making the assignments
  // since we check the array contents below this initialization.

  for (i = 0; i < MAX_PIC16MNEMONICS; i++) {
    pic16Mnemonics[i] = NULL;
  }

  pic16Mnemonics[POC_ADDLW] = &pic16_pciADDLW;
  pic16Mnemonics[POC_ADDWF] = &pic16_pciADDWF;
  pic16Mnemonics[POC_ADDFW] = &pic16_pciADDFW;
  pic16Mnemonics[POC_ADDWFC] = &pic16_pciADDWFC;
  pic16Mnemonics[POC_ADDFWC] = &pic16_pciADDFWC;
  pic16Mnemonics[POC_ANDLW] = &pic16_pciANDLW;
  pic16Mnemonics[POC_ANDWF] = &pic16_pciANDWF;
  pic16Mnemonics[POC_ANDFW] = &pic16_pciANDFW;
  pic16Mnemonics[POC_BC] = &pic16_pciBC;
  pic16Mnemonics[POC_BCF] = &pic16_pciBCF;
  pic16Mnemonics[POC_BN] = &pic16_pciBN;
  pic16Mnemonics[POC_BNC] = &pic16_pciBNC;
  pic16Mnemonics[POC_BNN] = &pic16_pciBNN;
  pic16Mnemonics[POC_BNOV] = &pic16_pciBNOV;
  pic16Mnemonics[POC_BNZ] = &pic16_pciBNZ;
  pic16Mnemonics[POC_BOV] = &pic16_pciBOV;
  pic16Mnemonics[POC_BRA] = &pic16_pciBRA;
  pic16Mnemonics[POC_BSF] = &pic16_pciBSF;
  pic16Mnemonics[POC_BTFSC] = &pic16_pciBTFSC;
  pic16Mnemonics[POC_BTFSS] = &pic16_pciBTFSS;
  pic16Mnemonics[POC_BTG] = &pic16_pciBTG;
  pic16Mnemonics[POC_BZ] = &pic16_pciBZ;
  pic16Mnemonics[POC_CALL] = &pic16_pciCALL;
  pic16Mnemonics[POC_CLRF] = &pic16_pciCLRF;
  pic16Mnemonics[POC_CLRWDT] = &pic16_pciCLRWDT;
  pic16Mnemonics[POC_COMF] = &pic16_pciCOMF;
  pic16Mnemonics[POC_COMFW] = &pic16_pciCOMFW;
  pic16Mnemonics[POC_CPFSEQ] = &pic16_pciCPFSEQ;
  pic16Mnemonics[POC_CPFSGT] = &pic16_pciCPFSGT;
  pic16Mnemonics[POC_CPFSLT] = &pic16_pciCPFSLT;
  pic16Mnemonics[POC_DAW] = &pic16_pciDAW;
  pic16Mnemonics[POC_DCFSNZ] = &pic16_pciDCFSNZ;
  pic16Mnemonics[POC_DECF] = &pic16_pciDECF;
  pic16Mnemonics[POC_DECFW] = &pic16_pciDECFW;
  pic16Mnemonics[POC_DECFSZ] = &pic16_pciDECFSZ;
  pic16Mnemonics[POC_DECFSZW] = &pic16_pciDECFSZW;
  pic16Mnemonics[POC_GOTO] = &pic16_pciGOTO;
  pic16Mnemonics[POC_INCF] = &pic16_pciINCF;
  pic16Mnemonics[POC_INCFW] = &pic16_pciINCFW;
  pic16Mnemonics[POC_INCFSZ] = &pic16_pciINCFSZ;
  pic16Mnemonics[POC_INCFSZW] = &pic16_pciINCFSZW;
  pic16Mnemonics[POC_INFSNZ] = &pic16_pciINFSNZ;
  pic16Mnemonics[POC_IORWF] = &pic16_pciIORWF;
  pic16Mnemonics[POC_IORFW] = &pic16_pciIORFW;
  pic16Mnemonics[POC_IORLW] = &pic16_pciIORLW;
  pic16Mnemonics[POC_LFSR] = &pic16_pciLFSR;
  pic16Mnemonics[POC_MOVF] = &pic16_pciMOVF;
  pic16Mnemonics[POC_MOVFW] = &pic16_pciMOVFW;
  pic16Mnemonics[POC_MOVFF] = &pic16_pciMOVFF;
  pic16Mnemonics[POC_MOVLB] = &pic16_pciMOVLB;
  pic16Mnemonics[POC_MOVLW] = &pic16_pciMOVLW;
  pic16Mnemonics[POC_MOVWF] = &pic16_pciMOVWF;
  pic16Mnemonics[POC_MULLW] = &pic16_pciMULLW;
  pic16Mnemonics[POC_MULWF] = &pic16_pciMULWF;
  pic16Mnemonics[POC_NEGF] = &pic16_pciNEGF;
  pic16Mnemonics[POC_NOP] = &pic16_pciNOP;
  pic16Mnemonics[POC_POP] = &pic16_pciPOP;
  pic16Mnemonics[POC_PUSH] = &pic16_pciPUSH;
  pic16Mnemonics[POC_RCALL] = &pic16_pciRCALL;
  pic16Mnemonics[POC_RETFIE] = &pic16_pciRETFIE;
  pic16Mnemonics[POC_RETLW] = &pic16_pciRETLW;
  pic16Mnemonics[POC_RETURN] = &pic16_pciRETURN;
  pic16Mnemonics[POC_RLCF] = &pic16_pciRLCF;
  pic16Mnemonics[POC_RLCFW] = &pic16_pciRLCFW;
  pic16Mnemonics[POC_RLCF] = &pic16_pciRLNCF;
  pic16Mnemonics[POC_RLNCFW] = &pic16_pciRLNCFW;
  pic16Mnemonics[POC_RRCF] = &pic16_pciRRCF;
  pic16Mnemonics[POC_RRCFW] = &pic16_pciRRCFW;
  pic16Mnemonics[POC_RRNCF] = &pic16_pciRRNCF;
  pic16Mnemonics[POC_RRNCFW] = &pic16_pciRRNCFW;
  pic16Mnemonics[POC_SETF] = &pic16_pciSETF;
  pic16Mnemonics[POC_SUBLW] = &pic16_pciSUBLW;
  pic16Mnemonics[POC_SUBWF] = &pic16_pciSUBWF;
  pic16Mnemonics[POC_SUBFW] = &pic16_pciSUBFW;
  pic16Mnemonics[POC_SUBWFB_D0] = &pic16_pciSUBWFB_D0;
  pic16Mnemonics[POC_SUBWFB_D1] = &pic16_pciSUBWFB_D1;
  pic16Mnemonics[POC_SUBFWB_D0] = &pic16_pciSUBFWB_D0;
  pic16Mnemonics[POC_SUBFWB_D1] = &pic16_pciSUBFWB_D1;
  pic16Mnemonics[POC_SWAPF] = &pic16_pciSWAPF;
  pic16Mnemonics[POC_SWAPFW] = &pic16_pciSWAPFW;
//  pic16Mnemonics[POC_TRIS] = &pic16_pciTRIS;
  pic16Mnemonics[POC_TSTFSZ] = &pic16_pciTSTFSZ;
  pic16Mnemonics[POC_XORLW] = &pic16_pciXORLW;
  pic16Mnemonics[POC_XORWF] = &pic16_pciXORWF;
  pic16Mnemonics[POC_XORFW] = &pic16_pciXORFW;

  for(i=0; i<MAX_PIC16MNEMONICS; i++)
    if(pic16Mnemonics[i])
      hTabAddItem(&pic16MnemonicsHash, mnem2key(pic16Mnemonics[i]->mnemonic), pic16Mnemonics[i]);
  pci = hTabFirstItem(pic16MnemonicsHash, &key);

  while(pci) {
    DFPRINTF((stderr, "element %d key %d, mnem %s\n",i++,key,pci->mnemonic));
    pci = hTabNextItem(pic16MnemonicsHash, &key);
  }

  mnemonics_initialized = 1;
}

int pic16_getpCodePeepCommand(char *cmd);

int pic16_getpCode(char *mnem,unsigned dest)
{

  pCodeInstruction *pci;
  int key = mnem2key(mnem);

  if(!mnemonics_initialized)
    pic16initMnemonics();

  pci = hTabFirstItemWK(pic16MnemonicsHash, key);

  while(pci) {

    if(STRCASECMP(pci->mnemonic, mnem) == 0) {
      if((pci->num_ops <= 1) || (pci->isModReg == dest) || (pci->isBitInst) ||
         (pci->num_ops <= 2 && pci->isAccess) ||
         (pci->num_ops <= 2 && pci->isFastCall))
	return(pci->op);
    }

    pci = hTabNextItemWK (pic16MnemonicsHash);
  
  }

  return -1;
}

/*-----------------------------------------------------------------*
 * pic16initpCodePeepCommands
 *
 *-----------------------------------------------------------------*/
void pic16initpCodePeepCommands(void)
{

  int key, i;
  peepCommand *pcmd;

  i = 0;
  do {
    hTabAddItem(&pic16pCodePeepCommandsHash, 
		mnem2key(peepCommands[i].cmd), &peepCommands[i]);
    i++;
  } while (peepCommands[i].cmd);

  pcmd = hTabFirstItem(pic16pCodePeepCommandsHash, &key);

  while(pcmd) {
    //fprintf(stderr, "peep command %s  key %d\n",pcmd->cmd,pcmd->id);
    pcmd = hTabNextItem(pic16pCodePeepCommandsHash, &key);
  }

}

/*-----------------------------------------------------------------
 *
 *
 *-----------------------------------------------------------------*/

int pic16_getpCodePeepCommand(char *cmd)
{

  peepCommand *pcmd;
  int key = mnem2key(cmd);


  pcmd = hTabFirstItemWK(pic16pCodePeepCommandsHash, key);

  while(pcmd) {
    // fprintf(stderr," comparing %s to %s\n",pcmd->cmd,cmd);
    if(STRCASECMP(pcmd->cmd, cmd) == 0) {
      return pcmd->id;
    }

    pcmd = hTabNextItemWK (pic16pCodePeepCommandsHash);
  
  }

  return -1;
}

static char getpBlock_dbName(pBlock *pb)
{
  if(!pb)
    return 0;

  if(pb->cmemmap)
    return pb->cmemmap->dbName;

  return pb->dbName;
}
void pic16_pBlockConvert2ISR(pBlock *pb)
{
  if(!pb)
    return;

  if(pb->cmemmap)
    pb->cmemmap = NULL;

  pb->dbName = 'I';
}

/*-----------------------------------------------------------------*/
/* pic16_movepBlock2Head - given the dbname of a pBlock, move all  */
/*                   instances to the front of the doubly linked   */
/*                   list of pBlocks                               */
/*-----------------------------------------------------------------*/

void pic16_movepBlock2Head(char dbName)
{
  pBlock *pb;

  pb = the_pFile->pbHead;

  while(pb) {

    if(getpBlock_dbName(pb) == dbName) {
      pBlock *pbn = pb->next;
      pb->next = the_pFile->pbHead;
      the_pFile->pbHead->prev = pb;
      the_pFile->pbHead = pb;

      if(pb->prev)
	pb->prev->next = pbn;

      // If the pBlock that we just moved was the last
      // one in the link of all of the pBlocks, then we
      // need to point the tail to the block just before
      // the one we moved.
      // Note: if pb->next is NULL, then pb must have 
      // been the last pBlock in the chain.

      if(pbn)
	pbn->prev = pb->prev;
      else
	the_pFile->pbTail = pb->prev;

      pb = pbn;

    } else
      pb = pb->next;

  }

}

void pic16_copypCode(FILE *of, char dbName)
{
  pBlock *pb;

  if(!of || !the_pFile)
    return;

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if(getpBlock_dbName(pb) == dbName) {
      pBlockStats(of,pb);
      pic16_printpBlock(of,pb);
    }
  }

}
void pic16_pcode_test(void)
{

  DFPRINTF((stderr,"pcode is alive!\n"));

  //initMnemonics();

  if(the_pFile) {

    pBlock *pb;
    FILE *pFile;
    char buffer[100];

    /* create the file name */
    strcpy(buffer,dstFileName);
    strcat(buffer,".p");

    if( !(pFile = fopen(buffer, "w" ))) {
      werror(E_FILE_OPEN_ERR,buffer);
      exit(1);
    }

    fprintf(pFile,"pcode dump\n\n");

    for(pb = the_pFile->pbHead; pb; pb = pb->next) {
      fprintf(pFile,"\n\tNew pBlock\n\n");
      if(pb->cmemmap)
	fprintf(pFile,"%s",pb->cmemmap->sname);
      else
	fprintf(pFile,"internal pblock");

      fprintf(pFile,", dbName =%c\n",getpBlock_dbName(pb));
      pic16_printpBlock(pFile,pb);
    }
  }
}
/*-----------------------------------------------------------------*/
/* int RegCond(pCodeOp *pcop) - if pcop points to the STATUS reg-  */
/*      ister, RegCond will return the bit being referenced.       */
/*                                                                 */
/* fixme - why not just OR in the pcop bit field                   */
/*-----------------------------------------------------------------*/

static int RegCond(pCodeOp *pcop)
{

  if(!pcop)
    return 0;

  if(pcop->type == PO_GPR_BIT  && !strcmp(pcop->name, pic16_pc_status.pcop.name)) {
    switch(PCORB(pcop)->bit) {
    case PIC_C_BIT:
      return PCC_C;
    case PIC_DC_BIT:
	return PCC_DC;
    case PIC_Z_BIT:
      return PCC_Z;
    }

  }

  return 0;
}

/*-----------------------------------------------------------------*/
/* pic16_newpCode - create and return a newly initialized pCode          */
/*                                                                 */
/*  fixme - rename this                                            */
/*                                                                 */
/* The purpose of this routine is to create a new Instruction      */
/* pCode. This is called by gen.c while the assembly code is being */
/* generated.                                                      */
/*                                                                 */
/* Inouts:                                                         */
/*  PIC_OPCODE op - the assembly instruction we wish to create.    */
/*                  (note that the op is analogous to but not the  */
/*                  same thing as the opcode of the instruction.)  */
/*  pCdoeOp *pcop - pointer to the operand of the instruction.     */
/*                                                                 */
/* Outputs:                                                        */
/*  a pointer to the new malloc'd pCode is returned.               */
/*                                                                 */
/*                                                                 */
/*                                                                 */
/*-----------------------------------------------------------------*/
pCode *pic16_newpCode (PIC_OPCODE op, pCodeOp *pcop)
{
  pCodeInstruction *pci ;

  if(!mnemonics_initialized)
    pic16initMnemonics();
    
  pci = Safe_calloc(1, sizeof(pCodeInstruction));

  if((op>=0) && (op < MAX_PIC16MNEMONICS) && pic16Mnemonics[op]) {
    memcpy(pci, pic16Mnemonics[op], sizeof(pCodeInstruction));
    pci->pcop = pcop;

    if(pci->inCond & PCC_EXAMINE_PCOP)
      pci->inCond  |= RegCond(pcop);

    if(pci->outCond & PCC_EXAMINE_PCOP)
      pci->outCond  |= RegCond(pcop);

    pci->pc.prev = pci->pc.next = NULL;
    return (pCode *)pci;
  }

  fprintf(stderr, "pCode mnemonic error %s,%d\n",__FUNCTION__,__LINE__);
  exit(1);

  return NULL;
}     	

/*-----------------------------------------------------------------*/
/* pic16_newpCodeWild - create a "wild" as in wild card pCode            */
/*                                                                 */
/* Wild pcodes are used during the peep hole optimizer to serve    */
/* as place holders for any instruction. When a snippet of code is */
/* compared to a peep hole rule, the wild card opcode will match   */
/* any instruction. However, the optional operand and label are    */
/* additional qualifiers that must also be matched before the      */
/* line (of assembly code) is declared matched. Note that the      */
/* operand may be wild too.                                        */
/*                                                                 */
/*   Note, a wild instruction is specified just like a wild var:   */
/*      %4     ; A wild instruction,                               */
/*  See the peeph.def file for additional examples                 */
/*                                                                 */
/*-----------------------------------------------------------------*/

pCode *pic16_newpCodeWild(int pCodeID, pCodeOp *optional_operand, pCodeOp *optional_label)
{

  pCodeWild *pcw;
    
  pcw = Safe_calloc(1,sizeof(pCodeWild));

  pcw->pci.pc.type = PC_WILD;
  pcw->pci.pc.prev = pcw->pci.pc.next = NULL;
  pcw->pci.from = pcw->pci.to = pcw->pci.label = NULL;
  pcw->pci.pc.pb = NULL;

  //  pcw->pci.pc.analyze = genericAnalyze;
  pcw->pci.pc.destruct = genericDestruct;
  pcw->pci.pc.print = genericPrint;

  pcw->id = pCodeID;              // this is the 'n' in %n
  pcw->operand = optional_operand;
  pcw->label   = optional_label;

  pcw->mustBeBitSkipInst = 0;
  pcw->mustNotBeBitSkipInst = 0;
  pcw->invertBitSkipInst = 0;

  return ( (pCode *)pcw);
  
}

 /*-----------------------------------------------------------------*/
/* newPcodeInlineP - create a new pCode from a char string           */
/*-----------------------------------------------------------------*/


pCode *pic16_newpCodeInlineP(char *cP)
{

  pCodeComment *pcc ;
    
  pcc = Safe_calloc(1,sizeof(pCodeComment));

  pcc->pc.type = PC_INLINE;
  pcc->pc.prev = pcc->pc.next = NULL;
  //pcc->pc.from = pcc->pc.to = pcc->pc.label = NULL;
  pcc->pc.pb = NULL;

  //  pcc->pc.analyze = genericAnalyze;
  pcc->pc.destruct = genericDestruct;
  pcc->pc.print = genericPrint;

  if(cP)
    pcc->comment = Safe_strdup(cP);
  else
    pcc->comment = NULL;

  return ( (pCode *)pcc);

}

/*-----------------------------------------------------------------*/
/* newPcodeCharP - create a new pCode from a char string           */
/*-----------------------------------------------------------------*/

pCode *pic16_newpCodeCharP(char *cP)
{

  pCodeComment *pcc ;
    
  pcc = Safe_calloc(1,sizeof(pCodeComment));

  pcc->pc.type = PC_COMMENT;
  pcc->pc.prev = pcc->pc.next = NULL;
  //pcc->pc.from = pcc->pc.to = pcc->pc.label = NULL;
  pcc->pc.pb = NULL;

  //  pcc->pc.analyze = genericAnalyze;
  pcc->pc.destruct = genericDestruct;
  pcc->pc.print = genericPrint;

  if(cP)
    pcc->comment = Safe_strdup(cP);
  else
    pcc->comment = NULL;

  return ( (pCode *)pcc);

}

/*-----------------------------------------------------------------*/
/* pic16_newpCodeFunction -                                              */
/*-----------------------------------------------------------------*/


pCode *pic16_newpCodeFunction(char *mod,char *f)
{
  pCodeFunction *pcf;

  pcf = Safe_calloc(1,sizeof(pCodeFunction));
  //_ALLOC(pcf,sizeof(pCodeFunction));

  pcf->pc.type = PC_FUNCTION;
  pcf->pc.prev = pcf->pc.next = NULL;
  //pcf->pc.from = pcf->pc.to = pcf->pc.label = NULL;
  pcf->pc.pb = NULL;

  //  pcf->pc.analyze = genericAnalyze;
  pcf->pc.destruct = genericDestruct;
  pcf->pc.print = pCodePrintFunction;

  pcf->ncalled = 0;

  if(mod) {
    //_ALLOC_ATOMIC(pcf->modname,strlen(mod)+1);
    pcf->modname = Safe_calloc(1,strlen(mod)+1);
    strcpy(pcf->modname,mod);
  } else
    pcf->modname = NULL;

  if(f) {
    //_ALLOC_ATOMIC(pcf->fname,strlen(f)+1);
    pcf->fname = Safe_calloc(1,strlen(f)+1);
    strcpy(pcf->fname,f);
  } else
    pcf->fname = NULL;

  return ( (pCode *)pcf);

}

/*-----------------------------------------------------------------*/
/* pic16_newpCodeFlow                                                    */
/*-----------------------------------------------------------------*/
static void destructpCodeFlow(pCode *pc)
{
  if(!pc || !isPCFL(pc))
    return;

/*
  if(PCFL(pc)->from)
  if(PCFL(pc)->to)
*/
  pic16_unlinkpCode(pc);

  deleteSet(&PCFL(pc)->registers);
  deleteSet(&PCFL(pc)->from);
  deleteSet(&PCFL(pc)->to);
  free(pc);

}

pCode *pic16_newpCodeFlow(void )
{
  pCodeFlow *pcflow;

  //_ALLOC(pcflow,sizeof(pCodeFlow));
  pcflow = Safe_calloc(1,sizeof(pCodeFlow));

  pcflow->pc.type = PC_FLOW;
  pcflow->pc.prev = pcflow->pc.next = NULL;
  pcflow->pc.pb = NULL;

  //  pcflow->pc.analyze = genericAnalyze;
  pcflow->pc.destruct = destructpCodeFlow;
  pcflow->pc.print = genericPrint;

  pcflow->pc.seq = GpcFlowSeq++;

  pcflow->from = pcflow->to = NULL;

  pcflow->inCond = PCC_NONE;
  pcflow->outCond = PCC_NONE;

  pcflow->firstBank = -1;
  pcflow->lastBank = -1;

  pcflow->FromConflicts = 0;
  pcflow->ToConflicts = 0;

  pcflow->end = NULL;

  pcflow->registers = newSet();

  return ( (pCode *)pcflow);

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
pCodeFlowLink *pic16_newpCodeFlowLink(pCodeFlow *pcflow)
{
  pCodeFlowLink *pcflowLink;

  pcflowLink = Safe_calloc(1,sizeof(pCodeFlowLink));

  pcflowLink->pcflow = pcflow;
  pcflowLink->bank_conflict = 0;

  return pcflowLink;
}

/*-----------------------------------------------------------------*/
/* pic16_newpCodeCSource - create a new pCode Source Symbol        */
/*-----------------------------------------------------------------*/

pCode *pic16_newpCodeCSource(int ln, char *f, char *l)
{

  pCodeCSource *pccs;
    
  pccs = Safe_calloc(1,sizeof(pCodeCSource));

  pccs->pc.type = PC_CSOURCE;
  pccs->pc.prev = pccs->pc.next = NULL;
  pccs->pc.pb = NULL;

  pccs->pc.destruct = genericDestruct;
  pccs->pc.print = genericPrint;

  pccs->line_number = ln;
  if(l)
    pccs->line = Safe_strdup(l);
  else
    pccs->line = NULL;

  if(f)
    pccs->file_name = Safe_strdup(f);
  else
    pccs->file_name = NULL;

  return ( (pCode *)pccs);

}


/*******************************************************************/
/* pic16_newpCodeAsmDir - create a new pCode Assembler Directive   */
/*			added by VR 6-Jun-2003                     */
/*******************************************************************/

pCode *pic16_newpCodeAsmDir(char *asdir, char *argfmt, ...)
{
  pCodeAsmDir *pcad;
  va_list ap;
  char buffer[256];		// how long can a directive be?!
  char *lbp=buffer;
  

	pcad = Safe_calloc(1, sizeof(pCodeAsmDir));
	pcad->pc.type = PC_ASMDIR;
	pcad->pc.prev = pcad->pc.next = NULL;
	pcad->pc.pb = NULL;
	
	pcad->pc.destruct = genericDestruct;
	pcad->pc.print = genericPrint;

	if(asdir && *asdir) {
		pcad->directive = Safe_strdup( asdir );
	}
	
	va_start(ap, argfmt);
	
	if(argfmt && *argfmt)
		vsprintf(buffer, argfmt, ap);
	
	va_end(ap);
	
	while(isspace(*lbp))lbp++;
	
	if(lbp && *lbp)
		pcad->arg = Safe_strdup( lbp );

  return ((pCode *)pcad);
}

/*-----------------------------------------------------------------*/
/* pCodeLabelDestruct - free memory used by a label.               */
/*-----------------------------------------------------------------*/
static void pCodeLabelDestruct(pCode *pc)
{

  if(!pc)
    return;

  if((pc->type == PC_LABEL) && PCL(pc)->label)
    free(PCL(pc)->label);

  free(pc);

}

pCode *pic16_newpCodeLabel(char *name, int key)
{

  char *s = buffer;
  pCodeLabel *pcl;
    
  pcl = Safe_calloc(1,sizeof(pCodeLabel) );

  pcl->pc.type = PC_LABEL;
  pcl->pc.prev = pcl->pc.next = NULL;
  //pcl->pc.from = pcl->pc.to = pcl->pc.label = NULL;
  pcl->pc.pb = NULL;

  //  pcl->pc.analyze = genericAnalyze;
  pcl->pc.destruct = pCodeLabelDestruct;
  pcl->pc.print = pCodePrintLabel;

  pcl->key = key;

  pcl->label = NULL;
  if(key>0) {
    sprintf(s,"_%05d_DS_",key);
  } else
    s = name;

  if(s)
    pcl->label = Safe_strdup(s);

  //fprintf(stderr,"pic16_newpCodeLabel: key=%d, name=%s\n",key, ((s)?s:""));
  return ( (pCode *)pcl);

}


/*-----------------------------------------------------------------*/
/* newpBlock - create and return a pointer to a new pBlock         */
/*-----------------------------------------------------------------*/
static pBlock *newpBlock(void)
{

  pBlock *PpB;

  PpB = Safe_calloc(1,sizeof(pBlock) );
  PpB->next = PpB->prev = NULL;

  PpB->function_entries = PpB->function_exits = PpB->function_calls = NULL;
  PpB->tregisters = NULL;
  PpB->visited = 0;
  PpB->FlowTree = NULL;

  return PpB;

}

/*-----------------------------------------------------------------*/
/* pic16_newpCodeChain - create a new chain of pCodes                    */
/*-----------------------------------------------------------------*
 *
 *  This function will create a new pBlock and the pointer to the
 *  pCode that is passed in will be the first pCode in the block.
 *-----------------------------------------------------------------*/


pBlock *pic16_newpCodeChain(memmap *cm,char c, pCode *pc)
{

  pBlock *pB  = newpBlock();

  pB->pcHead  = pB->pcTail = pc;
  pB->cmemmap = cm;
  pB->dbName  = c;

  return pB;
}

/*-----------------------------------------------------------------*/
/* pic16_newpCodeOpLabel - Create a new label given the key              */
/*  Note, a negative key means that the label is part of wild card */
/*  (and hence a wild card label) used in the pCodePeep            */
/*   optimizations).                                               */
/*-----------------------------------------------------------------*/

pCodeOp *pic16_newpCodeOpLabel(char *name, int key)
{
  char *s=NULL;
  static int label_key=-1;

  pCodeOp *pcop;

  pcop = Safe_calloc(1,sizeof(pCodeOpLabel) );
  pcop->type = PO_LABEL;

  pcop->name = NULL;

  if(key>0)
    sprintf(s=buffer,"_%05d_DS_",key);
  else 
    s = name, key = label_key--;

  if(s)
    pcop->name = Safe_strdup(s);

  ((pCodeOpLabel *)pcop)->key = key;

  //fprintf(stderr,"pic16_newpCodeOpLabel: key=%d, name=%s\n",key,((s)?s:""));
  return pcop;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
pCodeOp *pic16_newpCodeOpLit(int lit)
{
  char *s = buffer;
  pCodeOp *pcop;


  pcop = Safe_calloc(1,sizeof(pCodeOpLit) );
  pcop->type = PO_LITERAL;

  pcop->name = NULL;
  if(lit>=0) {
    sprintf(s,"0x%02x",lit);
    if(s)
      pcop->name = Safe_strdup(s);
  }

  ((pCodeOpLit *)pcop)->lit = lit;

  return pcop;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
pCodeOp *pic16_newpCodeOpImmd(char *name, int offset, int index, int code_space)
{
  pCodeOp *pcop;

  pcop = Safe_calloc(1,sizeof(pCodeOpImmd) );
  pcop->type = PO_IMMEDIATE;
  if(name) {
    regs *r = pic16_dirregWithName(name);
    pcop->name = Safe_strdup(name);
    PCOI(pcop)->r = r;
    if(r) {
      //fprintf(stderr, " pic16_newpCodeOpImmd reg %s exists\n",name);
      PCOI(pcop)->rIdx = r->rIdx;
    } else {
      //fprintf(stderr, " pic16_newpCodeOpImmd reg %s doesn't exist\n",name);
      PCOI(pcop)->rIdx = -1;
    }
    //fprintf(stderr,"%s %s %d\n",__FUNCTION__,name,offset);
  } else {
    pcop->name = NULL;
  }

  PCOI(pcop)->index = index;
  PCOI(pcop)->offset = offset;
  PCOI(pcop)->_const = code_space;

  return pcop;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
pCodeOp *pic16_newpCodeOpWild(int id, pCodeWildBlock *pcwb, pCodeOp *subtype)
{
  char *s = buffer;
  pCodeOp *pcop;


  if(!pcwb || !subtype) {
    fprintf(stderr, "Wild opcode declaration error: %s-%d\n",__FILE__,__LINE__);
    exit(1);
  }

  pcop = Safe_calloc(1,sizeof(pCodeOpWild));
  pcop->type = PO_WILD;
  sprintf(s,"%%%d",id);
  pcop->name = Safe_strdup(s);

  PCOW(pcop)->id = id;
  PCOW(pcop)->pcwb = pcwb;
  PCOW(pcop)->subtype = subtype;
  PCOW(pcop)->matched = NULL;

  return pcop;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
pCodeOp *pic16_newpCodeOpBit(char *s, int bit, int inBitSpace)
{
  pCodeOp *pcop;

  pcop = Safe_calloc(1,sizeof(pCodeOpRegBit) );
  pcop->type = PO_GPR_BIT;
  if(s)
    pcop->name = Safe_strdup(s);   
  else
    pcop->name = NULL;

  PCORB(pcop)->bit = bit;
  PCORB(pcop)->inBitSpace = inBitSpace;

  /* pCodeOpBit is derived from pCodeOpReg. We need to init this too */
  PCOR(pcop)->r = NULL;
  PCOR(pcop)->rIdx = 0;
  return pcop;
}

/*-----------------------------------------------------------------*
 * pCodeOp *pic16_newpCodeOpReg(int rIdx) - allocate a new register
 *
 * If rIdx >=0 then a specific register from the set of registers
 * will be selected. If rIdx <0, then a new register will be searched
 * for.
 *-----------------------------------------------------------------*/

pCodeOp *pic16_newpCodeOpReg(int rIdx)
{
  pCodeOp *pcop;

  pcop = Safe_calloc(1,sizeof(pCodeOpReg) );

  pcop->name = NULL;

  if(rIdx >= 0) {
    PCOR(pcop)->rIdx = rIdx;
    PCOR(pcop)->r = pic16_regWithIdx(rIdx);
  } else {
    PCOR(pcop)->r = pic16_findFreeReg(REG_GPR);

    if(PCOR(pcop)->r)
      PCOR(pcop)->rIdx = PCOR(pcop)->r->rIdx;
  }

  pcop->type = PCOR(pcop)->r->pc_type;

  return pcop;
}

pCodeOp *pic16_newpCodeOpRegFromStr(char *name)
{
  pCodeOp *pcop;

  pcop = Safe_calloc(1,sizeof(pCodeOpReg) );
  PCOR(pcop)->r = pic16_allocRegByName(name, 1);
  PCOR(pcop)->rIdx = PCOR(pcop)->r->rIdx;
  pcop->type = PCOR(pcop)->r->pc_type;
  pcop->name = PCOR(pcop)->r->name;

  return pcop;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

pCodeOp *pic16_newpCodeOp(char *name, PIC_OPTYPE type)
{
  pCodeOp *pcop;

  switch(type) {
  case PO_BIT:
  case PO_GPR_BIT:
    pcop = pic16_newpCodeOpBit(name, -1,0);
    break;

  case PO_LITERAL:
    pcop = pic16_newpCodeOpLit(-1);
    break;

  case PO_LABEL:
    pcop = pic16_newpCodeOpLabel(NULL,-1);
    break;
  case PO_GPR_TEMP:
    pcop = pic16_newpCodeOpReg(-1);
    break;

  case PO_GPR_REGISTER:
    if(name)
      pcop = pic16_newpCodeOpRegFromStr(name);
    else
      pcop = pic16_newpCodeOpReg(-1);
    break;

  default:
    pcop = Safe_calloc(1,sizeof(pCodeOp) );
    pcop->type = type;
    if(name)
      pcop->name = Safe_strdup(name);   
    else
      pcop->name = NULL;
  }

  return pcop;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
void pic16_pCodeConstString(char *name, char *value)
{
  pBlock *pb;

  //  fprintf(stderr, " %s  %s  %s\n",__FUNCTION__,name,value);

  if(!name || !value)
    return;

  pb = pic16_newpCodeChain(NULL, 'P',pic16_newpCodeCharP("; Starting pCode block"));

  pic16_addpBlock(pb);

  sprintf(buffer,"; %s = %s",name,value);
  
  pic16_addpCode2pBlock(pb,pic16_newpCodeCharP(buffer));
  pic16_addpCode2pBlock(pb,pic16_newpCodeLabel(name,-1));

  do {
    pic16_addpCode2pBlock(pb,pic16_newpCode(POC_RETLW,pic16_newpCodeOpLit(*value)));
  }while (*value++);


}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
#if 0
static void pCodeReadCodeTable(void)
{
  pBlock *pb;

  fprintf(stderr, " %s\n",__FUNCTION__);

  pb = pic16_newpCodeChain(NULL, 'P',pic16_newpCodeCharP("; Starting pCode block"));

  pic16_addpBlock(pb);

  pic16_addpCode2pBlock(pb,pic16_newpCodeCharP("; ReadCodeTable - built in function"));
  pic16_addpCode2pBlock(pb,pic16_newpCodeCharP("; Inputs: temp1,temp2 = code pointer"));
  pic16_addpCode2pBlock(pb,pic16_newpCodeCharP("; Outpus: W (from RETLW at temp2:temp1)"));
  pic16_addpCode2pBlock(pb,pic16_newpCodeLabel("ReadCodeTable:",-1));

  pic16_addpCode2pBlock(pb,pic16_newpCode(POC_MOVFW,pic16_newpCodeOpRegFromStr("temp2")));
  pic16_addpCode2pBlock(pb,pic16_newpCode(POC_MOVWF,pic16_newpCodeOpRegFromStr("PCLATH")));
  pic16_addpCode2pBlock(pb,pic16_newpCode(POC_MOVFW,pic16_newpCodeOpRegFromStr("temp1")));
  pic16_addpCode2pBlock(pb,pic16_newpCode(POC_MOVWF,pic16_newpCodeOpRegFromStr("PCL")));


}
#endif
/*-----------------------------------------------------------------*/
/* pic16_addpCode2pBlock - place the pCode into the pBlock linked list   */
/*-----------------------------------------------------------------*/
void pic16_addpCode2pBlock(pBlock *pb, pCode *pc)
{

  if(!pc)
    return;

  if(!pb->pcHead) {
    /* If this is the first pcode to be added to a block that
     * was initialized with a NULL pcode, then go ahead and
     * make this pcode the head and tail */
    pb->pcHead  = pb->pcTail = pc;
  } else {
    //    if(pb->pcTail)
    pb->pcTail->next = pc;

    pc->prev = pb->pcTail;
    pc->pb = pb;

    pb->pcTail = pc;
  }
}

/*-----------------------------------------------------------------*/
/* pic16_addpBlock - place a pBlock into the pFile                 */
/*-----------------------------------------------------------------*/
void pic16_addpBlock(pBlock *pb)
{
  // fprintf(stderr," Adding pBlock: dbName =%c\n",getpBlock_dbName(pb));

  if(!the_pFile) {
    /* First time called, we'll pass through here. */
    //_ALLOC(the_pFile,sizeof(pFile));
    the_pFile = Safe_calloc(1,sizeof(pFile));
    the_pFile->pbHead = the_pFile->pbTail = pb;
    the_pFile->functions = NULL;
    return;
  }

  the_pFile->pbTail->next = pb;
  pb->prev = the_pFile->pbTail;
  pb->next = NULL;
  the_pFile->pbTail = pb;
}

/*-----------------------------------------------------------------*/
/* removepBlock - remove a pBlock from the pFile                   */
/*-----------------------------------------------------------------*/
static void removepBlock(pBlock *pb)
{
  pBlock *pbs;

  if(!the_pFile)
    return;


  //fprintf(stderr," Removing pBlock: dbName =%c\n",getpBlock_dbName(pb));

  for(pbs = the_pFile->pbHead; pbs; pbs = pbs->next) {
    if(pbs == pb) {

      if(pbs == the_pFile->pbHead)
	the_pFile->pbHead = pbs->next;

      if (pbs == the_pFile->pbTail) 
	the_pFile->pbTail = pbs->prev;

      if(pbs->next)
	pbs->next->prev = pbs->prev;

      if(pbs->prev)
	pbs->prev->next = pbs->next;

      return;

    }
  }

  fprintf(stderr, "Warning: call to %s:%s didn't find pBlock\n",__FILE__,__FUNCTION__);

}

/*-----------------------------------------------------------------*/
/* printpCode - write the contents of a pCode to a file            */
/*-----------------------------------------------------------------*/
static void printpCode(FILE *of, pCode *pc)
{

  if(!pc || !of)
    return;

  if(pc->print) {
    pc->print(of,pc);
    return;
  }

  fprintf(of,"warning - unable to print pCode\n");
}

/*-----------------------------------------------------------------*/
/* pic16_printpBlock - write the contents of a pBlock to a file    */
/*-----------------------------------------------------------------*/
void pic16_printpBlock(FILE *of, pBlock *pb)
{
  pCode *pc;

  if(!pb)
    return;

  if(!of)
    of = stderr;

  for(pc = pb->pcHead; pc; pc = pc->next)
    printpCode(of,pc);

}

/*-----------------------------------------------------------------*/
/*                                                                 */
/*       pCode processing                                          */
/*                                                                 */
/*                                                                 */
/*                                                                 */
/*-----------------------------------------------------------------*/

void pic16_unlinkpCode(pCode *pc)
{


  if(pc) {
#ifdef PCODE_DEBUG
    fprintf(stderr,"Unlinking: ");
    printpCode(stderr, pc);
#endif
    if(pc->prev) 
      pc->prev->next = pc->next;
    if(pc->next)
      pc->next->prev = pc->prev;

    pc->prev = pc->next = NULL;
  }
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

static void genericDestruct(pCode *pc)
{

  pic16_unlinkpCode(pc);

  if(isPCI(pc)) {
    /* For instructions, tell the register (if there's one used)
     * that it's no longer needed */
    regs *reg = pic16_getRegFromInstruction(pc);
    if(reg)
      deleteSetItem (&(reg->reglives.usedpCodes),pc);

	if(PCI(pc)->is2MemOp) {
		reg = pic16_getRegFromInstruction2(pc);
		if(reg)
			deleteSetItem(&(reg->reglives.usedpCodes), pc);
	}
  }

  /* Instead of deleting the memory used by this pCode, mark
   * the object as bad so that if there's a pointer to this pCode
   * dangling around somewhere then (hopefully) when the type is
   * checked we'll catch it.
   */

  pc->type = PC_BAD;

  pic16_addpCode2pBlock(pb_dead_pcodes, pc);

  //free(pc);

}



#if 0
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void pBlockRegs(FILE *of, pBlock *pb)
{

  regs  *r;

  r = setFirstItem(pb->tregisters);
  while (r) {
    r = setNextItem(pb->tregisters);
  }
}
#endif

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
char *pic16_get_op(pCodeOp *pcop,char *buffer, size_t size)
{
  regs *r;
  static char b[50];
  char *s;
  int use_buffer = 1;    // copy the string to the passed buffer pointer

  if(!buffer) {
    buffer = b;
    size = sizeof(b);
    use_buffer = 0;     // Don't bother copying the string to the buffer.
  } 

  if(pcop) {
    switch(pcop->type) {
    case PO_INDF0:
    case PO_FSR0:
      if(use_buffer) {
	SAFE_snprintf(&buffer,&size,"%s",PCOR(pcop)->r->name);
	return buffer;
      }
      return PCOR(pcop)->r->name;
      break;
    case PO_GPR_TEMP:
      r = pic16_regWithIdx(PCOR(pcop)->r->rIdx);

      if(use_buffer) {
	SAFE_snprintf(&buffer,&size,"%s",r->name);
	return buffer;
      }

      return r->name;


    case PO_IMMEDIATE:
      s = buffer;

      if(PCOI(pcop)->_const) {

	if( PCOI(pcop)->offset && PCOI(pcop)->offset<4) {
	  SAFE_snprintf(&s,&size,"(((%s+%d) >> %d)&0xff)",
			pcop->name,
			PCOI(pcop)->index,
			8 * PCOI(pcop)->offset );
	} else
	  SAFE_snprintf(&s,&size,"LOW(%s+%d)",pcop->name,PCOI(pcop)->index);
      } else {
      
	if( PCOI(pcop)->index) { // && PCOI(pcc->pcop)->offset<4) {
	  SAFE_snprintf(&s,&size,"(%s + %d)",
			pcop->name,
			PCOI(pcop)->index );
	} else {
	  if(PCOI(pcop)->offset)
	    SAFE_snprintf(&s,&size,"(%s >> %d)&0xff",pcop->name, 8*PCOI(pcop)->offset);
	  else
	    SAFE_snprintf(&s,&size,"%s",pcop->name);
	}
      }

      return buffer;

    case PO_DIR:
      s = buffer;
      //size = sizeof(buffer);
      if( PCOR(pcop)->instance) {
		SAFE_snprintf(&s,&size,"(%s + %d)",
			pcop->name,
			PCOR(pcop)->instance );
	}
	//fprintf(stderr,"PO_DIR %s\n",buffer);
       else
	SAFE_snprintf(&s,&size,"%s",pcop->name);
      return buffer;

    default:
      if  (pcop->name) {
	if(use_buffer) {
	  SAFE_snprintf(&buffer,&size,"%s",pcop->name);
	  return buffer;
	}
	return pcop->name;
      }

    }
  }

  return "NO operand";

}

/*-----------------------------------------------------------------*/
/* pic16_get_op2 - variant to support two memory operand commands  */
/*-----------------------------------------------------------------*/
char *pic16_get_op2(pCodeOp *pcop,char *buffer, size_t size)
{
  regs *r;
  static char b[50];
  char *s;
  int use_buffer = 1;    // copy the string to the passed buffer pointer

  if(!buffer) {
    buffer = b;
    size = sizeof(b);
    use_buffer = 0;     // Don't bother copying the string to the buffer.
  } 

#if 0
	fprintf(stderr, "%s:%d second operand %s is %d\tPO_DIR(%d) PO_GPR_TEMP(%d) PO_IMMEDIATE(%d) PO_INDF0(%d) PO_FSR0(%d)\n",
		__FUNCTION__, __LINE__, PCOR(PCOR2(pcop)->pcop2)->r->name, PCOR2(pcop)->pcop2->type,
		PO_DIR, PO_GPR_TEMP, PO_IMMEDIATE, PO_INDF0, PO_FSR0);
#endif

  if(pcop) {
    switch(PCOR2(pcop)->pcop2->type) {
    case PO_INDF0:
    case PO_FSR0:
      if(use_buffer) {
	SAFE_snprintf(&buffer,&size,"%s",PCOR(PCOR2(pcop)->pcop2)->r->name);
	return buffer;
      }
      return PCOR(PCOR2(pcop)->pcop2)->r->name;
      break;
    case PO_GPR_TEMP:
      r = pic16_regWithIdx(PCOR(PCOR2(pcop)->pcop2)->r->rIdx);

      if(use_buffer) {
	SAFE_snprintf(&buffer,&size,"%s",r->name);
	return buffer;
      }

      return r->name;


    case PO_IMMEDIATE:
	break;
#if 0
      s = buffer;

      if(PCOI(pcop)->_const) {

	if( PCOI(pcop)->offset && PCOI(pcop)->offset<4) {
	  SAFE_snprintf(&s,&size,"(((%s+%d) >> %d)&0xff)",
			pcop->name,
			PCOI(pcop)->index,
			8 * PCOI(pcop)->offset );
	} else
	  SAFE_snprintf(&s,&size,"LOW(%s+%d)",pcop->name,PCOI(pcop)->index);
      } else {
      
	if( PCOI(pcop)->index) { // && PCOI(pcc->pcop)->offset<4) {
	  SAFE_snprintf(&s,&size,"(%s + %d)",
			pcop->name,
			PCOI(pcop)->index );
	} else {
	  if(PCOI(pcop)->offset)
	    SAFE_snprintf(&s,&size,"(%s >> %d)&0xff",pcop->name, 8*PCOI(pcop)->offset);
	  else
	    SAFE_snprintf(&s,&size,"%s",pcop->name);
	}
      }

      return buffer;
#endif

    case PO_DIR:
      s = buffer;
      //size = sizeof(buffer);
      if( PCOR(PCOR2(pcop)->pcop2)->instance) {
		SAFE_snprintf(&s,&size,"(%s + %d)",
			PCOR(PCOR2(pcop)->pcop2)->r->name,
			PCOR(PCOR2(pcop)->pcop2)->instance );
	}
	//fprintf(stderr,"PO_DIR %s\n",buffer);
       else
	SAFE_snprintf(&s,&size,"%s",PCOR(PCOR2(pcop)->pcop2)->r->name);
      return buffer;

    default:
#if 0
      if  (PCOR2(pcop)->r1->name) {
	if(use_buffer) {
	  SAFE_snprintf(&buffer,&size,"%s",PCOR2(pcop)->r1->name);
	  return buffer;
	}
	return PCOR2(pcop)->r1->name;
      }
#else
      break;
#endif
    }
  }

  return "NO operand";

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static char *pic16_get_op_from_instruction( pCodeInstruction *pcc)
{

  if(pcc )
    return pic16_get_op(pcc->pcop,NULL,0);

  /* gcc 3.2:  warning: concatenation of string literals with __FUNCTION__ is deprecated 
   *   return ("ERROR Null: "__FUNCTION__);
   */
  return ("ERROR Null: pic16_get_op_from_instruction");

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void pCodeOpPrint(FILE *of, pCodeOp *pcop)
{

  fprintf(of,"pcodeopprint- not implemented\n");
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static char *pCode2str(char *str, size_t size, pCode *pc)
{
  char *s = str;

  switch(pc->type) {

  case PC_OPCODE:
    SAFE_snprintf(&s,&size, "\t%s\t", PCI(pc)->mnemonic);

    if( (PCI(pc)->num_ops >= 1) && (PCI(pc)->pcop)) {

	if(PCI(pc)->is2MemOp) {
		SAFE_snprintf(&s,&size, "%s,%s", 
		pic16_get_op(PCOP(PCI(pc)->pcop), NULL, 0),
		pic16_get_op2(PCOP(PCI(pc)->pcop), NULL, 0));
		break;
	}

      if(PCI(pc)->isBitInst) {
	if(PCI(pc)->pcop->type == PO_GPR_BIT) {
	  if( (((pCodeOpRegBit *)(PCI(pc)->pcop))->inBitSpace) )
	    SAFE_snprintf(&s,&size,"(%s >> 3), (%s & 7)", 
			  PCI(pc)->pcop->name ,
			  PCI(pc)->pcop->name );
	  else
	    SAFE_snprintf(&s,&size,"%s,%d", pic16_get_op_from_instruction(PCI(pc)), 
			  (((pCodeOpRegBit *)(PCI(pc)->pcop))->bit ));
	} else if(PCI(pc)->pcop->type == PO_GPR_BIT) {
	  SAFE_snprintf(&s,&size,"%s,%d", pic16_get_op_from_instruction(PCI(pc)),PCORB(PCI(pc)->pcop)->bit);
	}else
	  SAFE_snprintf(&s,&size,"%s,0 ; ?bug", pic16_get_op_from_instruction(PCI(pc)));
	//PCI(pc)->pcop->t.bit );
      } else {

	if(PCI(pc)->pcop->type == PO_GPR_BIT) {
	  if( PCI(pc)->num_ops == 3)
	    SAFE_snprintf(&s,&size,"(%s >> 3),%c",pic16_get_op_from_instruction(PCI(pc)),((PCI(pc)->isModReg) ? 'F':'W'));
	  else
	    SAFE_snprintf(&s,&size,"(1 << (%s & 7))",pic16_get_op_from_instruction(PCI(pc)));

	}else {
	  SAFE_snprintf(&s,&size,"%s",pic16_get_op_from_instruction(PCI(pc)));

	  if( PCI(pc)->num_ops == 3)
	    SAFE_snprintf(&s,&size,",%c", ( (PCI(pc)->isModReg) ? 'F':'W'));
	}
      }

    }
    break;

  case PC_COMMENT:
    /* assuming that comment ends with a \n */
    SAFE_snprintf(&s,&size,";%s", ((pCodeComment *)pc)->comment);
    break;

  case PC_INLINE:
    /* assuming that inline code ends with a \n */
    SAFE_snprintf(&s,&size,"%s", ((pCodeComment *)pc)->comment);
    break;

  case PC_LABEL:
    SAFE_snprintf(&s,&size,";label=%s, key=%d\n",PCL(pc)->label,PCL(pc)->key);
    break;
  case PC_FUNCTION:
    SAFE_snprintf(&s,&size,";modname=%s,function=%s: id=%d\n",PCF(pc)->modname,PCF(pc)->fname);
    break;
  case PC_WILD:
    SAFE_snprintf(&s,&size,";\tWild opcode: id=%d\n",PCW(pc)->id);
    break;
  case PC_FLOW:
    SAFE_snprintf(&s,&size,";\t--FLOW change\n");
    break;
  case PC_CSOURCE:
    SAFE_snprintf(&s,&size,";#CSRC\t%s %d\n; %s\n", PCCS(pc)->file_name, PCCS(pc)->line_number, PCCS(pc)->line);
    break;
  case PC_ASMDIR:
  	SAFE_snprintf(&s,&size,"\t%s\t%s\n", PCAD(pc)->directive, PCAD(pc)->arg?PCAD(pc)->arg:"");
  	break;

  case PC_BAD:
    SAFE_snprintf(&s,&size,";A bad pCode is being used\n");
  }

  return str;

}

/*-----------------------------------------------------------------*/
/* genericPrint - the contents of a pCode to a file                */
/*-----------------------------------------------------------------*/
static void genericPrint(FILE *of, pCode *pc)
{

  if(!pc || !of)
    return;

  switch(pc->type) {
  case PC_COMMENT:
    fprintf(of,";%s\n", ((pCodeComment *)pc)->comment);
    break;

  case PC_INLINE:
    fprintf(of,"%s\n", ((pCodeComment *)pc)->comment);
     break;

  case PC_OPCODE:
    // If the opcode has a label, print that first
    {
      pBranch *pbl = PCI(pc)->label;
      while(pbl && pbl->pc) {
	if(pbl->pc->type == PC_LABEL)
	  pCodePrintLabel(of, pbl->pc);
	pbl = pbl->next;
      }
    }

    if(PCI(pc)->cline) 
      genericPrint(of,PCODE(PCI(pc)->cline));

    {
      char str[256];
      
      pCode2str(str, 256, pc);

      fprintf(of,"%s",str);

      /* Debug */
      if(pic16_debug_verbose) {
	fprintf(of, "\t;key=%03x %d",pc->seq, __LINE__);
	if(PCI(pc)->pcflow)
	  fprintf(of,",flow seq=%03x",PCI(pc)->pcflow->pc.seq);
      }
    }
    fprintf(of, "\n");
    break;
      
#if 0
    {
      pBranch *dpb = pc->to;   // debug
      while(dpb) {
	switch ( dpb->pc->type) {
	case PC_OPCODE:
	  fprintf(of, "\t;%s", PCI(dpb->pc)->mnemonic);
	  break;
	case PC_LABEL:
	  fprintf(of, "\t;label %d", PCL(dpb->pc)->key);
	  break;
	case PC_FUNCTION:
	  fprintf(of, "\t;function %s", ( (PCF(dpb->pc)->fname) ? (PCF(dpb->pc)->fname) : "[END]"));
	  break;
	case PC_FLOW:
	  fprintf(of, "\t;flow");
	  break;
	case PC_COMMENT:
	case PC_WILD:
	  break;
	}
	dpb = dpb->next;
      }
    }
#endif
//    fprintf(of,"\n");		// these are moved prior to #if 0
//    break;

  case PC_WILD:
    fprintf(of,";\tWild opcode: id=%d\n",PCW(pc)->id);
    if(PCW(pc)->pci.label)
      pCodePrintLabel(of, PCW(pc)->pci.label->pc);

    if(PCW(pc)->operand) {
      fprintf(of,";\toperand  ");
      pCodeOpPrint(of,PCW(pc)->operand );
    }
    break;

  case PC_FLOW:
    if(pic16_debug_verbose) {
      fprintf(of,";<>Start of new flow, seq=0x%x",pc->seq);
      if(PCFL(pc)->ancestor)
	fprintf(of," ancestor = 0x%x", PCODE(PCFL(pc)->ancestor)->seq);
      fprintf(of,"\n");

    }
    break;

  case PC_CSOURCE:
    fprintf(of,";#CSRC\t%s %d\n;  %s\n", PCCS(pc)->file_name, PCCS(pc)->line_number, PCCS(pc)->line);
    break;
  case PC_ASMDIR:
  	fprintf(of, "\t%s\t%s\n", PCAD(pc)->directive, PCAD(pc)->arg?PCAD(pc)->arg:"");
  	break;
  	
  case PC_LABEL:
  default:
    fprintf(of,"unknown pCode type %d\n",pc->type);
  }

}

/*-----------------------------------------------------------------*/
/* pCodePrintFunction - prints function begin/end                  */
/*-----------------------------------------------------------------*/

static void pCodePrintFunction(FILE *of, pCode *pc)
{

  if(!pc || !of)
    return;

  if( ((pCodeFunction *)pc)->modname) 
    fprintf(of,"F_%s",((pCodeFunction *)pc)->modname);

  if(PCF(pc)->fname) {
    pBranch *exits = PCF(pc)->to;
    int i=0;
    fprintf(of,"%s\t;Function start\n",PCF(pc)->fname);
    while(exits) {
      i++;
      exits = exits->next;
    }
    //if(i) i--;
    fprintf(of,"; %d exit point%c\n",i, ((i==1) ? ' ':'s'));
    
  }else {
    if((PCF(pc)->from && 
	PCF(pc)->from->pc->type == PC_FUNCTION &&
	PCF(PCF(pc)->from->pc)->fname) )
      fprintf(of,"; exit point of %s\n",PCF(PCF(pc)->from->pc)->fname);
    else
      fprintf(of,"; exit point [can't find entry point]\n");
  }
}
/*-----------------------------------------------------------------*/
/* pCodePrintLabel - prints label                                  */
/*-----------------------------------------------------------------*/

static void pCodePrintLabel(FILE *of, pCode *pc)
{

  if(!pc || !of)
    return;

  if(PCL(pc)->label) 
    fprintf(of,"%s\n",PCL(pc)->label);
  else if (PCL(pc)->key >=0) 
    fprintf(of,"_%05d_DS_:\n",PCL(pc)->key);
  else
    fprintf(of,";wild card label: id=%d\n",-PCL(pc)->key);

}
/*-----------------------------------------------------------------*/
/* unlinkpCodeFromBranch - Search for a label in a pBranch and     */
/*                         remove it if it is found.               */
/*-----------------------------------------------------------------*/
static void unlinkpCodeFromBranch(pCode *pcl , pCode *pc)
{
  pBranch *b, *bprev;


  bprev = NULL;

  if(pcl->type == PC_OPCODE)
    b = PCI(pcl)->label;
  else {
    fprintf(stderr, "LINE %d. can't unlink from non opcode\n",__LINE__);
    exit(1);

  }

  //fprintf (stderr, "%s \n",__FUNCTION__);
  //pcl->print(stderr,pcl);
  //pc->print(stderr,pc);
  while(b) {
    if(b->pc == pc) {
      //fprintf (stderr, "found label\n");

      /* Found a label */
      if(bprev) {
	bprev->next = b->next;  /* Not first pCode in chain */
	free(b);
      } else {
	pc->destruct(pc);
	PCI(pcl)->label = b->next;   /* First pCode in chain */
	free(b);
      }
      return;  /* A label can't occur more than once */
    }
    bprev = b;
    b = b->next;
  }

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
pBranch * pic16_pBranchAppend(pBranch *h, pBranch *n)
{
  pBranch *b;

  if(!h)
    return n;

  if(h == n)
    return n;

  b = h;
  while(b->next)
    b = b->next;

  b->next = n;

  return h;
  
}  
/*-----------------------------------------------------------------*/
/* pBranchLink - given two pcodes, this function will link them    */
/*               together through their pBranches                  */
/*-----------------------------------------------------------------*/
static void pBranchLink(pCodeFunction *f, pCodeFunction *t)
{
  pBranch *b;

  // Declare a new branch object for the 'from' pCode.

  //_ALLOC(b,sizeof(pBranch));
  b = Safe_calloc(1,sizeof(pBranch));
  b->pc = PCODE(t);             // The link to the 'to' pCode.
  b->next = NULL;

  f->to = pic16_pBranchAppend(f->to,b);

  // Now do the same for the 'to' pCode.

  //_ALLOC(b,sizeof(pBranch));
  b = Safe_calloc(1,sizeof(pBranch));
  b->pc = PCODE(f);
  b->next = NULL;

  t->from = pic16_pBranchAppend(t->from,b);
  
}

#if 0
/*-----------------------------------------------------------------*/
/* pBranchFind - find the pBranch in a pBranch chain that contains */
/*               a pCode                                           */
/*-----------------------------------------------------------------*/
static pBranch *pBranchFind(pBranch *pb,pCode *pc)
{
  while(pb) {

    if(pb->pc == pc)
      return pb;

    pb = pb->next;
  }

  return NULL;
}

/*-----------------------------------------------------------------*/
/* pCodeUnlink - Unlink the given pCode from its pCode chain.      */
/*-----------------------------------------------------------------*/
static void pCodeUnlink(pCode *pc)
{
  pBranch *pb1,*pb2;
  pCode *pc1;

  if(!pc->prev || !pc->next) {
    fprintf(stderr,"unlinking bad pCode in %s:%d\n",__FILE__,__LINE__);
    exit(1);
  }

  /* first remove the pCode from the chain */
  pc->prev->next = pc->next;
  pc->next->prev = pc->prev;

  /* Now for the hard part... */

  /* Remove the branches */

  pb1 = pc->from;
  while(pb1) {
    pc1 = pb1->pc;    /* Get the pCode that branches to the
		       * one we're unlinking */

    /* search for the link back to this pCode (the one we're
     * unlinking) */
    if(pb2 = pBranchFind(pc1->to,pc)) {
      pb2->pc = pc->to->pc;  // make the replacement

      /* if the pCode we're unlinking contains multiple 'to'
       * branches (e.g. this a skip instruction) then we need
       * to copy these extra branches to the chain. */
      if(pc->to->next)
	pic16_pBranchAppend(pb2, pc->to->next);
    }
    
    pb1 = pb1->next;
  }


}
#endif
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
#if 0
static void genericAnalyze(pCode *pc)
{
  switch(pc->type) {
  case PC_WILD:
  case PC_COMMENT:
    return;
  case PC_LABEL:
  case PC_FUNCTION:
  case PC_OPCODE:
    {
      // Go through the pCodes that are in pCode chain and link
      // them together through the pBranches. Note, the pCodes
      // are linked together as a contiguous stream like the 
      // assembly source code lines. The linking here mimics this
      // except that comments are not linked in.
      // 
      pCode *npc = pc->next;
      while(npc) {
	if(npc->type == PC_OPCODE || npc->type == PC_LABEL) {
	  pBranchLink(pc,npc);
	  return;
	} else
	  npc = npc->next;
      }
      /* reached the end of the pcode chain without finding
       * an instruction we could link to. */
    }
    break;
  case PC_FLOW:
    fprintf(stderr,"analyze PC_FLOW\n");

    return;
  case PC_BAD:
    fprintf(stderr,,";A bad pCode is being used\n");

  }
}
#endif

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static int compareLabel(pCode *pc, pCodeOpLabel *pcop_label)
{
  pBranch *pbr;

  if(pc->type == PC_LABEL) {
    if( ((pCodeLabel *)pc)->key ==  pcop_label->key)
      return TRUE;
  }
  if(pc->type == PC_OPCODE) {
    pbr = PCI(pc)->label;
    while(pbr) {
      if(pbr->pc->type == PC_LABEL) {
	if( ((pCodeLabel *)(pbr->pc))->key ==  pcop_label->key)
	  return TRUE;
      }
      pbr = pbr->next;
    }
  }

  return FALSE;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static int checkLabel(pCode *pc)
{
  pBranch *pbr;

  if(pc && isPCI(pc)) {
    pbr = PCI(pc)->label;
    while(pbr) {
      if(isPCL(pbr->pc) && (PCL(pbr->pc)->key >= 0))
	return TRUE;

      pbr = pbr->next;
    }
  }

  return FALSE;
}

/*-----------------------------------------------------------------*/
/* findLabelinpBlock - Search the pCode for a particular label     */
/*-----------------------------------------------------------------*/
static pCode * findLabelinpBlock(pBlock *pb,pCodeOpLabel *pcop_label)
{
  pCode  *pc;

  if(!pb)
    return NULL;

  for(pc = pb->pcHead; pc; pc = pc->next) 
    if(compareLabel(pc,pcop_label))
      return pc;
    
  return NULL;
}
#if 0
/*-----------------------------------------------------------------*/
/* findLabel - Search the pCode for a particular label             */
/*-----------------------------------------------------------------*/
static pCode * findLabel(pCodeOpLabel *pcop_label)
{
  pBlock *pb;
  pCode  *pc;

  if(!the_pFile)
    return NULL;

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if( (pc = findLabelinpBlock(pb,pcop_label)) != NULL)
      return pc;
  }

  fprintf(stderr,"Couldn't find label %s", pcop_label->pcop.name);
  return NULL;
}
#endif
/*-----------------------------------------------------------------*/
/* pic16_findNextpCode - given a pCode, find the next of type 'pct'      */
/*                 in the linked list                              */
/*-----------------------------------------------------------------*/
pCode * pic16_findNextpCode(pCode *pc, PC_TYPE pct)
{

  while(pc) {
    if(pc->type == pct)
      return pc;

    pc = pc->next;
  }

  return NULL;
}

/*-----------------------------------------------------------------*/
/* findPrevpCode - given a pCode, find the previous of type 'pct'  */
/*                 in the linked list                              */
/*-----------------------------------------------------------------*/
static pCode * findPrevpCode(pCode *pc, PC_TYPE pct)
{

  while(pc) {
    if(pc->type == pct)
      return pc;

    pc = pc->prev;
  }

  return NULL;
}
/*-----------------------------------------------------------------*/
/* pic16_findNextInstruction - given a pCode, find the next instruction  */
/*                       in the linked list                        */
/*-----------------------------------------------------------------*/
pCode * pic16_findNextInstruction(pCode *pci)
{
  pCode *pc = pci;

  while(pc) {
    if((pc->type == PC_OPCODE) || (pc->type == PC_WILD))
      return pc;

#ifdef PCODE_DEBUG
    fprintf(stderr,"pic16_findNextInstruction:  ");
    printpCode(stderr, pc);
#endif
    pc = pc->next;
  }

  //fprintf(stderr,"Couldn't find instruction\n");
  return NULL;
}

/*-----------------------------------------------------------------*/
/* pic16_findNextInstruction - given a pCode, find the next instruction  */
/*                       in the linked list                        */
/*-----------------------------------------------------------------*/
pCode * pic16_findPrevInstruction(pCode *pci)
{
  return findPrevpCode(pci, PC_OPCODE);
}
#if 0
/*-----------------------------------------------------------------*/
/* findFunctionEnd - given a pCode find the end of the function    */
/*                   that contains it                              */
/*-----------------------------------------------------------------*/
static pCode * findFunctionEnd(pCode *pc)
{

  while(pc) {
    if(pc->type == PC_FUNCTION &&  !(PCF(pc)->fname))
      return pc;

    pc = pc->next;
  }

  fprintf(stderr,"Couldn't find function end\n");
  return NULL;
}
#endif
#if 0
/*-----------------------------------------------------------------*/
/* AnalyzeLabel - if the pCode is a label, then merge it with the  */
/*                instruction with which it is associated.         */
/*-----------------------------------------------------------------*/
static void AnalyzeLabel(pCode *pc)
{

  pCodeUnlink(pc);

}
#endif

#if 0
static void AnalyzeGOTO(pCode *pc)
{

  pBranchLink(pc,findLabel( (pCodeOpLabel *) (PCI(pc)->pcop) ));

}

static void AnalyzeSKIP(pCode *pc)
{

  pBranchLink(pc,pic16_findNextInstruction(pc->next));
  pBranchLink(pc,pic16_findNextInstruction(pc->next->next));

}

static void AnalyzeRETURN(pCode *pc)
{

  //  branch_link(pc,findFunctionEnd(pc->next));

}

#endif

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
regs * pic16_getRegFromInstruction(pCode *pc)
{

  if(!pc                   || 
     !isPCI(pc)            ||
     !PCI(pc)->pcop        ||
     PCI(pc)->num_ops == 0 ||
     (PCI(pc)->num_ops == 1 && PCI(pc)->isFastCall))
    return NULL;

  switch(PCI(pc)->pcop->type) {
  case PO_INDF0:
  case PO_FSR0:
    return PCOR(PCI(pc)->pcop)->r;

    //    return typeRegWithIdx (PCOR(PCI(pc)->pcop)->rIdx, REG_SFR, 0);

  case PO_BIT:
  case PO_GPR_TEMP:
    //fprintf(stderr, "pic16_getRegFromInstruction - bit or temp\n");
    return PCOR(PCI(pc)->pcop)->r;

  case PO_IMMEDIATE:
    if(PCOI(PCI(pc)->pcop)->r)
      return (PCOI(PCI(pc)->pcop)->r);

    //fprintf(stderr, "pic16_getRegFromInstruction - immediate\n");
    return pic16_dirregWithName(PCI(pc)->pcop->name);
    //return NULL; // PCOR(PCI(pc)->pcop)->r;

  case PO_GPR_BIT:
    return PCOR(PCI(pc)->pcop)->r;

  case PO_DIR:
    //fprintf(stderr, "pic16_getRegFromInstruction - dir\n");
    return PCOR(PCI(pc)->pcop)->r;
  case PO_LITERAL:
    //fprintf(stderr, "pic16_getRegFromInstruction - literal\n");
    break;

  default:
    //fprintf(stderr, "pic16_getRegFromInstruction - unknown reg type %d\n",PCI(pc)->pcop->type);
    //genericPrint(stderr, pc);
    break;
  }

  return NULL;

}

/*-------------------------------------------------------------------------------*/
/* pic16_getRegFromInstruction2 - variant to support two memory operand commands */
/*-------------------------------------------------------------------------------*/
regs * pic16_getRegFromInstruction2(pCode *pc)
{

  if(!pc                   || 
     !isPCI(pc)            ||
     !PCI(pc)->pcop        ||
     PCI(pc)->num_ops == 0 ||
     (PCI(pc)->num_ops == 1))		// accept only 2 operand commands
    return NULL;


/*
 * operands supported in MOVFF:
 *  PO_INF0/PO_FSR0
 *  PO_GPR_TEMP
 *  PO_IMMEDIATE
 *  PO_DIR
 *
 */
  switch(PCI(pc)->pcop->type) {
  case PO_INDF0:
  case PO_FSR0:
    return PCOR(PCOR2(PCI(pc)->pcop)->pcop2)->r;

    //    return typeRegWithIdx (PCOR(PCI(pc)->pcop)->rIdx, REG_SFR, 0);

//  case PO_BIT:
  case PO_GPR_TEMP:
    //fprintf(stderr, "pic16_getRegFromInstruction - bit or temp\n");
    return PCOR(PCOR2(PCI(pc)->pcop)->pcop2)->r;

  case PO_IMMEDIATE:
	break;
#if 0
    if(PCOI(PCI(pc)->pcop)->r)
      return (PCOI(PCI(pc)->pcop)->r);

    //fprintf(stderr, "pic16_getRegFromInstruction - immediate\n");
    return pic16_dirregWithName(PCI(pc)->pcop->name);
    //return NULL; // PCOR(PCI(pc)->pcop)->r;
#endif

  case PO_GPR_BIT:
	break;
//    return PCOR2(PCI(pc)->pcop)->r;

  case PO_DIR:
    //fprintf(stderr, "pic16_getRegFromInstruction - dir\n");
    return PCOR(PCOR2(PCI(pc)->pcop)->pcop2)->r;

  case PO_LITERAL:
	break;
    //fprintf(stderr, "pic16_getRegFromInstruction - literal\n");

  default:
    //fprintf(stderr, "pic16_getRegFromInstruction - unknown reg type %d\n",PCI(pc)->pcop->type);
    //genericPrint(stderr, pc);
    break;
  }

  return NULL;

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

static void AnalyzepBlock(pBlock *pb)
{
  pCode *pc;

  if(!pb)
    return;

  /* Find all of the registers used in this pBlock 
   * by looking at each instruction and examining it's
   * operands
   */
  for(pc = pb->pcHead; pc; pc = pc->next) {

    /* Is this an instruction with operands? */
    if(pc->type == PC_OPCODE && PCI(pc)->pcop) {

      if(PCI(pc)->pcop->type == PO_GPR_TEMP) {

	/* Loop through all of the registers declared so far in
	   this block and see if we find this one there */

	regs *r = setFirstItem(pb->tregisters);

	while(r) {
	  if(r->rIdx == PCOR(PCI(pc)->pcop)->r->rIdx) {
	    PCOR(PCI(pc)->pcop)->r = r;
	    break;
	  }
	  r = setNextItem(pb->tregisters);
	}

	if(!r) {
	  /* register wasn't found */
	  //r = Safe_calloc(1, sizeof(regs));
	  //memcpy(r,PCOR(PCI(pc)->pcop)->r, sizeof(regs));
	  //addSet(&pb->tregisters, r);
	  addSet(&pb->tregisters, PCOR(PCI(pc)->pcop)->r);
	  //PCOR(PCI(pc)->pcop)->r = r;
	  //fprintf(stderr,"added register to pblock: reg %d\n",r->rIdx);
	}/* else 
	  fprintf(stderr,"found register in pblock: reg %d\n",r->rIdx);
	 */
      }
      if(PCI(pc)->pcop->type == PO_GPR_REGISTER) {
	if(PCOR(PCI(pc)->pcop)->r) {
	  pic16_allocWithIdx (PCOR(PCI(pc)->pcop)->r->rIdx);
	  DFPRINTF((stderr,"found register in pblock: reg 0x%x\n",PCOR(PCI(pc)->pcop)->r->rIdx));
	} else {
	  if(PCI(pc)->pcop->name)
	    fprintf(stderr,"ERROR: %s is a NULL register\n",PCI(pc)->pcop->name );
	  else
	    fprintf(stderr,"ERROR: NULL register\n");
	}
      }
    }


  }
}

/*-----------------------------------------------------------------*/
/* */
/*-----------------------------------------------------------------*/
#define PCI_HAS_LABEL(x) ((x) && (PCI(x)->label != NULL))

static void InsertpFlow(pCode *pc, pCode **pflow)
{
  if(*pflow)
    PCFL(*pflow)->end = pc;

  if(!pc || !pc->next)
    return;

  *pflow = pic16_newpCodeFlow();
  pic16_pCodeInsertAfter(pc, *pflow);
}

/*-----------------------------------------------------------------*/
/* pic16_BuildFlow(pBlock *pb) - examine the code in a pBlock and build  */
/*                         the flow blocks.                        */
/*
 * pic16_BuildFlow inserts pCodeFlow objects into the pCode chain at each
 * point the instruction flow changes. 
 */
/*-----------------------------------------------------------------*/
void pic16_BuildFlow(pBlock *pb)
{
  pCode *pc;
  pCode *last_pci=NULL;
  pCode *pflow=NULL;
  int seq = 0;

  if(!pb)
    return;

  //fprintf (stderr,"build flow start seq %d  ",GpcFlowSeq);
  /* Insert a pCodeFlow object at the beginning of a pBlock */

  InsertpFlow(pb->pcHead, &pflow);

  //pflow = pic16_newpCodeFlow();    /* Create a new Flow object */
  //pflow->next = pb->pcHead;  /* Make the current head the next object */
  //pb->pcHead->prev = pflow;  /* let the current head point back to the flow object */
  //pb->pcHead = pflow;        /* Make the Flow object the head */
  //pflow->pb = pb;

  for( pc = pic16_findNextInstruction(pb->pcHead);
       pc != NULL;
       pc=pic16_findNextInstruction(pc)) { 

    pc->seq = seq++;
    PCI(pc)->pcflow = PCFL(pflow);

    //fprintf(stderr," build: ");
    //pflow->print(stderr,pflow);

    if( PCI(pc)->isSkip) {

      /* The two instructions immediately following this one 
       * mark the beginning of a new flow segment */

      while(pc && PCI(pc)->isSkip) {

	PCI(pc)->pcflow = PCFL(pflow);
	pc->seq = seq-1;
	seq = 1;

	InsertpFlow(pc, &pflow);
	pc=pic16_findNextInstruction(pc->next);
      }

      seq = 0;

      if(!pc)
	break;

      PCI(pc)->pcflow = PCFL(pflow);
      pc->seq = 0;
      InsertpFlow(pc, &pflow);

    } else if ( PCI(pc)->isBranch && !checkLabel(pic16_findNextInstruction(pc->next)))  {

      InsertpFlow(pc, &pflow);
      seq = 0;

    } else if (checkLabel(pc)) { 

      /* This instruction marks the beginning of a
       * new flow segment */

      pc->seq = 0;
      seq = 1;

      /* If the previous pCode is not a flow object, then 
       * insert a new flow object. (This check prevents 
       * two consecutive flow objects from being insert in
       * the case where a skip instruction preceeds an
       * instruction containing a label.) */

      if(last_pci && (PCI(last_pci)->pcflow == PCFL(pflow)))
	InsertpFlow(pic16_findPrevInstruction(pc->prev), &pflow);

      PCI(pc)->pcflow = PCFL(pflow);
      
    }
    last_pci = pc;
    pc = pc->next;
  }

  //fprintf (stderr,",end seq %d",GpcFlowSeq);
  if(pflow)
    PCFL(pflow)->end = pb->pcTail;
}

/*-------------------------------------------------------------------*/
/* unBuildFlow(pBlock *pb) - examine the code in a pBlock and build  */
/*                           the flow blocks.                        */
/*
 * unBuildFlow removes pCodeFlow objects from a pCode chain
 */
/*-----------------------------------------------------------------*/
static void unBuildFlow(pBlock *pb)
{
  pCode *pc,*pcnext;

  if(!pb)
    return;

  pc = pb->pcHead;

  while(pc) {
    pcnext = pc->next;

    if(isPCI(pc)) {

      pc->seq = 0;
      if(PCI(pc)->pcflow) {
	//free(PCI(pc)->pcflow);
	PCI(pc)->pcflow = NULL;
      }

    } else if(isPCFL(pc) )
      pc->destruct(pc);

    pc = pcnext;
  }


}
#if 0
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void dumpCond(int cond)
{

  static char *pcc_str[] = {
    //"PCC_NONE",
    "PCC_REGISTER",
    "PCC_REGISTER2",
    "PCC_C",
    "PCC_Z",
    "PCC_DC",
    "PCC_OV",
    "PCC_N",
    "PCC_W",
    "PCC_EXAMINE_PCOP",
    "PCC_LITERAL",
    "PCC_REL_ADDR"
  };

  int ncond = sizeof(pcc_str) / sizeof(char *);
  int i,j;

  fprintf(stderr, "0x%04X\n",cond);

  for(i=0,j=1; i<ncond; i++, j<<=1)
    if(cond & j)
      fprintf(stderr, "  %s\n",pcc_str[i]);

}
#endif

#if 0
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void FlowStats(pCodeFlow *pcflow)
{

  pCode *pc;

  if(!isPCFL(pcflow))
    return;

  fprintf(stderr, " FlowStats - flow block (seq=%d)\n", pcflow->pc.seq);

  pc = pic16_findNextpCode(PCODE(pcflow), PC_OPCODE); 

  if(!pc) {
    fprintf(stderr, " FlowStats - empty flow (seq=%d)\n", pcflow->pc.seq);
    return;
  }


  fprintf(stderr, "  FlowStats inCond: ");
  dumpCond(pcflow->inCond);
  fprintf(stderr, "  FlowStats outCond: ");
  dumpCond(pcflow->outCond);

}
#endif
/*-----------------------------------------------------------------*
 * int isBankInstruction(pCode *pc) - examine the pCode *pc to determine
 *    if it affects the banking bits. 
 * 
 * return: -1 == Banking bits are unaffected by this pCode.
 *
 * return: > 0 == Banking bits are affected.
 *
 *  If the banking bits are affected, then the returned value describes
 * which bits are affected and how they're affected. The lower half
 * of the integer maps to the bits that are affected, the upper half
 * to whether they're set or cleared.
 *
 *-----------------------------------------------------------------*/

static int isBankInstruction(pCode *pc)
{
  regs *reg;
  int bank = -1;

  if(!isPCI(pc))
    return -1;

  if( PCI(pc)->op == POC_MOVLB ||
      (( (reg = pic16_getRegFromInstruction(pc)) != NULL) && isBSR_REG(reg))) {
    bank = PCOL(pc)->lit;
  }

  return bank;
}


/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void FillFlow(pCodeFlow *pcflow)
{

  pCode *pc;
  int cur_bank;

  if(!isPCFL(pcflow))
    return;

  //  fprintf(stderr, " FillFlow - flow block (seq=%d)\n", pcflow->pc.seq);

  pc = pic16_findNextpCode(PCODE(pcflow), PC_OPCODE); 

  if(!pc) {
    //fprintf(stderr, " FillFlow - empty flow (seq=%d)\n", pcflow->pc.seq);
    return;
  }

  cur_bank = -1;

  do {
    isBankInstruction(pc);
    pc = pc->next;
  } while (pc && (pc != pcflow->end) && !isPCFL(pc));

/*
  if(!pc ) {
    fprintf(stderr, "  FillFlow - Bad end of flow\n");
  } else {
    fprintf(stderr, "  FillFlow - Ending flow with\n  ");
    pc->print(stderr,pc);
  }

  fprintf(stderr, "  FillFlow inCond: ");
  dumpCond(pcflow->inCond);
  fprintf(stderr, "  FillFlow outCond: ");
  dumpCond(pcflow->outCond);
*/
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void LinkFlow_pCode(pCodeInstruction *from, pCodeInstruction *to)
{
  pCodeFlowLink *fromLink, *toLink;

  if(!from || !to || !to->pcflow || !from->pcflow)
    return;

  fromLink = pic16_newpCodeFlowLink(from->pcflow);
  toLink   = pic16_newpCodeFlowLink(to->pcflow);

  addSetIfnotP(&(from->pcflow->to), toLink);   //to->pcflow);
  addSetIfnotP(&(to->pcflow->from), fromLink); //from->pcflow);

}

/*-----------------------------------------------------------------*
 * void LinkFlow(pBlock *pb)
 *
 * In pic16_BuildFlow, the PIC code has been partitioned into contiguous
 * non-branching segments. In LinkFlow, we determine the execution
 * order of these segments. For example, if one of the segments ends
 * with a skip, then we know that there are two possible flow segments
 * to which control may be passed.
 *-----------------------------------------------------------------*/
static void LinkFlow(pBlock *pb)
{
  pCode *pc=NULL;
  pCode *pcflow;
  pCode *pct;

  //fprintf(stderr,"linkflow \n");

  for( pcflow = pic16_findNextpCode(pb->pcHead, PC_FLOW); 
       pcflow != NULL;
       pcflow = pic16_findNextpCode(pcflow->next, PC_FLOW) ) {

    if(!isPCFL(pcflow))
      fprintf(stderr, "LinkFlow - pcflow is not a flow object ");

    //fprintf(stderr," link: ");
    //pcflow->print(stderr,pcflow);

    //FillFlow(PCFL(pcflow));

    pc = PCFL(pcflow)->end;

    //fprintf(stderr, "LinkFlow - flow block (seq=%d) ", pcflow->seq);
    if(isPCI_SKIP(pc)) {
      //fprintf(stderr, "ends with skip\n");
      //pc->print(stderr,pc);
      pct=pic16_findNextInstruction(pc->next);
      LinkFlow_pCode(PCI(pc),PCI(pct));
      pct=pic16_findNextInstruction(pct->next);
      LinkFlow_pCode(PCI(pc),PCI(pct));
      continue;
    }

    if(isPCI_BRANCH(pc)) {
      pCodeOpLabel *pcol = PCOLAB(PCI(pc)->pcop);

      //fprintf(stderr, "ends with branch\n  ");
      //pc->print(stderr,pc);

      if(!(pcol && isPCOLAB(pcol))) {
	if((PCI(pc)->op != POC_RETLW) && (PCI(pc)->op != POC_RETURN) && (PCI(pc)->op != POC_CALL) && (PCI(pc)->op != POC_RETFIE) ) {
	  pc->print(stderr,pc);
	  fprintf(stderr, "ERROR: %s, branch instruction doesn't have label\n",__FUNCTION__);
	}
	continue;
      }

      if( (pct = findLabelinpBlock(pb,pcol)) != NULL)
	LinkFlow_pCode(PCI(pc),PCI(pct));
      else
	fprintf(stderr, "ERROR: %s, couldn't find label. key=%d,lab=%s\n",
		__FUNCTION__,pcol->key,((PCOP(pcol)->name)?PCOP(pcol)->name:"-"));
  //fprintf(stderr,"pic16_newpCodeOpLabel: key=%d, name=%s\n",key,((s)?s:""));

      continue;
    }

    if(isPCI(pc)) {
      //fprintf(stderr, "ends with non-branching instruction:\n");
      //pc->print(stderr,pc);

      LinkFlow_pCode(PCI(pc),PCI(pic16_findNextInstruction(pc->next)));

      continue;
    }

    if(pc) {
      //fprintf(stderr, "ends with unknown\n");
      //pc->print(stderr,pc);
      continue;
    }

    //fprintf(stderr, "ends with nothing: ERROR\n");
    
  }
}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
int pic16_isPCinFlow(pCode *pc, pCode *pcflow)
{

  if(!pc || !pcflow)
    return 0;

  if(!isPCI(pc) || !PCI(pc)->pcflow || !isPCFL(pcflow) )
    return 0;

  if( PCI(pc)->pcflow->pc.seq == pcflow->seq)
    return 1;

  return 0;
}

/*-----------------------------------------------------------------*/
/* BanksUsedFlow - Identify which banks are used in flow 2.        */
/*-----------------------------------------------------------------*/
static void BanksUsedFlow2(pCode *pcflow)
{
  pCode *pc=NULL;

  int bank = -1;
  bool RegUsed = 0;

  regs *reg;

  if(!isPCFL(pcflow)) {
    fprintf(stderr, "BanksUsed - pcflow is not a flow object ");
    return;
  }

  pc = pic16_findNextInstruction(pcflow->next);

  PCFL(pcflow)->lastBank = -1;

  while(pic16_isPCinFlow(pc,pcflow)) {

    int bank_selected = isBankInstruction(pc);

    //if(PCI(pc)->pcflow) 
    //fprintf(stderr,"BanksUsedFlow2, looking at seq %d\n",PCI(pc)->pcflow->pc.seq);

    if(bank_selected >= 0) {
      //fprintf(stderr,"BanksUsed - mucking with bank %d\n",bank_selected);

      /* This instruction is modifying banking bits before accessing registers */
      if(!RegUsed)
	PCFL(pcflow)->firstBank = -1;

      if(PCFL(pcflow)->lastBank == -1)
	PCFL(pcflow)->lastBank = 0;

      bank = 1 << bank_selected;
      PCFL(pcflow)->lastBank |= bank;
				 

    } else { 
      reg = pic16_getRegFromInstruction(pc);

      if(reg && !pic16_isREGinBank(reg, bank)) {
	int allbanks = pic16_REGallBanks(reg);
	if(bank == -1)
	  PCFL(pcflow)->firstBank = allbanks;

	PCFL(pcflow)->lastBank = allbanks;

	bank = allbanks;
      }
      RegUsed = 1;
    }

    pc = pic16_findNextInstruction(pc->next);
  }

//  fprintf(stderr,"BanksUsedFlow2 flow seq=%3d, first bank = 0x%03x, Last bank 0x%03x\n",
//	  pcflow->seq,PCFL(pcflow)->firstBank,PCFL(pcflow)->lastBank);



}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void BanksUsedFlow(pBlock *pb)
{
  pCode *pcflow;


  //pb->pcHead->print(stderr, pb->pcHead);

  pcflow = pic16_findNextpCode(pb->pcHead, PC_FLOW);
  //pcflow->print(stderr,pcflow);

  for( pcflow = pic16_findNextpCode(pb->pcHead, PC_FLOW); 
       pcflow != NULL;
       pcflow = pic16_findNextpCode(pcflow->next, PC_FLOW) ) {

    BanksUsedFlow2(pcflow);
  }

}


/*-----------------------------------------------------------------*/
/* insertBankSwitch - inserts a bank switch statement in the assembly listing */
/*-----------------------------------------------------------------*/
static void insertBankSwitch(int position, pCode *pc, int bsr)
{
  pCode *new_pc;
  regs *reg;

  if(!pc)
    return;

/*
 * if bsr == -1 then do not insert a MOVLB instruction, but rather
 * insert a BANKSEL assembler directive for the symbol used by
 * the pCode. This will allow the linker to setup the correct
 * bank at linking time
 */

	if(!options.gen_banksel || bsr != -1) {
//		new_pc = pic16_newpCode(POC_MOVLB, pic16_newpCodeOpLit(bsr));
		return;
	} else {
		/* emit the BANKSEL [symbol] */

		/* IMPORTANT: The following code does not check if a symbol is
		 * split in multiple banks. This should be corrected. - VR 6/6/2003 */

		reg = pic16_getRegFromInstruction(pc);
		if(!reg)return;
		new_pc = pic16_newpCodeAsmDir("BANKSEL", "%s", reg->name);
		
		position = 0;		// position is always before (sanity check!)
	}

#if 0
	fprintf(stderr, "%s:%d: inserting bank switch\tbank = %d\n", __FUNCTION__, __LINE__, bsr);
	pc->print(stderr, pc);
#endif

  if(position) {
    /* insert the bank switch after this pc instruction */
    pCode *pcnext = pic16_findNextInstruction(pc);
    pic16_pCodeInsertAfter(pc, new_pc);
    if(pcnext)
      pc = pcnext;

  } else
    pic16_pCodeInsertAfter(pc->prev, new_pc);

  /* Move the label, if there is one */

  if(PCI(pc)->label) {
    PCI(new_pc)->label = PCI(pc)->label;
    PCI(pc)->label = NULL;
  }

  /* The new instruction has the same pcflow block */
  PCI(new_pc)->pcflow = PCI(pc)->pcflow;

}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void FixRegisterBankingInFlow(pCodeFlow *pcfl, int cur_bank)
{
  pCode *pc=NULL;
  pCode *pcprev=NULL;
  pCode *new_pc;

  regs *reg;

  if(!pcfl)
    return;

  pc = pic16_findNextInstruction(pcfl->pc.next);

  while(pic16_isPCinFlow(pc,PCODE(pcfl))) {

    reg = pic16_getRegFromInstruction(pc);

#if 1
    if(reg) {
      fprintf(stderr, "%s:%d  %s  ",__FUNCTION__, __LINE__, reg->name);
      fprintf(stderr, "addr = 0x%03x, bank = %d\n",reg->address,REG_BANK(reg));

    }
#endif

    if( ( (reg && !isACCESS_BANK(reg) && REG_BANK(reg)!=cur_bank) || 
	  ((PCI(pc)->op == POC_CALL) && (cur_bank != 0) ) ) &&
	(!isPCI_LIT(pc)) ){

      /* Examine the instruction before this one to make sure it is
       * not a skip type instruction */
      pcprev = findPrevpCode(pc->prev, PC_OPCODE);

      if(!pcprev || (pcprev && !isPCI_SKIP(pcprev) /* && !isBankInstruction(pcprev)*/)) {
	int reg_bank;

	reg_bank =  (reg) ? REG_BANK(reg) : 0;

	
        if (cur_bank != reg_bank) {
	  //fprintf(stderr, "Cool! can switch banks\n");
	  cur_bank = reg_bank;
	  insertBankSwitch(0, pc, cur_bank);
	}

      } else {
	//fprintf(stderr, "Bummer can't switch banks\n");
	;
      }
    }

    pcprev = pc;
    pc = pic16_findNextInstruction(pc->next);

  }

  if(pcprev && cur_bank) {
    /* Brute force - make sure that we point to bank 0 at the
     * end of each flow block */
    new_pc = pic16_newpCode(POC_MOVLB, pic16_newpCodeOpLit(0));
    pic16_pCodeInsertAfter(pcprev, new_pc);
      cur_bank = 0;
    //fprintf(stderr, "Brute force switch\n");
  }

}

/*-----------------------------------------------------------------*/
/*int compareBankFlow - compare the banking requirements between   */
/*  flow objects. */
/*-----------------------------------------------------------------*/
static int compareBankFlow(pCodeFlow *pcflow, pCodeFlowLink *pcflowLink, int toORfrom)
{

  if(!pcflow || !pcflowLink || !pcflowLink->pcflow)
    return 0;

  if(!isPCFL(pcflow) || !isPCFL(pcflowLink->pcflow))
    return 0;

  if(pcflow->firstBank == -1)
    return 0;


  if(pcflowLink->pcflow->firstBank == -1) {
    pCodeFlowLink *pctl = setFirstItem( toORfrom ? 
					pcflowLink->pcflow->to : 
					pcflowLink->pcflow->from);
    return compareBankFlow(pcflow, pctl, toORfrom);
  }

  if(toORfrom) {
    if(pcflow->lastBank == pcflowLink->pcflow->firstBank)
      return 0;

    pcflowLink->bank_conflict++;
    pcflowLink->pcflow->FromConflicts++;
    pcflow->ToConflicts++;
  } else {
    
    if(pcflow->firstBank == pcflowLink->pcflow->lastBank)
      return 0;

    pcflowLink->bank_conflict++;
    pcflowLink->pcflow->ToConflicts++;
    pcflow->FromConflicts++;

  }
  /*
  fprintf(stderr,"compare flow found conflict: seq %d from conflicts %d, to conflicts %d\n",
	  pcflowLink->pcflow->pc.seq,
	  pcflowLink->pcflow->FromConflicts,
	  pcflowLink->pcflow->ToConflicts);
  */
  return 1;

}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void FixBankFlow(pBlock *pb)
{
  pCode *pc=NULL;
  pCode *pcflow;
  pCodeFlowLink *pcfl;

  pCode *pcflow_max_To=NULL;
  pCode *pcflow_max_From=NULL;
  int max_ToConflicts=0;
  int max_FromConflicts=0;

  //fprintf(stderr,"Fix Bank flow \n");
  pcflow = pic16_findNextpCode(pb->pcHead, PC_FLOW);


  /*
    First loop through all of the flow objects in this pcode block
    and fix the ones that have banking conflicts between the 
    entry and exit.
  */

//	fprintf(stderr, "FixBankFlow - Phase 1\n");

  for( pcflow = pic16_findNextpCode(pb->pcHead, PC_FLOW); 
       pcflow != NULL;
       pcflow = pic16_findNextpCode(pcflow->next, PC_FLOW) ) {

    if(!isPCFL(pcflow)) {
      fprintf(stderr, "FixBankFlow - pcflow is not a flow object ");
      continue;
    }

    if(PCFL(pcflow)->firstBank != PCFL(pcflow)->lastBank  &&
       PCFL(pcflow)->firstBank >= 0 &&
       PCFL(pcflow)->lastBank >= 0 ) {

      int cur_bank = (PCFL(pcflow)->firstBank < PCFL(pcflow)->lastBank) ?
	PCFL(pcflow)->firstBank : PCFL(pcflow)->lastBank;

      FixRegisterBankingInFlow(PCFL(pcflow),cur_bank);
      BanksUsedFlow2(pcflow);

    }
  }

//	fprintf(stderr, "FixBankFlow - Phase 2\n");

  for( pcflow = pic16_findNextpCode(pb->pcHead, PC_FLOW); 
       pcflow != NULL;
       pcflow = pic16_findNextpCode(pcflow->next, PC_FLOW) ) {

    int nFlows;
    int nConflicts;

    if(!isPCFL(pcflow)) {
      fprintf(stderr, "FixBankFlow - pcflow is not a flow object ");
      continue;
    }

    PCFL(pcflow)->FromConflicts = 0;
    PCFL(pcflow)->ToConflicts = 0;

    nFlows = 0;
    nConflicts = 0;

    //fprintf(stderr, " FixBankFlow flow seq %d\n",pcflow->seq);
    pcfl = setFirstItem(PCFL(pcflow)->from);
    while (pcfl) {

      pc = PCODE(pcfl->pcflow);

      if(!isPCFL(pc)) {
	fprintf(stderr,"oops dumpflow - to is not a pcflow\n");
	pc->print(stderr,pc);
      }

      nConflicts += compareBankFlow(PCFL(pcflow), pcfl, 0);
      nFlows++;

      pcfl=setNextItem(PCFL(pcflow)->from);
    }

    if((nFlows >= 2) && nConflicts && (PCFL(pcflow)->firstBank>0)) {
      //fprintf(stderr, " From conflicts flow seq %d, nflows %d ,nconflicts %d\n",pcflow->seq,nFlows, nConflicts);

      FixRegisterBankingInFlow(PCFL(pcflow),0);
      BanksUsedFlow2(pcflow);

      continue;  /* Don't need to check the flow from here - it's already been fixed */

    }

    nFlows = 0;
    nConflicts = 0;

    pcfl = setFirstItem(PCFL(pcflow)->to);
    while (pcfl) {

      pc = PCODE(pcfl->pcflow);
      if(!isPCFL(pc)) {
	fprintf(stderr,"oops dumpflow - to is not a pcflow\n");
	pc->print(stderr,pc);
      }

      nConflicts += compareBankFlow(PCFL(pcflow), pcfl, 1);
      nFlows++;

      pcfl=setNextItem(PCFL(pcflow)->to);
    }

    if((nFlows >= 2) && nConflicts &&(nConflicts != nFlows) && (PCFL(pcflow)->lastBank>0)) {
      //fprintf(stderr, " To conflicts flow seq %d, nflows %d ,nconflicts %d\n",pcflow->seq,nFlows, nConflicts);

      FixRegisterBankingInFlow(PCFL(pcflow),0);
      BanksUsedFlow2(pcflow);
    }
  }

  /*
    Loop through the flow objects again and find the ones with the 
    maximum conflicts
  */

  for( pcflow = pic16_findNextpCode(pb->pcHead, PC_FLOW); 
       pcflow != NULL;
       pcflow = pic16_findNextpCode(pcflow->next, PC_FLOW) ) {

    if(PCFL(pcflow)->ToConflicts > max_ToConflicts)
      pcflow_max_To = pcflow;

    if(PCFL(pcflow)->FromConflicts > max_FromConflicts)
      pcflow_max_From = pcflow;
  }
/*
  if(pcflow_max_To)
    fprintf(stderr,"compare flow Max To conflicts: seq %d conflicts %d\n",
	    PCFL(pcflow_max_To)->pc.seq,
	    PCFL(pcflow_max_To)->ToConflicts);

  if(pcflow_max_From)
    fprintf(stderr,"compare flow Max From conflicts: seq %d conflicts %d\n",
	    PCFL(pcflow_max_From)->pc.seq,
	    PCFL(pcflow_max_From)->FromConflicts);
*/
}
#if 0
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void DumpFlow(pBlock *pb)
{
  pCode *pc=NULL;
  pCode *pcflow;
  pCodeFlowLink *pcfl;


  fprintf(stderr,"Dump flow \n");
  pb->pcHead->print(stderr, pb->pcHead);

  pcflow = pic16_findNextpCode(pb->pcHead, PC_FLOW);
  pcflow->print(stderr,pcflow);

  for( pcflow = pic16_findNextpCode(pb->pcHead, PC_FLOW); 
       pcflow != NULL;
       pcflow = pic16_findNextpCode(pcflow->next, PC_FLOW) ) {

    if(!isPCFL(pcflow)) {
      fprintf(stderr, "DumpFlow - pcflow is not a flow object ");
      continue;
    }
    fprintf(stderr,"dumping: ");
    pcflow->print(stderr,pcflow);
    FlowStats(PCFL(pcflow));

    for(pcfl = setFirstItem(PCFL(pcflow)->to); pcfl; pcfl=setNextItem(PCFL(pcflow)->to)) {

      pc = PCODE(pcfl->pcflow);

      fprintf(stderr, "    from seq %d:\n",pc->seq);
      if(!isPCFL(pc)) {
	fprintf(stderr,"oops dumpflow - from is not a pcflow\n");
	pc->print(stderr,pc);
      }

    }

    for(pcfl = setFirstItem(PCFL(pcflow)->to); pcfl; pcfl=setNextItem(PCFL(pcflow)->to)) {

      pc = PCODE(pcfl->pcflow);

      fprintf(stderr, "    to seq %d:\n",pc->seq);
      if(!isPCFL(pc)) {
	fprintf(stderr,"oops dumpflow - to is not a pcflow\n");
	pc->print(stderr,pc);
      }

    }

  }

}
#endif
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static int OptimizepBlock(pBlock *pb)
{
  pCode *pc, *pcprev;
  int matches =0;

  if(!pb || !peepOptimizing)
    return 0;

  DFPRINTF((stderr," Optimizing pBlock: %c\n",getpBlock_dbName(pb)));
/*
  for(pc = pb->pcHead; pc; pc = pc->next)
    matches += pic16_pCodePeepMatchRule(pc);
*/

  pc = pic16_findNextInstruction(pb->pcHead);
  if(!pc)
    return 0;

  pcprev = pc->prev;
  do {


    if(pic16_pCodePeepMatchRule(pc)) {

      matches++;

      if(pcprev)
	pc = pic16_findNextInstruction(pcprev->next);
      else 
	pc = pic16_findNextInstruction(pb->pcHead);
    } else
      pc = pic16_findNextInstruction(pc->next);
  } while(pc);

  if(matches)
    DFPRINTF((stderr," Optimizing pBlock: %c - matches=%d\n",getpBlock_dbName(pb),matches));
  return matches;

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static pCode * findInstructionUsingLabel(pCodeLabel *pcl, pCode *pcs)
{
  pCode *pc;

  for(pc = pcs; pc; pc = pc->next) {

    if((pc->type == PC_OPCODE) && 
       (PCI(pc)->pcop) && 
       (PCI(pc)->pcop->type == PO_LABEL) &&
       (PCOLAB(PCI(pc)->pcop)->key == pcl->key))
      return pc;
  }
 

  return NULL;
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void exchangeLabels(pCodeLabel *pcl, pCode *pc)
{

  char *s=NULL;

  if(isPCI(pc) && 
     (PCI(pc)->pcop) && 
     (PCI(pc)->pcop->type == PO_LABEL)) {

    pCodeOpLabel *pcol = PCOLAB(PCI(pc)->pcop);

    //fprintf(stderr,"changing label key from %d to %d\n",pcol->key, pcl->key);
    if(pcol->pcop.name)
      free(pcol->pcop.name);

    /* If the key is negative, then we (probably) have a label to
     * a function and the name is already defined */
       
    if(pcl->key>0)
      sprintf(s=buffer,"_%05d_DS_",pcl->key);
    else 
      s = pcl->label;

    //sprintf(buffer,"_%05d_DS_",pcl->key);
    if(!s) {
      fprintf(stderr, "ERROR %s:%d function label is null\n",__FUNCTION__,__LINE__);
    }
    pcol->pcop.name = Safe_strdup(s);
    pcol->key = pcl->key;
    //pc->print(stderr,pc);

  }


}

/*-----------------------------------------------------------------*/
/* pBlockRemoveUnusedLabels - remove the pCode labels from the     */
/*                            pCode chain if they're not used.     */
/*-----------------------------------------------------------------*/
static void pBlockRemoveUnusedLabels(pBlock *pb)
{
  pCode *pc; pCodeLabel *pcl;

  if(!pb)
    return;

  for(pc = pb->pcHead; (pc=pic16_findNextInstruction(pc->next)) != NULL; ) {

    pBranch *pbr = PCI(pc)->label;
    if(pbr && pbr->next) {
      pCode *pcd = pb->pcHead;

      //fprintf(stderr, "multiple labels\n");
      //pc->print(stderr,pc);

      pbr = pbr->next;
      while(pbr) {

	while ( (pcd = findInstructionUsingLabel(PCL(PCI(pc)->label->pc), pcd)) != NULL) {
	  //fprintf(stderr,"Used by:\n");
	  //pcd->print(stderr,pcd);

	  exchangeLabels(PCL(pbr->pc),pcd);

	  pcd = pcd->next;
	}
	pbr = pbr->next;
      }
    }
  }

  for(pc = pb->pcHead; pc; pc = pc->next) {

    if(isPCL(pc)) // pc->type == PC_LABEL)
      pcl = PCL(pc);
    else if (isPCI(pc) && PCI(pc)->label) //((pc->type == PC_OPCODE) && PCI(pc)->label)
      pcl = PCL(PCI(pc)->label->pc);
    else continue;

    //fprintf(stderr," found  A LABEL !!! key = %d, %s\n", pcl->key,pcl->label);

    /* This pCode is a label, so search the pBlock to see if anyone
     * refers to it */

    if( (pcl->key>0) && (!findInstructionUsingLabel(pcl, pb->pcHead))) {
    //if( !findInstructionUsingLabel(pcl, pb->pcHead)) {
      /* Couldn't find an instruction that refers to this label
       * So, unlink the pCode label from it's pCode chain
       * and destroy the label */
      //fprintf(stderr," removed  A LABEL !!! key = %d, %s\n", pcl->key,pcl->label);

      DFPRINTF((stderr," !!! REMOVED A LABEL !!! key = %d, %s\n", pcl->key,pcl->label));
      if(pc->type == PC_LABEL) {
	pic16_unlinkpCode(pc);
	pCodeLabelDestruct(pc);
      } else {
	unlinkpCodeFromBranch(pc, PCODE(pcl));
	/*if(pc->label->next == NULL && pc->label->pc == NULL) {
	  free(pc->label);
	}*/
      }

    }
  }

}


/*-----------------------------------------------------------------*/
/* pic16_pBlockMergeLabels - remove the pCode labels from the pCode      */
/*                     chain and put them into pBranches that are  */
/*                     associated with the appropriate pCode       */
/*                     instructions.                               */
/*-----------------------------------------------------------------*/
void pic16_pBlockMergeLabels(pBlock *pb)
{
  pBranch *pbr;
  pCode *pc, *pcnext=NULL;

  if(!pb)
    return;

  /* First, Try to remove any unused labels */
  //pBlockRemoveUnusedLabels(pb);

  /* Now loop through the pBlock and merge the labels with the opcodes */

  pc = pb->pcHead;
  //  for(pc = pb->pcHead; pc; pc = pc->next) {

  while(pc) {
    pCode *pcn = pc->next;

    if(pc->type == PC_LABEL) {

      //fprintf(stderr," checking merging label %s\n",PCL(pc)->label);
      //fprintf(stderr,"Checking label key = %d\n",PCL(pc)->key);
      if((pcnext = pic16_findNextInstruction(pc) )) {

	// Unlink the pCode label from it's pCode chain
	pic16_unlinkpCode(pc);
	
	//fprintf(stderr,"Merged label key = %d\n",PCL(pc)->key);
	// And link it into the instruction's pBranch labels. (Note, since
	// it's possible to have multiple labels associated with one instruction
	// we must provide a means to accomodate the additional labels. Thus
	// the labels are placed into the singly-linked list "label" as 
	// opposed to being a single member of the pCodeInstruction.)

	//_ALLOC(pbr,sizeof(pBranch));
	pbr = Safe_calloc(1,sizeof(pBranch));
	pbr->pc = pc;
	pbr->next = NULL;

	PCI(pcnext)->label = pic16_pBranchAppend(PCI(pcnext)->label,pbr);

      } else {
	fprintf(stderr, "WARNING: couldn't associate label %s with an instruction\n",PCL(pc)->label);
      }
    } else if(pc->type == PC_CSOURCE) {

      /* merge the source line symbolic info into the next instruction */
      if((pcnext = pic16_findNextInstruction(pc) )) {

	// Unlink the pCode label from it's pCode chain
	pic16_unlinkpCode(pc);
	PCI(pcnext)->cline = PCCS(pc);
	//fprintf(stderr, "merging CSRC\n");
	//genericPrint(stderr,pcnext);
      }

    }
    pc = pcn;
  }
  pBlockRemoveUnusedLabels(pb);

}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static int OptimizepCode(char dbName)
{
#define MAX_PASSES 4

  int matches = 0;
  int passes = 0;
  pBlock *pb;

  if(!the_pFile)
    return 0;

  DFPRINTF((stderr," Optimizing pCode\n"));

  do {
    matches = 0;
    for(pb = the_pFile->pbHead; pb; pb = pb->next) {
      if('*' == dbName || getpBlock_dbName(pb) == dbName)
	matches += OptimizepBlock(pb);
    }
  }
  while(matches && ++passes < MAX_PASSES);

  return matches;
}

/*-----------------------------------------------------------------*/
/* pic16_popCopyGPR2Bit - copy a pcode operator                          */
/*-----------------------------------------------------------------*/

pCodeOp *pic16_popCopyGPR2Bit(pCodeOp *pc, int bitval)
{
  pCodeOp *pcop;

  pcop = pic16_newpCodeOpBit(pc->name, bitval, 0);

  if( !( (pcop->type == PO_LABEL) ||
	 (pcop->type == PO_LITERAL) ||
	 (pcop->type == PO_STR) ))
    PCOR(pcop)->r = PCOR(pc)->r;  /* This is dangerous... */

  return pcop;
}



#if 0
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
int InstructionRegBank(pCode *pc)
{
  regs *reg;

  if( (reg = pic16_getRegFromInstruction(pc)) == NULL)
    return -1;

  return REG_BANK(reg);

}
#endif

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static void pic16_FixRegisterBanking(pBlock *pb)
{
  pCode *pc=NULL;
  pCode *pcprev=NULL;

  int cur_bank;
  regs *reg;

  if(!pb)
    return;

  //pc = pic16_findNextpCode(pb->pcHead, PC_FLOW);
  pc = pic16_findNextpCode(pb->pcHead, PC_OPCODE);
  if(!pc)
    return;
  /* loop through all of the flow blocks with in one pblock */

//	fprintf(stderr,"%s:%d: Register banking\n", __FUNCTION__, __LINE__);

  cur_bank = 0;
  do {
    /* at this point, pc should point to a PC_FLOW object */


    /* for each flow block, determine the register banking 
       requirements */

    //    do {
      if(isPCI(pc) && !PCI(pc)->is2MemOp) {
//		genericPrint(stderr, pc);

	reg = pic16_getRegFromInstruction(pc);
#if 0
	if(reg) {
	  fprintf(stderr, "%s:%d:  %s  %d\n",__FUNCTION__, __LINE__, reg->name, reg->rIdx);
	  fprintf(stderr, "addr = 0x%03x, bank = %d, bit=%d\tmapped = %d sfr=%d fix=%d\n",
		  reg->address,REG_BANK(reg),reg->isBitField, reg->isMapped,
		  	pic16_finalMapping[ reg->rIdx ].isSFR, reg->isFixed);

	}
#endif

#warning Fix this if-conditional
	/* the !(reg->rIdx==-1) is a temporary hack. It should be changed - VR 6-Jun-2003 */
	if( ( (reg /*&& !(reg->rIdx==-1)*/ && !isACCESS_BANK(reg) && (isBankInstruction(pc)==-1) && !(reg->alias == 0x80) )
		 /*|| (PCI(pc)->op != POC_CALL)*/ )
	      && (!isPCI_LIT(pc)) ) {


	  /* Examine the instruction before this one to make sure it is
	   * not a skip type instruction */
	  pcprev = findPrevpCode(pc->prev, PC_OPCODE);

	  if(!pcprev || (pcprev && !isPCI_SKIP(pcprev) /*&& !isBankInstruction(pcprev)*/)) {
	    int reg_bank;

		reg_bank =  (reg) ? REG_BANK(reg) : 0;

#if 0
		fprintf(stderr, "%s:%d add bank = %d\n", __FUNCTION__, __LINE__, reg_bank);
		pc->print(stderr, pc);
#endif

//		if (cur_bank != reg_bank) {
			cur_bank = reg_bank;
			insertBankSwitch(0, pc, (options.gen_banksel)?-1:cur_bank);	//cur_bank);
//		}

	  }else {
	    //fprintf(stderr, "Bummer can't switch banks\n");
	    ;
	  }
	}

	pcprev = pc;

      }

      pc = pc->next;
      // } while(pc && !(isPCFL(pc))); 


  }while (pc);

  if(pcprev && cur_bank) {

    int pos = 1;  /* Assume that the bank switch instruction(s)
		   * are inserted after this instruction */

    if((PCI(pcprev)->op == POC_RETLW) || 
       (PCI(pcprev)->op == POC_RETURN) || 
       (PCI(pcprev)->op == POC_RETFIE)) {

      /* oops, a RETURN - we need to switch banks *before* the RETURN */

      pos = 0;

    } 
	    
    /* Brute force - make sure that we point to bank 0 at the
     * end of each flow block */

    insertBankSwitch(pos, pcprev, 0);
/*
    new_pc = pic16_newpCode(POC_MOVLB, pic16_newpCodeOpLit(0));
    pic16_pCodeInsertAfter(pcprev, new_pc);
*/
    cur_bank = 0;
    //fprintf(stderr, "Brute force switch\n");
  }

}




#if 0
	if(reg && REG_BANK(reg)!=cur_bank) {
	  //fprintf(stderr,"need to switch banks\n");
	  /* Examine the instruction before this one to make sure it is
	   * not a skip type instruction */
	  pcprev = findPrevpCode(pc->prev, PC_OPCODE);
	  if(!pcprev || (pcprev && !isPCI_SKIP(pcprev))) {
	    int b = cur_bank ^ REG_BANK(reg);

	    cur_bank = REG_BANK(reg);

	    switch(b & 3) {
	    case 0:
	      break;
	    case 1:
	      insertBankSwitch(0, pc, cur_bank&1, PIC_RP0_BIT);
	      break;
	    case 2:
	      insertBankSwitch(0, pc, cur_bank&2, PIC_RP1_BIT);
	      insertBankSwitch(0, pc, cur_bank&2, PIC_RP1_BIT);
	      break;
	    case 3:
	      if(cur_bank & 3) {
		insertBankSwitch(0, pc, cur_bank&1, PIC_RP0_BIT);
		insertBankSwitch(0, pc, cur_bank&2, PIC_RP1_BIT);
	      } else
		insertBankSwitch(0, pc, -1, -1);
	      break;

	    }
#endif




static void pBlockDestruct(pBlock *pb)
{

  if(!pb)
    return;


  free(pb);

}

/*-----------------------------------------------------------------*/
/* void mergepBlocks(char dbName) - Search for all pBlocks with the*/
/*                                  name dbName and combine them   */
/*                                  into one block                 */
/*-----------------------------------------------------------------*/
static void mergepBlocks(char dbName)
{

  pBlock *pb, *pbmerged = NULL,*pbn;

  pb = the_pFile->pbHead;

  //fprintf(stderr," merging blocks named %c\n",dbName);
  while(pb) {

    pbn = pb->next;
    //fprintf(stderr,"looking at %c\n",getpBlock_dbName(pb));
    if( getpBlock_dbName(pb) == dbName) {

      //fprintf(stderr," merged block %c\n",dbName);

      if(!pbmerged) {
	pbmerged = pb;
      } else {
	pic16_addpCode2pBlock(pbmerged, pb->pcHead);
	/* pic16_addpCode2pBlock doesn't handle the tail: */
	pbmerged->pcTail = pb->pcTail;

	pb->prev->next = pbn;
	if(pbn) 
	  pbn->prev = pb->prev;


	pBlockDestruct(pb);
      }
      //pic16_printpBlock(stderr, pbmerged);
    } 
    pb = pbn;
  }

}

/*-----------------------------------------------------------------*/
/* AnalyzeFlow - Examine the flow of the code and optimize         */
/*                                                                 */
/* level 0 == minimal optimization                                 */
/*   optimize registers that are used only by two instructions     */
/* level 1 == maximal optimization                                 */
/*   optimize by looking at pairs of instructions that use the     */
/*   register.                                                     */
/*-----------------------------------------------------------------*/

static void AnalyzeFlow(int level)
{
  static int times_called=0;

  pBlock *pb;

  if(!the_pFile)
    return;


  /* if this is not the first time this function has been called,
     then clean up old flow information */
  if(times_called++) {
    for(pb = the_pFile->pbHead; pb; pb = pb->next)
      unBuildFlow(pb);

    pic16_RegsUnMapLiveRanges();

  }

  GpcFlowSeq = 1;

  /* Phase 2 - Flow Analysis - Register Banking
   *
   * In this phase, the individual flow blocks are examined
   * and register banking is fixed.
   */

#if 0
	for(pb = the_pFile->pbHead; pb; pb = pb->next)
		pic16_FixRegisterBanking(pb);
#endif

  /* Phase 2 - Flow Analysis
   *
   * In this phase, the pCode is partition into pCodeFlow 
   * blocks. The flow blocks mark the points where a continuous
   * stream of instructions changes flow (e.g. because of
   * a call or goto or whatever).
   */

  for(pb = the_pFile->pbHead; pb; pb = pb->next)
    pic16_BuildFlow(pb);


  /* Phase 2 - Flow Analysis - linking flow blocks
   *
   * In this phase, the individual flow blocks are examined
   * to determine their order of excution.
   */

  for(pb = the_pFile->pbHead; pb; pb = pb->next)
    LinkFlow(pb);

  /* Phase 3 - Flow Analysis - Flow Tree
   *
   * In this phase, the individual flow blocks are examined
   * to determine their order of excution.
   */

  for(pb = the_pFile->pbHead; pb; pb = pb->next)
    pic16_BuildFlowTree(pb);


  /* Phase x - Flow Analysis - Used Banks
   *
   * In this phase, the individual flow blocks are examined
   * to determine the Register Banks they use
   */

  for(pb = the_pFile->pbHead; pb; pb = pb->next)
    FixBankFlow(pb);


  for(pb = the_pFile->pbHead; pb; pb = pb->next)
    pic16_pCodeRegMapLiveRanges(pb);

  pic16_RemoveUnusedRegisters();

  //  for(pb = the_pFile->pbHead; pb; pb = pb->next)
  pic16_pCodeRegOptimizeRegUsage(level);


	if(!options.nopeep)
		OptimizepCode('*');


/*
  for(pb = the_pFile->pbHead; pb; pb = pb->next)
    DumpFlow(pb);
*/
  /* debug stuff */ 
  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    pCode *pcflow;
    for( pcflow = pic16_findNextpCode(pb->pcHead, PC_FLOW); 
	 (pcflow = pic16_findNextpCode(pcflow, PC_FLOW)) != NULL;
	 pcflow = pcflow->next) {

      FillFlow(PCFL(pcflow));
    }
  }

/*
  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    pCode *pcflow;
    for( pcflow = pic16_findNextpCode(pb->pcHead, PC_FLOW); 
	 (pcflow = pic16_findNextpCode(pcflow, PC_FLOW)) != NULL;
	 pcflow = pcflow->next) {

      FlowStats(PCFL(pcflow));
    }
  }
*/
}

/*-----------------------------------------------------------------*/
/* pic16_AnalyzeBanking - Called after the memory addresses have been    */
/*                  assigned to the registers.                     */
/*                                                                 */
/*-----------------------------------------------------------------*/

void pic16_AnalyzeBanking(void)
{
  pBlock  *pb;

  if(!pic16_picIsInitialized()) {
    fprintf(stderr,"Temporary ERROR: at the moment you have to use\n");
    fprintf(stderr,"an include file create by inc2h.pl. See SDCC source:\n");
    fprintf(stderr,"support/scripts/inc2h.pl\n");
    fprintf(stderr,"this is a nuisance bug that will be fixed shortly\n");

    exit(1);
  }

  /* Phase x - Flow Analysis - Used Banks
   *
   * In this phase, the individual flow blocks are examined
   * to determine the Register Banks they use
   */

  AnalyzeFlow(0);
  AnalyzeFlow(1);

  for(pb = the_pFile->pbHead; pb; pb = pb->next)
    BanksUsedFlow(pb);

  for(pb = the_pFile->pbHead; pb; pb = pb->next)
    pic16_FixRegisterBanking(pb);

}

/*-----------------------------------------------------------------*/
/* buildCallTree - Look at the flow and extract all of the calls.  */
/*-----------------------------------------------------------------*/
static set *register_usage(pBlock *pb);

static void buildCallTree(void    )
{
  pBranch *pbr;
  pBlock  *pb;
  pCode   *pc;

  if(!the_pFile)
    return;



  /* Now build the call tree.
     First we examine all of the pCodes for functions.
     Keep in mind that the function boundaries coincide
     with pBlock boundaries. 

     The algorithm goes something like this:
     We have two nested loops. The outer loop iterates
     through all of the pBlocks/functions. The inner
     loop iterates through all of the pCodes for
     a given pBlock. When we begin iterating through
     a pBlock, the variable pc_fstart, pCode of the start
     of a function, is cleared. We then search for pCodes
     of type PC_FUNCTION. When one is encountered, we
     initialize pc_fstart to this and at the same time
     associate a new pBranch object that signifies a 
     branch entry. If a return is found, then this signifies
     a function exit point. We'll link the pCodes of these
     returns to the matching pc_fstart.

     When we're done, a doubly linked list of pBranches
     will exist. The head of this list is stored in
     `the_pFile', which is the meta structure for all
     of the pCode. Look at the pic16_printCallTree function
     on how the pBranches are linked together.

   */
  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    pCode *pc_fstart=NULL;
    for(pc = pb->pcHead; pc; pc = pc->next) {
      if(isPCF(pc)) {
	if (PCF(pc)->fname) {

	  if(STRCASECMP(PCF(pc)->fname, "_main") == 0) {
	    //fprintf(stderr," found main \n");
	    pb->cmemmap = NULL;  /* FIXME do we need to free ? */
	    pb->dbName = 'M';
	  }

	  pbr = Safe_calloc(1,sizeof(pBranch));
	  pbr->pc = pc_fstart = pc;
	  pbr->next = NULL;

	  the_pFile->functions = pic16_pBranchAppend(the_pFile->functions,pbr);

	  // Here's a better way of doing the same:
	  addSet(&pb->function_entries, pc);

	} else {
	  // Found an exit point in a function, e.g. return
	  // (Note, there may be more than one return per function)
	  if(pc_fstart)
	    pBranchLink(PCF(pc_fstart), PCF(pc));

	  addSet(&pb->function_exits, pc);
	}
      } else if(isCALL(pc)) {
	addSet(&pb->function_calls,pc);
      }
    }
  }

  /* Re-allocate the registers so that there are no collisions
   * between local variables when one function call another */

  // this is weird...
  //  pic16_deallocateAllRegs();

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if(!pb->visited)
      register_usage(pb);
  }

}

/*-----------------------------------------------------------------*/
/* pic16_AnalyzepCode - parse the pCode that has been generated and form */
/*                all of the logical connections.                  */
/*                                                                 */
/* Essentially what's done here is that the pCode flow is          */
/* determined.                                                     */
/*-----------------------------------------------------------------*/

void pic16_AnalyzepCode(char dbName)
{
  pBlock *pb;
  int i,changes;

  if(!the_pFile)
    return;

  mergepBlocks('D');


  /* Phase 1 - Register allocation and peep hole optimization
   *
   * The first part of the analysis is to determine the registers
   * that are used in the pCode. Once that is done, the peep rules
   * are applied to the code. We continue to loop until no more
   * peep rule optimizations are found (or until we exceed the
   * MAX_PASSES threshold). 
   *
   * When done, the required registers will be determined.
   *
   */
  i = 0;
  do {

    DFPRINTF((stderr," Analyzing pCode: PASS #%d\n",i+1));
    //fprintf(stderr," Analyzing pCode: PASS #%d\n",i+1);

    /* First, merge the labels with the instructions */
    for(pb = the_pFile->pbHead; pb; pb = pb->next) {
      if('*' == dbName || getpBlock_dbName(pb) == dbName) {

	DFPRINTF((stderr," analyze and merging block %c\n",dbName));
	//fprintf(stderr," analyze and merging block %c\n",dbName);
	pic16_pBlockMergeLabels(pb);
	AnalyzepBlock(pb);
      } else {
	DFPRINTF((stderr," skipping block analysis dbName=%c blockname=%c\n",dbName,getpBlock_dbName));
      }
    }

	if(!options.nopeep)
		changes = OptimizepCode(dbName);
	else changes = 0;

  } while(changes && (i++ < MAX_PASSES));

  buildCallTree();
}

/*-----------------------------------------------------------------*/
/* ispCodeFunction - returns true if *pc is the pCode of a         */
/*                   function                                      */
/*-----------------------------------------------------------------*/
static bool ispCodeFunction(pCode *pc)
{

  if(pc && pc->type == PC_FUNCTION && PCF(pc)->fname)
    return 1;

  return 0;
}

/*-----------------------------------------------------------------*/
/* findFunction - Search for a function by name (given the name)   */
/*                in the set of all functions that are in a pBlock */
/* (note - I expect this to change because I'm planning to limit   */
/*  pBlock's to just one function declaration                      */
/*-----------------------------------------------------------------*/
static pCode *findFunction(char *fname)
{
  pBlock *pb;
  pCode *pc;
  if(!fname)
    return NULL;

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {

    pc = setFirstItem(pb->function_entries);
    while(pc) {
    
      if((pc->type == PC_FUNCTION) &&
	 (PCF(pc)->fname) && 
	 (strcmp(fname, PCF(pc)->fname)==0))
	return pc;

      pc = setNextItem(pb->function_entries);

    }

  }
  return NULL;
}

static void MarkUsedRegisters(set *regset)
{

  regs *r1,*r2;

  for(r1=setFirstItem(regset); r1; r1=setNextItem(regset)) {
//	fprintf(stderr, "marking register = %s\t", r1->name);
    r2 = pic16_regWithIdx(r1->rIdx);
//	fprintf(stderr, "to register = %s\n", r2->name);
    r2->isFree = 0;
    r2->wasUsed = 1;
  }
}

static void pBlockStats(FILE *of, pBlock *pb)
{

  pCode *pc;
  regs  *r;

  fprintf(of,";***\n;  pBlock Stats: dbName = %c\n;***\n",getpBlock_dbName(pb));

  // for now just print the first element of each set
  pc = setFirstItem(pb->function_entries);
  if(pc) {
    fprintf(of,";entry:  ");
    pc->print(of,pc);
  }
  pc = setFirstItem(pb->function_exits);
  if(pc) {
    fprintf(of,";has an exit\n");
    //pc->print(of,pc);
  }

  pc = setFirstItem(pb->function_calls);
  if(pc) {
    fprintf(of,";functions called:\n");

    while(pc) {
      if(pc->type == PC_OPCODE && PCI(pc)->op == POC_CALL) {
	fprintf(of,";   %s\n",pic16_get_op_from_instruction(PCI(pc)));
      }
      pc = setNextItem(pb->function_calls);
    }
  }

  r = setFirstItem(pb->tregisters);
  if(r) {
    int n = elementsInSet(pb->tregisters);

    fprintf(of,";%d compiler assigned register%c:\n",n, ( (n!=1) ? 's' : ' '));

    while (r) {
      fprintf(of,";   %s\n",r->name);
      r = setNextItem(pb->tregisters);
    }
  }
}

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
#if 0
static void sequencepCode(void)
{
  pBlock *pb;
  pCode *pc;


  for(pb = the_pFile->pbHead; pb; pb = pb->next) {

    pb->seq = GpCodeSequenceNumber+1;

    for( pc = pb->pcHead; pc; pc = pc->next)
      pc->seq = ++GpCodeSequenceNumber;
  }

}
#endif

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static set *register_usage(pBlock *pb)
{
  pCode *pc,*pcn;
  set *registers=NULL;
  set *registersInCallPath = NULL;

  /* check recursion */

  pc = setFirstItem(pb->function_entries);

  if(!pc)
    return registers;

  pb->visited = 1;

  if(pc->type != PC_FUNCTION)
    fprintf(stderr,"%s, first pc is not a function???\n",__FUNCTION__);

  pc = setFirstItem(pb->function_calls);
  for( ; pc; pc = setNextItem(pb->function_calls)) {

    if(pc->type == PC_OPCODE && PCI(pc)->op == POC_CALL) {
      char *dest = pic16_get_op_from_instruction(PCI(pc));

      pcn = findFunction(dest);
      if(pcn) 
	registersInCallPath = register_usage(pcn->pb);
    } else
      fprintf(stderr,"BUG? pCode isn't a POC_CALL %d\n",__LINE__);

  }

#ifdef PCODE_DEBUG
  pBlockStats(stderr,pb);  // debug
#endif

  // Mark the registers in this block as used.

  MarkUsedRegisters(pb->tregisters);
  if(registersInCallPath) {
    /* registers were used in the functions this pBlock has called */
    /* so now, we need to see if these collide with the ones we are */
    /* using here */

    regs *r1,*r2, *newreg;

    DFPRINTF((stderr,"comparing registers\n"));

    r1 = setFirstItem(registersInCallPath);
    while(r1) {

      r2 = setFirstItem(pb->tregisters);

      while(r2 && (r1->type != REG_STK)) {

	if(r2->rIdx == r1->rIdx) {
	  newreg = pic16_findFreeReg(REG_GPR);


	  if(!newreg) {
	    DFPRINTF((stderr,"Bummer, no more registers.\n"));
	    exit(1);
	  }

	  DFPRINTF((stderr,"Cool found register collision nIdx=%d moving to %d\n",
		  r1->rIdx, newreg->rIdx));
	  r2->rIdx = newreg->rIdx;
	  //if(r2->name) free(r2->name);
	  if(newreg->name)
	    r2->name = Safe_strdup(newreg->name);
	  else
	    r2->name = NULL;
	  newreg->isFree = 0;
	  newreg->wasUsed = 1;
	}
	r2 = setNextItem(pb->tregisters);
      }

      r1 = setNextItem(registersInCallPath);
    }

    /* Collisions have been resolved. Now free the registers in the call path */
    r1 = setFirstItem(registersInCallPath);
    while(r1) {
      if(r1->type != REG_STK) {
	newreg = pic16_regWithIdx(r1->rIdx);
	newreg->isFree = 1;
      }
      r1 = setNextItem(registersInCallPath);
    }

  }// else
  //    MarkUsedRegisters(pb->registers);

  registers = unionSets(pb->tregisters, registersInCallPath, THROW_NONE);
#ifdef PCODE_DEBUG
  if(registers) 
    DFPRINTF((stderr,"returning regs\n"));
  else
    DFPRINTF((stderr,"not returning regs\n"));

  DFPRINTF((stderr,"pBlock after register optim.\n"));
  pBlockStats(stderr,pb);  // debug
#endif

  return registers;
}

/*-----------------------------------------------------------------*/
/* pct2 - writes the call tree to a file                           */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void pct2(FILE *of,pBlock *pb,int indent)
{
  pCode *pc,*pcn;
  int i;
  //  set *registersInCallPath = NULL;

  if(!of)
    return;

  if(indent > 10)
    return; //recursion ?

  pc = setFirstItem(pb->function_entries);

  if(!pc)
    return;

  pb->visited = 0;

  for(i=0;i<indent;i++)   // Indentation
    fputc(' ',of);

  if(pc->type == PC_FUNCTION)
    fprintf(of,"%s\n",PCF(pc)->fname);
  else
    return;  // ???


  pc = setFirstItem(pb->function_calls);
  for( ; pc; pc = setNextItem(pb->function_calls)) {

    if(pc->type == PC_OPCODE && PCI(pc)->op == POC_CALL) {
      char *dest = pic16_get_op_from_instruction(PCI(pc));

      pcn = findFunction(dest);
      if(pcn) 
	pct2(of,pcn->pb,indent+1);
    } else
      fprintf(of,"BUG? pCode isn't a POC_CALL %d\n",__LINE__);

  }


}


/*-----------------------------------------------------------------*/
/* pic16_printCallTree - writes the call tree to a file                  */
/*                                                                 */
/*-----------------------------------------------------------------*/

void pic16_printCallTree(FILE *of)
{
  pBranch *pbr;
  pBlock  *pb;
  pCode   *pc;

  if(!the_pFile)
    return;

  if(!of)
    of = stderr;

  fprintf(of, "\npBlock statistics\n");
  for(pb = the_pFile->pbHead; pb;  pb = pb->next )
    pBlockStats(of,pb);



  fprintf(of,"Call Tree\n");
  pbr = the_pFile->functions;
  while(pbr) {
    if(pbr->pc) {
      pc = pbr->pc;
      if(!ispCodeFunction(pc))
	fprintf(of,"bug in call tree");


      fprintf(of,"Function: %s\n", PCF(pc)->fname);

      while(pc->next && !ispCodeFunction(pc->next)) {
	pc = pc->next;
	if(pc->type == PC_OPCODE && PCI(pc)->op == POC_CALL)
	  fprintf(of,"\t%s\n",pic16_get_op_from_instruction(PCI(pc)));
      }
    }

    pbr = pbr->next;
  }


  fprintf(of,"\n**************\n\na better call tree\n");
  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    if(pb->visited)
      pct2(of,pb,0);
  }

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {
    fprintf(of,"block dbname: %c\n", getpBlock_dbName(pb));
  }
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/*-----------------------------------------------------------------*/

static void InlineFunction(pBlock *pb)
{
  pCode *pc;
  pCode *pc_call;

  if(!pb)
    return;

  pc = setFirstItem(pb->function_calls);

  for( ; pc; pc = setNextItem(pb->function_calls)) {

    if(isCALL(pc)) {
      pCode *pcn = findFunction(pic16_get_op_from_instruction(PCI(pc)));
      pCode *pct;
      pCode *pce;

      pBranch *pbr;

      if(pcn && isPCF(pcn) && (PCF(pcn)->ncalled == 1)) {
	
	//fprintf(stderr,"Cool can inline:\n");
	//pcn->print(stderr,pcn);

	//fprintf(stderr,"recursive call Inline\n");
	InlineFunction(pcn->pb);
	//fprintf(stderr,"return from recursive call Inline\n");

	/*
	  At this point, *pc points to a CALL mnemonic, and
	  *pcn points to the function that is being called.

	  To in-line this call, we need to remove the CALL
	  and RETURN(s), and link the function pCode in with
	  the CALLee pCode.

	*/


	/* Remove the CALL */
	pc_call = pc;
	pc = pc->prev;

	/* remove callee pBlock from the pBlock linked list */
	removepBlock(pcn->pb);

	pce = pcn;
	while(pce) {
	  pce->pb = pb;
	  pce = pce->next;
	}

	/* Remove the Function pCode */
	pct = pic16_findNextInstruction(pcn->next);

	/* Link the function with the callee */
	pc->next = pcn->next;
	pcn->next->prev = pc;
	
	/* Convert the function name into a label */

	pbr = Safe_calloc(1,sizeof(pBranch));
	pbr->pc = pic16_newpCodeLabel(PCF(pcn)->fname, -1);
	pbr->next = NULL;
	PCI(pct)->label = pic16_pBranchAppend(PCI(pct)->label,pbr);
	PCI(pct)->label = pic16_pBranchAppend(PCI(pct)->label,PCI(pc_call)->label);

	/* turn all of the return's except the last into goto's */
	/* check case for 2 instruction pBlocks */
	pce = pic16_findNextInstruction(pcn->next);
	while(pce) {
	  pCode *pce_next = pic16_findNextInstruction(pce->next);

	  if(pce_next == NULL) {
	    /* found the last return */
	    pCode *pc_call_next =  pic16_findNextInstruction(pc_call->next);

	    //fprintf(stderr,"found last return\n");
	    //pce->print(stderr,pce);
	    pce->prev->next = pc_call->next;
	    pc_call->next->prev = pce->prev;
	    PCI(pc_call_next)->label = pic16_pBranchAppend(PCI(pc_call_next)->label,
						      PCI(pce)->label);
	  }

	  pce = pce_next;
	}


      }
    } else
      fprintf(stderr,"BUG? pCode isn't a POC_CALL %d\n",__LINE__);

  }

}

/*-----------------------------------------------------------------*/
/*                                                                 */
/*-----------------------------------------------------------------*/

void pic16_InlinepCode(void)
{

  pBlock  *pb;
  pCode   *pc;

  if(!the_pFile)
    return;

  if(!functionInlining)
    return;

  /* Loop through all of the function definitions and count the
   * number of times each one is called */
  //fprintf(stderr,"inlining %d\n",__LINE__);

  for(pb = the_pFile->pbHead; pb; pb = pb->next) {

    pc = setFirstItem(pb->function_calls);

    for( ; pc; pc = setNextItem(pb->function_calls)) {

      if(isCALL(pc)) {
	pCode *pcn = findFunction(pic16_get_op_from_instruction(PCI(pc)));
	if(pcn && isPCF(pcn)) {
	  PCF(pcn)->ncalled++;
	}
      } else
	fprintf(stderr,"BUG? pCode isn't a POC_CALL %d\n",__LINE__);

    }
  }

  //fprintf(stderr,"inlining %d\n",__LINE__);

  /* Now, Loop through the function definitions again, but this
   * time inline those functions that have only been called once. */
  
  InlineFunction(the_pFile->pbHead);
  //fprintf(stderr,"inlining %d\n",__LINE__);

  for(pb = the_pFile->pbHead; pb; pb = pb->next)
    unBuildFlow(pb);

}
