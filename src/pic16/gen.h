/*-------------------------------------------------------------------------
  SDCCgen51.h - header file for code generation for 8051

             Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1998)
	     PIC port   - T. Scott Dattalo scott@dattalo.com (2000)
	     PIC16 port   - Martin Dubuc m.dubuc@rogers.com (2000)

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
   
   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!  
-------------------------------------------------------------------------*/

#ifndef SDCCGENPIC14_H
#define SDCCGENPIC14_H

struct pCodeOp;

enum
  {
    AOP_LIT = 1,
    AOP_REG, AOP_DIR,
    AOP_DPTR, AOP_DPTR2, AOP_R0, AOP_R1,
    AOP_STK, AOP_IMMD, AOP_STR,
    AOP_CRY, AOP_ACC,
    AOP_PCODE

  };

/* type asmop : a homogenised type for 
   all the different spaces an operand can be
   in */
typedef struct asmop
  {

    short type;			/* can have values
				   AOP_LIT    -  operand is a literal value
				   AOP_REG    -  is in registers
				   AOP_DIR    -  direct just a name
				   AOP_DPTR   -  dptr contains address of operand
				   AOP_DPTR2  -  dptr2 contains address of operand (DS80C390 only).
				   AOP_R0/R1  -  r0/r1 contains address of operand               
				   AOP_STK    -  should be pushed on stack this
				   can happen only for the result
				   AOP_IMMD   -  immediate value for eg. remateriazable 
				   AOP_CRY    -  carry contains the value of this
				   AOP_STR    -  array of strings
				   AOP_ACC    -  result is in the acc:b pair
				 */
    short coff;			/* current offset */
    short size;			/* total size */
    unsigned code:1;		/* is in Code space */
    unsigned paged:1;		/* in paged memory  */
    unsigned freed:1;		/* already freed    */
    union
      {
	value *aop_lit;		/* if literal */
	regs *aop_reg[4];	/* array of registers */
	char *aop_dir;		/* if direct  */
	regs *aop_ptr;		/* either -> to r0 or r1 */
	char *aop_immd;		/* if immediate others are implied */
	int aop_stk;		/* stack offset when AOP_STK */
	char *aop_str[4];	/* just a string array containing the location */
/*	regs *aop_alloc_reg;     * points to a dynamically allocated register */
	pCodeOp *pcop;
      }
    aopu;
  }
asmop;

void genpic16Code (iCode *);

//extern char *fReturnpic16[];
//extern char *fReturn390[];
extern unsigned pic16_fReturnSizePic;
//extern char **fReturn;


#define AOP(op) op->aop
#define AOP_TYPE(op) AOP(op)->type
#define AOP_SIZE(op) AOP(op)->size
#define IS_AOP_PREG(x) (AOP(x) && (AOP_TYPE(x) == AOP_R1 || \
                       AOP_TYPE(x) == AOP_R0))

#define AOP_NEEDSACC(x) (AOP(x) && (AOP_TYPE(x) == AOP_CRY ||  \
                        AOP_TYPE(x) == AOP_DPTR || AOP_TYPE(x) == AOP_DPTR2 || \
                         AOP(x)->paged)) 

#define AOP_INPREG(x) (x && (x->type == AOP_REG &&                        \
                      (x->aopu.aop_reg[0] == pic16_regWithIdx(R0_IDX) || \
                      x->aopu.aop_reg[0] == pic16_regWithIdx(R1_IDX) )))

#define RESULTONSTACK(x) \
                         (IC_RESULT(x) && IC_RESULT(x)->aop && \
                         IC_RESULT(x)->aop->type == AOP_STK )

#define MOVA(x) if (strcmp(x,"a") && strcmp(x,"acc")) pic16_emitcode(";XXX mov","a,%s  %s,%d",x,__FILE__,__LINE__);
#define CLRC    pic16_emitcode(";XXX clr","c %s,%d",__FILE__,__LINE__);

#define BIT_NUMBER(x) (x & 7)
#define BIT_REGISTER(x) (x>>3)


#define LSB     0
#define MSB16   1
#define MSB24   2
#define MSB32   3


#define FUNCTION_LABEL_INC  40

/*-----------------------------------------------------------------*/
/* Macros for emitting skip instructions                           */
/*-----------------------------------------------------------------*/

#define emitSKPC    pic16_emitpcode(POC_BTFSS,pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_C_BIT))
#define emitSKPNC   pic16_emitpcode(POC_BTFSC,pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_C_BIT))
#define emitSKPZ    pic16_emitpcode(POC_BTFSS,pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_Z_BIT))
#define emitSKPNZ   pic16_emitpcode(POC_BTFSC,pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_Z_BIT))
#define emitSKPDC   pic16_emitpcode(POC_BTFSS,pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_DC_BIT))
#define emitSKPNDC  pic16_emitpcode(POC_BTFSC,pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_DC_BIT))
#define emitCLRZ    pic16_emitpcode(POC_BCF,  pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_Z_BIT))
#define emitCLRC    pic16_emitpcode(POC_BCF,  pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_C_BIT))
#define emitCLRDC   pic16_emitpcode(POC_BCF,  pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_DC_BIT))
#define emitSETZ    pic16_emitpcode(POC_BSF,  pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_Z_BIT))
#define emitSETC    pic16_emitpcode(POC_BSF,  pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_C_BIT))
#define emitSETDC   pic16_emitpcode(POC_BSF,  pic16_popCopyGPR2Bit(PCOP(&pic16_pc_status),PIC_DC_BIT))

int pic16_getDataSize(operand *op);
void pic16_emitpcode(PIC_OPCODE poc, pCodeOp *pcop);
void pic16_emitpLabel(int key);
void pic16_emitcode (char *inst,char *fmt, ...);
void DEBUGpic16_emitcode (char *inst,char *fmt, ...);
bool pic16_sameRegs (asmop *aop1, asmop *aop2 );
char *pic16_aopGet (asmop *aop, int offset, bool bit16, bool dname);


bool pic16_genPlusIncr (iCode *ic);
void pic16_outBitAcc(operand *result);
void pic16_genPlusBits (iCode *ic);
void pic16_genPlus (iCode *ic);
bool pic16_genMinusDec (iCode *ic);
void pic16_addSign(operand *result, int offset, int sign);
void pic16_genMinusBits (iCode *ic);
void pic16_genMinus (iCode *ic);


pCodeOp *pic16_popGetLabel(unsigned int key);
pCodeOp *pic16_popCopyReg(pCodeOpReg *pc);
pCodeOp *pic16_popCopyGPR2Bit(pCodeOp *pc, int bitval);
pCodeOp *pic16_popGetLit(unsigned int lit);
pCodeOp *popGetWithString(char *str);
pCodeOp *pic16_popGet (asmop *aop, int offset);//, bool bit16, bool dname);
pCodeOp *pic16_popGetTempReg(void);
void pic16_popReleaseTempReg(pCodeOp *pcop);


void pic16_aopPut (asmop *aop, char *s, int offset);
void pic16_outAcc(operand *result);
void pic16_aopOp (operand *op, iCode *ic, bool result);
void pic16_outBitC(operand *result);
void pic16_toBoolean(operand *oper);
void pic16_freeAsmop (operand *op, asmop *aaop, iCode *ic, bool pop);
const char *pic16_pCodeOpType(  pCodeOp *pcop);



#endif
