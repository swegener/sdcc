#include "SDCCglobl.h"
#include "SDCCast.h"
#include "SDCCmem.h"
#include "SDCCy.h"
#include "SDCChasht.h"
#include "SDCCicode.h"
#include "SDCCerr.h"
#include "ralloc.h"

/* Not ported yet */
#if 0
/*-----------------------------------------------------------------*/
/* aggrToPtr - changes an aggregate to pointer to an aggregate     */
/*-----------------------------------------------------------------*/
link *aggrToPtr ( link *type, bool force)
{
    link *etype ;
    link *ptype ;

    
    if (IS_PTR(type) && !force)
	return type;

    etype = getSpec(type);
    ptype = newLink();

    ptype->next = type;
    /* if the output class is generic */
    if (SPEC_OCLS(etype) == generic)
	DCL_TYPE(ptype) = GPOINTER;
    else
	if (SPEC_OCLS(etype)->codesp ) {
	    DCL_TYPE(ptype) = POINTER ;
	    DCL_PTR_CONST(ptype) = 1;
	}
	else
	    DCL_TYPE(ptype) = POINTER ;
    
    /* if the variable was declared a constant */
    /* then the pointer points to a constant */
    if (IS_CONSTANT(etype) )
	DCL_PTR_CONST(ptype) = 1;

    /* the variable was volatile then pointer to volatile */
    if (IS_VOLATILE(etype))
	DCL_PTR_VOLATILE(ptype) = 1;
    return ptype; 
}

/*-----------------------------------------------------------------*/
/* geniCodeArray2Ptr - array to pointer                            */
/*-----------------------------------------------------------------*/
operand *geniCodeArray2Ptr (operand *op)
{
    link *optype = operandType(op);
    link *opetype = getSpec(optype);
    
    /* set the pointer depending on the storage class */
    if (SPEC_OCLS(opetype)->codesp ) {
	DCL_TYPE(optype) = POINTER ;
	DCL_PTR_CONST(optype) = 1;
    }
    else {
	DCL_TYPE(optype) = POINTER;
    }
    
    /* if the variable was declared a constant */
    /* then the pointer points to a constant */
    if (IS_CONSTANT(opetype) )
	DCL_PTR_CONST(optype) = 1;

    /* the variable was volatile then pointer to volatile */
    if (IS_VOLATILE(opetype))
	DCL_PTR_VOLATILE(optype) = 1;
    op->isaddr = 0;
    return op;
}

/*-----------------------------------------------------------------*/
/* geniCodeAddressOf - gens icode for '&' address of operator      */
/*-----------------------------------------------------------------*/
operand *geniCodeAddressOf (operand *op) 
{
    iCode *ic;
    link *p ;
    link *optype = operandType(op);
    link *opetype= getSpec(optype);
    
    /* this must be a lvalue */
    if (!op->isaddr && !IS_AGGREGATE(optype)) {
	werror (E_LVALUE_REQUIRED,"&");
	return op;
    }
    
    p = newLink();
    p->class = DECLARATOR ;
    /* set the pointer depending on the storage class */
    if (SPEC_OCLS(opetype)->codesp ) {
	DCL_TYPE(p) = POINTER ;
	DCL_PTR_CONST(p) = 1;
    }
    else {
	DCL_TYPE(p) = POINTER ;
    }
    
    /* make sure we preserve the const & volatile */
    if (IS_CONSTANT(opetype)) 
	DCL_PTR_CONST(p) = 1;

    if (IS_VOLATILE(opetype))
	DCL_PTR_VOLATILE(p) = 1;
    
    p->next = copyLinkChain(optype);
    
    /* if already a temp */
    if (IS_ITEMP(op)) {
	setOperandType (op,p);     
	op->isaddr= 0;
	return op;
    }
    
    /* other wise make this of the type coming in */
    ic = newiCode(ADDRESS_OF,op,NULL);
    IC_RESULT(ic) = newiTempOperand(p,1);
    IC_RESULT(ic)->isaddr = 0;
    ADDTOCHAIN(ic);
    return IC_RESULT(ic);
}

/*-----------------------------------------------------------------*/
/* setOClass - sets the output class depending on the pointer type */
/*-----------------------------------------------------------------*/
void setOClass (link *ptr, link *spec)
{
    switch (DCL_TYPE(ptr)) {
    case POINTER:
	SPEC_OCLS(spec) = data ;
	break ;
	
    case CPOINTER:
	SPEC_OCLS(spec) = code ;
	break ;  

    case GPOINTER:
	SPEC_OCLS(spec) = generic;
	break;

    default:
	assert(0);
    }
}

#endif
