// Philipp Klaus Krause, philipp@informatik.uni-frankfurt.de, pkk@spth.de, 2010 - 2011
//
// (c) 2010-2011 Goethe-Universität Frankfurt
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// An optimal, polynomial-time register allocator.

//#define DEBUG_RALLOC_DEC // Uncomment to get debug messages while doing register allocation on the tree decomposition.
//#define DEBUG_RALLOC_DEC_ASS // Uncomment to get debug messages about assignments while doing register allocation on the tree decomposition (much more verbose than the one above).

#include "SDCCralloc.hpp"


extern "C"
{
  unsigned char dryZ80iCode (iCode * ic);
};

#define REG_C 0
#define REG_B 1
#define REG_E 2
#define REG_D 3
#define REG_L 4
#define REG_H 5
#define REG_IYL 6
#define REG_IYH 7
#define REG_A (NUM_REGS - 1)

template <class G_t, class I_t>
float default_operand_cost(const operand *o, const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  float c = 0.0f;

  operand_map_t::const_iterator oi, oi_end;

  var_t byteregs[4];	// Todo: Change this when sdcc supports variables larger than 4 bytes.
  unsigned short int size;

  if(o && IS_SYMOP(o))
    {
      boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL_CONST(o)->key);
      if(oi != oi_end)
        {
          var_t v = oi->second;

          // In registers.
          if(a.local.find(v) != a.local.end())
            {
              c += 1.0f;
              byteregs[I[v].byte] = a.global[v];
              size = 1;

              while(++oi != oi_end)
                {
                  v = oi->second;
                  c += (a.local.find(v) != a.local.end() ? 1.0f : std::numeric_limits<float>::infinity());
                  byteregs[I[v].byte] = a.global[v];
                  size++;
                }

              // Penalty for not placing 2- and 4-byte variables in register pairs
              // Todo: Extend this once the register allcoator can use registers other than bc, de:
              if((size == 2 || size == 4) && (byteregs[1] != byteregs[0] + 1 || byteregs[0] != REG_C && byteregs[0] != REG_E && byteregs[0] != REG_L))
                c += 2.0f;
              if(size == 4 && (byteregs[3] != byteregs[2] + 1 || byteregs[2] != REG_C && byteregs[2] != REG_E && byteregs[0] != REG_L))
                c += 2.0f;

              // Code generator cannot handle variables only partially in A.
              if(OPTRALLOC_A && size > 1)
                for(unsigned short int i = 0; i < size; i++)
                  if(byteregs[i] == REG_A)
                    c += std::numeric_limits<float>::infinity();

              if(OPTRALLOC_A && byteregs[0] == REG_A)
                c -= 0.4f;
              else if(OPTRALLOC_HL && byteregs[0] == REG_L)
                c -= 0.1f;
              else if(OPTRALLOC_IY && byteregs[0] == REG_IYL || byteregs[0] == REG_IYH)
                c += 0.1f;
            }
          // Spilt.
          else
            {
              c += OP_SYMBOL_CONST(o)->remat ? 1.5f : 4.0f;
              while(++oi != oi_end)
                {
                  v = oi->second;
                  c += (a.local.find(v) == a.local.end() ? 4.0f : std::numeric_limits<float>::infinity());
                }
            }
        }
    }

  return(c);
}

// Check that the operand is either fully in registers or fully in memory.
template <class G_t, class I_t>
bool operand_sane(const operand *o, const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  if(!o || !IS_SYMOP(o))
    return(true);
 
  operand_map_t::const_iterator oi, oi_end;
  boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL_CONST(o)->key);
  
  if(oi == oi_end)
    return(true);
  
  // In registers.
  if(a.local.find(oi->second) != a.local.end())
    {
      while(++oi != oi_end)
        if(a.local.find(oi->second) == a.local.end())
          return(false);
    }
  else
    {
       while(++oi != oi_end)
        if(a.local.find(oi->second) != a.local.end())
          return(false);
    }
 
  return(true);
}

template <class G_t, class I_t> static float
default_instruction_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  float c = 0.0f;

  const iCode *ic = G[i].ic;

  c += default_operand_cost(IC_RESULT(ic), a, i, G, I);
  c += default_operand_cost(IC_LEFT(ic), a, i, G, I);
  c += default_operand_cost(IC_RIGHT(ic), a, i, G, I);

  return(c);
}

template <class G_t, class I_t> bool
inst_sane(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  const iCode *ic = G[i].ic;

  return(operand_sane(IC_RESULT(ic), a, i, G, I) && operand_sane(IC_LEFT(ic), a, i, G, I) && operand_sane(IC_RIGHT(ic), a, i, G, I));
}

// Treat assignment separately to handle coalescing.
template <class G_t, class I_t> static float
assign_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  float c = 0.0f;

  const iCode *ic = G[i].ic;

  const operand *right = IC_RIGHT(ic);
  const operand *result = IC_RESULT(ic);

  if(!right || !IS_SYMOP(right) || !result || !IS_SYMOP(result) || POINTER_GET(ic) || POINTER_SET(ic))
    return(default_instruction_cost(a, i, G, I));

  reg_t byteregs[4] = {-1, -1, -1, -1};	// Todo: Change this when sdcc supports variables larger than 4 bytes.

  operand_map_t::const_iterator oi, oi_end;

  int size1 = 0, size2 = 0;

  boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL_CONST(right)->key);
  if(oi != oi_end)
    {
      var_t v = oi->second;

      if(a.local.find(v) == a.local.end())
        return(default_instruction_cost(a, i, G, I));

      c += 1.0f;
      byteregs[I[v].byte] = a.global[v];
      size1 = 1;

      while(++oi != oi_end)
        {
          v = oi->second;
          c += (a.local.find(v) != a.local.end() ? 1.0f : std::numeric_limits<float>::infinity());
          byteregs[I[v].byte] = a.global[v];
          size1++;
        }

      // Code generator cannot handle variables only partially in A.
      if(OPTRALLOC_A && size1 > 1)
        for(unsigned short int i = 0; i < size1; i++)
          if(byteregs[i] == REG_A)
            c += std::numeric_limits<float>::infinity();

      if(OPTRALLOC_A && byteregs[0] == REG_A)
        c -= 0.4f;
      else if(OPTRALLOC_IY && byteregs[0] == REG_IYL || byteregs[0] == REG_IYH)
        c += 0.1f;
    }

  if(!size1)
    return(default_instruction_cost(a, i, G, I));

  boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL_CONST(result)->key);
  if(oi != oi_end)
    {
      var_t v = oi->second;

      if(a.local.find(v) == a.local.end())
        return(default_instruction_cost(a, i, G, I));

      c += 1.0f;
      if(byteregs[I[v].byte] == a.global[v])
        c -= 2.0f;
      size2 = 1;

      while(++oi != oi_end)
        {
          v = oi->second;
          c += (a.local.find(v) != a.local.end() ? 1.0f : std::numeric_limits<float>::infinity());
          if(byteregs[I[v].byte] == a.global[v])
            c -= 2.0f;
          size2++;
        }

      if(OPTRALLOC_A && byteregs[0] == REG_A)
        c -= 0.4f;
      else if(OPTRALLOC_IY && byteregs[0] == REG_IYL || byteregs[0] == REG_IYH)
        c += 0.1f;
    }

  if(!size2)
    return(default_instruction_cost(a, i, G, I));

  return(c);
}

template <class G_t, class I_t> static float
return_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  float c = 0.0f;

  const iCode *ic = G[i].ic;
  
  const operand *left = IC_LEFT(ic);
  
  if(!left || !IS_SYMOP(left))
    return(default_instruction_cost(a, i, G, I));

  reg_t byteregs[4] = {-1, -1, -1, -1};	// Todo: Change this when sdcc supports variables larger than 4 bytes.
  
  operand_map_t::const_iterator oi, oi_end;

  int size = 0;
  
  boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL_CONST(left)->key);
  if(oi != oi_end)
    {
      var_t v = oi->second;

      if(a.local.find(v) == a.local.end())
        return(default_instruction_cost(a, i, G, I));

      c += 1.0f;
      byteregs[I[v].byte] = a.global[v];
      size = 1;

      while(++oi != oi_end)
        {
          v = oi->second;
          c += (a.local.find(v) != a.local.end() ? 1.0f : std::numeric_limits<float>::infinity());
          byteregs[I[v].byte] = a.global[v];
          size++;
        }

      if(OPTRALLOC_A && byteregs[0] == REG_A)
        c -= 0.4f;
        
      if(byteregs[0] == REG_L)
        c -= 1.0f;
      if(byteregs[1] == REG_H)
        c -= 1.0f;
      if(byteregs[2] == REG_E)
        c -= 1.0f;
      if(byteregs[3] == REG_D)
        c -= 1.0f;
    }
    
  return(c);
}

template <class G_t, class I_t> static float
call_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  float c = 0.0f;

  const iCode *ic = G[i].ic;
  
  const operand *result = IC_RESULT(ic);
  
  if(!result || !IS_SYMOP(result))
    return(default_instruction_cost(a, i, G, I));

  reg_t byteregs[4] = {-1, -1, -1, -1};	// Todo: Change this when sdcc supports variables larger than 4 bytes.
  
  operand_map_t::const_iterator oi, oi_end;

  int size = 0;
  
  boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL_CONST(result)->key);
  if(oi != oi_end)
    {
      var_t v = oi->second;

      if(a.local.find(v) == a.local.end())
        return(default_instruction_cost(a, i, G, I));

      c += 1.0f;
      byteregs[I[v].byte] = a.global[v];
      size = 1;

      while(++oi != oi_end)
        {
          v = oi->second;
          c += (a.local.find(v) != a.local.end() ? 1.0f : std::numeric_limits<float>::infinity());
          byteregs[I[v].byte] = a.global[v];
          size++;
        }

      // Code generator cannot handle variables only partially in A.
      if(OPTRALLOC_A && size > 1)
        for(unsigned short int i = 0; i < size; i++)
          if(byteregs[i] == REG_A)
            c += std::numeric_limits<float>::infinity();

      if(OPTRALLOC_A && byteregs[0] == REG_A)
        c -= 0.4f;
        
      if(byteregs[0] == REG_L)
        c -= 1.0f;
      if(byteregs[1] == REG_H)
        c -= 1.0f;
      if(byteregs[2] == REG_E)
        c -= 1.0f;
      if(byteregs[3] == REG_D)
        c -= 1.0f;
    }
    
  return(c);
}

template <class G_t, class I_t> static float
ifx_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  const iCode *ic = G[i].ic;

  return(default_operand_cost(IC_COND(ic), a, i, G, I));
}

template <class G_t, class I_t> static float
jumptab_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  const iCode *ic = G[i].ic;

  return(default_operand_cost(IC_JTCOND(ic), a, i, G, I));
}

template <class I_t> void
add_operand_conflicts_in_node(const cfg_node &n, I_t &I)
{
  const iCode *ic = n.ic;
  
  const operand *result = IC_RESULT(ic);
  const operand *left = IC_LEFT(ic);
  const operand *right = IC_RIGHT(ic);
	
  if(!result || !IS_SYMOP(result))
    return;
    
  if(!(ic->op == '~' || ic->op == UNARYMINUS || ic->op == '+' || ic->op == '-' || ic->op == '^' || ic->op == '|' || ic->op == BITWISEAND)) 
    return; // Code generation can always handle all other operations. Todo: Handle ^, |, BITWISEAND there as well.
   
  operand_map_t::const_iterator oir, oir_end, oirs; 
  boost::tie(oir, oir_end) = n.operands.equal_range(OP_SYMBOL_CONST(result)->key);
  if(oir == oir_end)
    return;
    
  operand_map_t::const_iterator oio, oio_end;
  
  if(left && IS_SYMOP(left))
    for(boost::tie(oio, oio_end) = n.operands.equal_range(OP_SYMBOL_CONST(left)->key); oio != oio_end; ++oio)
      for(oirs = oir; oirs != oir_end; ++oirs)
        {
          var_t rvar = oirs->second;
          var_t ovar = oio->second;
          if(I[rvar].byte < I[ovar].byte)
            boost::add_edge(rvar, ovar, I);
        }
        
  if(right && IS_SYMOP(right))
    for(boost::tie(oio, oio_end) = n.operands.equal_range(OP_SYMBOL_CONST(right)->key); oio != oio_end; ++oio)
      for(oirs = oir; oirs != oir_end; ++oirs)
        {
          var_t rvar = oirs->second;
          var_t ovar = oio->second;
          if(I[rvar].byte < I[ovar].byte)
            boost::add_edge(rvar, ovar, I);
        }
}

// Return true, iff the operand is placed (partially) in r.
template <class G_t>
bool operand_in_reg(const operand *o, reg_t r, const i_assignment_t &ia, unsigned short int i, const G_t &G)
{
  if(!o || !IS_SYMOP(o))
    return(false);

  operand_map_t::const_iterator oi, oi_end;
  for(boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL_CONST(o)->key); oi != oi_end; ++oi)
    if(oi->second == ia.registers[r][1] || oi->second == ia.registers[r][0])
      return(true);

  return(false);
}

template <class G_t>
bool operand_is_pair(const operand *o, const assignment &a, unsigned short int i, const G_t &G)
{
  if(!o || !IS_SYMOP(o))
    return(false);

  operand_map_t::const_iterator oi, oi2, oi3, oi_end;
  boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL_CONST(o)->key);
  if(oi == oi_end)
    return(false);
  oi2 = oi;
  ++oi2;
  if(oi2 == oi_end)
    return(false);
  oi3 = oi2;
  ++oi3;
  if(oi3 != oi_end)
    return(false);

  if(a.global[oi->second] % 2)
    return(false);
  if(a.global[oi->second] + 1 != a.global[oi2->second])
    return(false);

  return(true);
}

template <class G_t, class I_t>
bool Ainst_ok(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  const iCode *ic = G[i].ic;
  const i_assignment_t &ia = a.i_assignment;

  if(ia.registers[REG_A][1] < 0)
    return(true);	// Register A not in use.

  //if(i == 15) std::cout << "Ainst_ok: A = (" << ia.registers[REG_A][0] << ", " << ia.registers[REG_A][1] << "), inst " << i << ", " << ic->key << "\n";

  // Code generator cannot handle variables that are only partially in A.
  if(I[ia.registers[REG_A][1]].size > 1 || ia.registers[REG_A][0] >= 0 && I[ia.registers[REG_A][0]].size > 1)
    return(false);

  // Check if the result of this instruction is placed in A.
  bool result_in_A = operand_in_reg(IC_RESULT(ic), REG_A, ia, i, G);
  
  // Check if an input of this instruction is placed in A.
  bool input_in_A;
  switch(ic->op)
    {
    case IFX:
      input_in_A = operand_in_reg(IC_COND(ic), REG_A, ia, i, G);
      break;
    case JUMPTABLE:
      input_in_A = operand_in_reg(IC_JTCOND(ic), REG_A, ia, i, G);
      break;
    default:
      input_in_A = operand_in_reg(IC_LEFT(ic), REG_A, ia, i, G) || operand_in_reg(IC_RIGHT(ic), REG_A, ia, i, G);
      break;
    }

  if(!result_in_A && !input_in_A)
    {
      // Variable in A is not used by this instruction
      if(ic->op == '+' && IS_ITEMP (IC_LEFT (ic)) && IS_ITEMP (IC_RESULT (ic)) && IS_OP_LITERAL (IC_RIGHT (ic)) &&
          ulFromVal (OP_VALUE (IC_RIGHT (ic))) == 1 &&
          OP_KEY (IC_RESULT (ic)) == OP_KEY (IC_LEFT (ic)))
        return(true);

      if(ic->op == '=' && !POINTER_SET (ic) && isOperandEqual(IC_RESULT(ic), IC_RIGHT(ic)))
        return(true);

      if(ic->op == GOTO || ic->op == LABEL)
        return(true);

      //if(i == 15) std::cout << "Not Used: Dropping at " << i << ", " << ic->key << "(" << int(ic->op) << "\n";
      return(false);
    }

  // Last use of operand in A.
  const std::set<var_t> &dying = G[i].dying;
  if(input_in_A && (result_in_A || dying.find(ia.registers[REG_A][1]) != dying.end() || dying.find(ia.registers[REG_A][0]) != dying.end()))
    {
      if(ic->op != IFX &&
        !((ic->op == RIGHT_OP || ic->op == LEFT_OP) && (IS_OP_LITERAL(IC_RIGHT(ic)) || operand_in_reg(IC_RIGHT(ic), REG_A, ia, i, G))) &&
        !(ic->op == '=' && !(IY_RESERVED && POINTER_SET(ic))) &&
        !IS_BITWISE_OP (ic) &&
        !(ic->op == '~') &&
        !(ic->op == '*' && IS_ITEMP(IC_LEFT(ic)) && IS_ITEMP(IC_RIGHT(ic))) &&
        !((ic->op == '-' || ic->op == '+' || ic->op == EQ_OP) && IS_OP_LITERAL(IC_RIGHT(ic))))
        {
          //if(i == 15) std::cout << "Last use: Dropping at " << i << ", " << ic->key << "(" << int(ic->op) << ")\n";
          return(false);
        }
    }
  // A is used, and has to be preserved for later use.
  else if(input_in_A &&
         ic->op != IFX &&
         ic->op != JUMPTABLE)
    {
      //if(i == 15) std::cout << "Intermediate use: Dropping at " << i << ", " << ic->key << "(" << int(ic->op) << "\n";
      return(false);
    }

  // First use of operand in A.
  if(result_in_A &&
      !POINTER_GET(ic) &&
      ic->op != '+' &&
      ic->op != '-' &&
      !IS_BITWISE_OP(ic) &&
      ic->op != '=' &&
      ic->op != EQ_OP &&
      ic->op != '<' &&
      ic->op != '>' &&
      ic->op != CAST &&
      ic->op != CALL &&
      ic->op != PCALL &&
      ic->op != GETHBIT &&
      !((ic->op == LEFT_OP || ic->op == RIGHT_OP) && IS_OP_LITERAL(IC_RIGHT(ic))))
    {
      //if(i == 15) std::cout << "First use: Dropping at " << i << ", " << ic->key << "(" << int(ic->op) << "\n";
      return(false);
    }

  //if(i == 15) std::cout << "Default OK\n";

  return(true);
}

template <class G_t, class I_t>
bool HLinst_ok(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  const iCode *ic = G[i].ic;

  // HL always unused on gbz80.
  if(TARGET_IS_GBZ80)
    return(true);

  const i_assignment_t &ia = a.i_assignment;

  bool unused_L = (ia.registers[REG_L][1] < 0);
  bool unused_H = (ia.registers[REG_H][1] < 0);

  if(unused_L && unused_H)
    return(true);	// Register HL not in use.

#if 0
  std::cout << "HLinst_ok: at (" << i << ", " << ic->key << ")\nL = (" << ia.registers[REG_L][0] << ", " << ia.registers[REG_L][1] << "), H = (" << ia.registers[REG_H][0] << ", " << ia.registers[REG_H][1] << ")inst " << i << ", " << ic->key << "\n";
#endif

  const operand *left = IC_LEFT(ic);
  const operand *right = IC_RIGHT(ic);
  const operand *result = IC_RESULT(ic);

  bool result_in_L = operand_in_reg(result, REG_L, ia, i, G);
  bool result_in_H = operand_in_reg(result, REG_H, ia, i, G);
  bool result_in_HL = result_in_L || result_in_H;

  bool input_in_L, input_in_H;
  switch(ic->op)
    {
    case IFX:
      input_in_L = operand_in_reg(IC_COND(ic), REG_L, ia, i, G);
      input_in_H = operand_in_reg(IC_COND(ic), REG_L, ia, i, G);
      break;
    case JUMPTABLE:
      input_in_L = operand_in_reg(IC_JTCOND(ic), REG_L, ia, i, G);
      input_in_H = operand_in_reg(IC_JTCOND(ic), REG_L, ia, i, G);
      break;
    default:
      input_in_L = operand_in_reg(left, REG_L, ia, i, G) || operand_in_reg(right, REG_L, ia, i, G);
      input_in_H = operand_in_reg(left, REG_H, ia, i, G) || operand_in_reg(right, REG_H, ia, i, G);
      break;
    }
  bool input_in_HL = input_in_L || input_in_H;

  const std::set<var_t> &dying = G[i].dying;
  
  bool dying_L = result_in_L || dying.find(ia.registers[REG_L][1]) != dying.end() || dying.find(ia.registers[REG_L][0]) != dying.end();
  bool dying_H = result_in_H || dying.find(ia.registers[REG_H][1]) != dying.end() || dying.find(ia.registers[REG_H][0]) != dying.end();

  bool result_only_HL = (result_in_L || unused_L || dying_L) && (result_in_H || unused_H || dying_H);

#if 0
  std::cout << "Result in L: " << result_in_L << ", result in H: " << result_in_H << "\n";
  std::cout << "Unsued L: " << unused_L << ", unused H: " << unused_H << "\n";
  std::cout << "Dying L: " << dying_L << ", dying H: " << dying_H << "\n";
  std::cout << "Result only HL: " << result_only_HL << "\n";
#endif

  if(ic->op == RETURN)
    return(true);

  if((IS_GB || IY_RESERVED) && (IS_TRUE_SYMOP(left) || IS_TRUE_SYMOP(right)))
    return(false);

  if(options.omitFramePtr)	// Todo: Make this more accurate to get better code when using --fomit-frame-pointer
	return(false);

  if(result_only_HL && ic->op == PCALL)
    return(true);

  if(ic->op == '+' && getSize(operandType(IC_RESULT(ic))) >= 2 &&
    (IS_TRUE_SYMOP (result) || IS_TRUE_SYMOP (left) || IS_TRUE_SYMOP (right))) // Might use (hl).
    return(false);

  if(ic->op == '+' && input_in_HL && IS_TRUE_SYMOP (result)) // Might use (hl) for result.
    return(false);
    
  // HL overwritten by result.
  if(result_only_HL && !POINTER_SET(ic) &&
      (ic->op == ADDRESS_OF ||
       ic->op == GET_VALUE_AT_ADDRESS ||
       ic->op == '+' ||
       ic->op == '*' ||
       ic->op == '='))
    return(true);

  if(IC_RESULT(ic) && IS_SYMOP(result) && isOperandInDirSpace(IC_RESULT(ic)))
    return(false);

  if((input_in_HL || !result_only_HL) && left && IS_SYMOP(left) && isOperandInDirSpace(IC_LEFT(ic)))
    return(false);

  if((input_in_HL || !result_only_HL) && right && IS_SYMOP(right) && isOperandInDirSpace(IC_RIGHT(ic)))
    return(false);

  // Operations that leave HL alone.
  if(ic->op == IFX)
    return(true);
  if(SKIP_IC2(ic))
    return(true);
  if(ic->op == IPUSH && input_in_H && (getSize(operandType(IC_LEFT(ic))) <= 2 || I[ia.registers[REG_L][1]].byte == 2 && I[ia.registers[REG_H][1]].byte == 3))
    return(true);
  if(POINTER_GET(ic) && input_in_L && input_in_H && (getSize(operandType(IC_RESULT(ic))) == 1 || !result_in_HL))
    return(true);
  if(ic->op == LEFT_OP && isOperandLiteral(IC_RIGHT(ic)))
    return(true);

  if((!POINTER_SET(ic) && !POINTER_GET(ic) && (
        (ic->op == '=' ||
         ic->op == CAST ||
         /*ic->op == UNARYMINUS ||*/
         ic->op == RIGHT_OP ||
         /*ic->op == '-' ||*/
         IS_BITWISE_OP(ic) ||
         /*ic->op == '>' ||
         ic->op == '<' ||
         ic->op == EQ_OP ||*/
         (ic->op == '+' && getSize(operandType(IC_RESULT(ic))) == 1) ||
         (ic->op == '+' && getSize(operandType(IC_RESULT(ic))) <= 2 && result_only_HL) ))))	// 16 bit addition might use add hl, rr
    return(true);

  if((ic->op == '<' || ic->op == '>') && (IS_ITEMP(left) || IS_OP_LITERAL(left) || IS_ITEMP(right) || IS_OP_LITERAL(right)))	// Todo: Fix for large stack.
    return(true);
    
  if(IS_VALOP(right) && ic->op == EQ_OP)
    return(true);

  // HL overwritten by result.
  if(result_only_HL && (ic->op == CALL || ic->op == PCALL))
    return(true);

  if(ic->op == '=' && POINTER_SET(ic) && !result_only_HL)	// loads result pointer into (hl) first.
    return(false);

  if(ic->op == '=' && !POINTER_GET(ic) && !input_in_HL)
    return(true);

#if 0
  std::cout << "HLinst_ok: L = (" << ia.registers[REG_L][0] << ", " << ia.registers[REG_L][1] << "), H = (" << ia.registers[REG_H][0] << ", " << ia.registers[REG_H][1] << ")inst " << i << ", " << ic->key << "\n";
  std::cout << "Result in L: " << result_in_L << ", result in H: " << result_in_H << "\n";
  std::cout << "HL default drop at " << ic->key << ", operation: " << ic->op << "\n";
#endif

  return(false);
}

template <class G_t, class I_t>
bool IYinst_ok(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  const iCode *ic = G[i].ic;

  // IY always unused on gbz80.
  if(TARGET_IS_GBZ80)
    return(true);

  const i_assignment_t &ia = a.i_assignment;

  //if(ic->key == 118)
//		std::cout << "1IYinst_ok: at (" << i << ", " << ic->key << ")\nIYL = (" << ia.registers[REG_IYL][0] << ", " << ia.registers[REG_IYL][1] << "), IYH = (" << ia.registers[REG_IYH][0] << ", " << ia.registers[REG_IYH][1] << ")inst " << i << ", " << ic->key << "\n";

  bool unused_IYL = (ia.registers[REG_IYL][1] < 0);
  bool unused_IYH = (ia.registers[REG_IYH][1] < 0);

  const operand *left = IC_LEFT(ic);
  const operand *right = IC_RIGHT(ic);
  const operand *result = IC_RESULT(ic);

  bool result_in_IYL = operand_in_reg(result, REG_IYL, ia, i, G);
  bool result_in_IYH = operand_in_reg(result, REG_IYH, ia, i, G);
  bool result_in_IY = result_in_IYL || result_in_IYH;

  bool input_in_IYL, input_in_IYH;
  switch(ic->op)
    {
    case IFX:
      input_in_IYL = operand_in_reg(IC_COND(ic), REG_IYL, ia, i, G);
      input_in_IYH = operand_in_reg(IC_COND(ic), REG_IYL, ia, i, G);
      break;
    case JUMPTABLE:
      input_in_IYL = operand_in_reg(IC_JTCOND(ic), REG_IYL, ia, i, G);
      input_in_IYH = operand_in_reg(IC_JTCOND(ic), REG_IYL, ia, i, G);
      break;
    default:
      input_in_IYL = operand_in_reg(left, REG_IYL, ia, i, G) || operand_in_reg(right, REG_IYL, ia, i, G);
      input_in_IYH = operand_in_reg(left, REG_IYH, ia, i, G) || operand_in_reg(right, REG_IYH, ia, i, G);
      break;
    }
  bool input_in_IY = input_in_IYL || input_in_IYH;

  //const std::set<var_t> &dying = G[i].dying;
  
  //bool dying_IYL = result_in_IYL || dying.find(ia.registers[REG_IYL][1]) != dying.end() || dying.find(ia.registers[REG_IYL][0]) != dying.end();
  //bool dying_IYH = result_in_IYH || dying.find(ia.registers[REG_IYH][1]) != dying.end() || dying.find(ia.registers[REG_IYH][0]) != dying.end();

  //bool result_only_IY = (result_in_IYL || unused_IYL || dying_IYL) && (result_in_IYH || unused_IYH || dying_IYH);

  if(unused_IYL && unused_IYH)
    return(true);	// Register IY not in use.
    
  if(options.omitFramePtr)	// Todo: Make this more accurate to get better code when using --fomit-frame-pointer
	return(false);

  // Code generator cannot handle variables that are only partially in IY.
  if(unused_IYL ^ unused_IYH)
    return(false);
  if(!unused_IYL && I[ia.registers[REG_IYL][1]].size != 2 || !unused_IYH && I[ia.registers[REG_IYH][1]].size != 2 ||
    ia.registers[REG_IYL][0] >= 0 && I[ia.registers[REG_IYL][0]].size != 2 || ia.registers[REG_IYH][0] >= 0 && I[ia.registers[REG_IYH][0]].size != 2)
    return(false);
  if(ia.registers[REG_IYL][1] >= 0 && (ia.registers[REG_IYH][1] <= 0 || I[ia.registers[REG_IYL][1]].v != I[ia.registers[REG_IYH][1]].v))
    return(false);
  if(ia.registers[REG_IYH][1] >= 0 && (ia.registers[REG_IYL][1] <= 0 || I[ia.registers[REG_IYH][1]].v != I[ia.registers[REG_IYL][1]].v))
    return(false);
  if(ia.registers[REG_IYL][0] >= 0 && (ia.registers[REG_IYH][0] <= 0 || I[ia.registers[REG_IYL][0]].v != I[ia.registers[REG_IYH][0]].v))
    return(false);
  if(ia.registers[REG_IYH][0] >= 0 && (ia.registers[REG_IYL][0] <= 0 || I[ia.registers[REG_IYH][0]].v != I[ia.registers[REG_IYL][0]].v))
    return(false);
  if(I[ia.registers[REG_IYL][1]].byte != 0 || I[ia.registers[REG_IYH][1]].byte != 1)
    return(false);
  if(ia.registers[REG_IYL][0] >= 0 && I[ia.registers[REG_IYL][0]].byte != 0 || ia.registers[REG_IYH][0] >= 0 && I[ia.registers[REG_IYH][0]].byte != 1)
    return(false);

#if 0
  std::cout << "IYinst_ok: Assignment: ";
  print_assignment(a);
  std::cout << "\n";
  std::cout << "2IYinst_ok: at (" << i << ", " << ic->key << ")\nIYL = (" << ia.registers[REG_IYL][0] << ", " << ia.registers[REG_IYL][1] << "), IYH = (" << ia.registers[REG_IYH][0] << ", " << ia.registers[REG_IYH][1] << ")inst " << i << ", " << ic->key << "\n";
#endif

  if(result_in_IY &&
    (ic->op == '=' ||
    ic->op == '+')) // todo: More instructions that can write iy.
    return(true);

  // Todo: Multiplication.

  if(ic->op == LEFT_OP && result_in_IY && input_in_IY && IS_VALOP (IC_RIGHT (ic)) && operandLitValue (IC_RIGHT (ic)) < 8)
    return(true);

  if(ic->op == '-' && result_in_IY && input_in_IY && IS_VALOP (IC_RIGHT (ic)) && operandLitValue (IC_RIGHT (ic)) < 4)
    return(true);

  if(SKIP_IC2(ic))
    return(true);

  if(!result_in_IY && !input_in_IY &&
    !(IC_RESULT(ic) && isOperandInDirSpace(IC_RESULT(ic))) &&
    !(IC_RIGHT(ic) && isOperandInDirSpace(IC_RIGHT(ic))) &&
    !(IC_LEFT(ic) && isOperandInDirSpace(IC_LEFT(ic))))
    return(true);

  if(!result_in_IY && !input_in_IY && ic->op == '=' && operand_is_pair(IC_RESULT(ic), a, i, G))	// DirSpace access won't use iy here.
    return(true);

  if(ic->op == IPUSH)	// todo: More instructions that can use IY.
    return(true);
    
  if(ic->op == GET_VALUE_AT_ADDRESS && isOperandInDirSpace(IC_RESULT(ic)))
    return(false);

  if(input_in_IY && !result_in_IY &&
    (ic->op == '=' && !POINTER_SET(ic) ||
     ic->op == GET_VALUE_AT_ADDRESS))
    return(true);

#if 0
  if(ic->key == 118)
    {
      std::cout << "Default drop.\n";
      std::cout << "result is pair: " << operand_is_pair(IC_RESULT(ic), a, i, G) << "\n";
    }
#endif

  return(false);
}

template <class G_t, class I_t>
void set_surviving_regs(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  iCode *ic = G[i].ic;
  
  ic->rSurv = newBitVect(NUM_REGS);
  
  std::set<var_t>::const_iterator v, v_end;
  for (v = G[i].alive.begin(), v_end = G[i].alive.end(); v != v_end; ++v)
    if(G[i].dying.find(*v) == G[i].dying.end())
      if(!(IC_RESULT(ic) && IS_SYMOP(IC_RESULT(ic)) && OP_SYMBOL_CONST(IC_RESULT(ic))->key == I[*v].v))
        ic->rSurv = bitVectSetBit(ic->rSurv, a.global[*v]);
}

template<class G_t>
void unset_surviving_regs(unsigned short int i, const G_t &G)
{
  iCode *ic = G[i].ic;
  
  freeBitVect(ic->rSurv);
}

template <class G_t, class I_t>
void assign_operand_for_cost(operand *o, const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  if(!o || !IS_SYMOP(o))
    return;
  symbol *sym = OP_SYMBOL(o);
  operand_map_t::const_iterator oi, oi_end;
  for(boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL_CONST(o)->key); oi != oi_end; ++oi)
    {
      var_t v = oi->second;
      if(a.global[v] >= 0)
        { 
          if((a.global[v] != REG_A || !OPTRALLOC_A) && (a.global[v] != REG_IYL && a.global[v] != REG_IYH || !OPTRALLOC_IY))
            {
              sym->regs[I[v].byte] = regsZ80 + a.global[v];
              sym->accuse = 0;
              sym->isspilt = false;
              sym->nRegs = I[v].size;
            }
          else if(a.global[v] == REG_A)
            {
              sym->accuse = ACCUSE_A;
              sym->isspilt = false;
              sym->nRegs = 0;
              sym->regs[I[v].byte] = 0;
            }
          else
            {
              sym->accuse = ACCUSE_IY;
              sym->isspilt = false;
              sym->nRegs = 0;
              sym->regs[I[v].byte] = 0;
            }
        }
      else
        {
          sym->isspilt = true;
          sym->accuse = 0;
          sym->nRegs = I[v].size;
          sym->regs[I[v].byte] = 0;
        }
    }
}

template <class G_t, class I_t>
void assign_operands_for_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  const iCode *ic = G[i].ic;
  
  if(ic->op == IFX)
    assign_operand_for_cost(IC_COND(ic), a, i, G, I);
  else if(ic->op == JUMPTABLE)
    assign_operand_for_cost(IC_JTCOND(ic), a, i, G, I);
  else
    {
      assign_operand_for_cost(IC_LEFT(ic), a, i, G, I);
      assign_operand_for_cost(IC_RIGHT(ic), a, i, G, I);
      assign_operand_for_cost(IC_RESULT(ic), a, i, G, I);
    }
    
  if(ic->op == SEND && ic->builtinSEND)
    {
      assign_operands_for_cost(a, *(adjacent_vertices(i, G).first), G, I);
    }
}

// Cost function.
template <class G_t, class I_t>
float instruction_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  iCode *ic = G[i].ic;
  float c;

  if(!inst_sane(a, i, G, I))
    return(std::numeric_limits<float>::infinity());

  if(OPTRALLOC_A && !Ainst_ok(a, i, G, I))
    return(std::numeric_limits<float>::infinity());

  if(OPTRALLOC_HL && !HLinst_ok(a, i, G, I))
    return(std::numeric_limits<float>::infinity());

  if(OPTRALLOC_IY && !IYinst_ok(a, i, G, I))
    return(std::numeric_limits<float>::infinity());

  if(OPTRALLOC_EXACT_COST)
    {
      switch(ic->op)
        {
        // Register assignment doesn't matter for these:
        case FUNCTION:
        case ENDFUNCTION:
        case LABEL:
        case GOTO:
        case INLINEASM:
          return(0.0f);
        // Exact cost:
        case '!':
        case '~':
        case UNARYMINUS:
        case '+':
        case '-':
        case '^':
        case '|':
        case BITWISEAND:
        case IPUSH:
        //case IPOP:
        case CALL:
        case PCALL:
        case RETURN:
        case '*':
        case '>':
        case '<':
        case EQ_OP:
        case AND_OP:
        case OR_OP:
        case GETHBIT:
        case LEFT_OP:
        case RIGHT_OP:
        case GET_VALUE_AT_ADDRESS:
        case '=':
        case IFX:
        case ADDRESS_OF:
        case JUMPTABLE:
        case CAST:
        //case RECEIVE:
        case SEND:
        case DUMMY_READ_VOLATILE:
        case CRITICAL:
        case ENDCRITICAL:
          assign_operands_for_cost(a, i, G, I);
          set_surviving_regs(a, i, G, I);
          c = dryZ80iCode(ic);
          unset_surviving_regs(i, G);
          return(c);
        // Inexact cost:
        default:
          return(default_instruction_cost(a, i, G, I));
        }
    }
  else
    {
      // Inexact cost function
      switch(ic->op)
        {
        // Register assignment doesn't matter for these
        case FUNCTION:
        case ENDFUNCTION:
        case LABEL:
        case GOTO:
        case INLINEASM:
          return(0.0f);
        case '=':
        case CAST:
          return(assign_cost(a, i, G, I));
        case RETURN:
          return(return_cost(a, i, G, I));
        case CALL:
          return(call_cost(a, i, G, I));
        case IFX:
          return(ifx_cost(a, i, G, I));
        case JUMPTABLE:
          return(jumptab_cost(a, i, G, I));
        default:
          return(default_instruction_cost(a, i, G, I));
        }
    }
}

template <class I_t>
float weird_byte_order(const assignment &a, const I_t &I) 
{
  float c = 0.0f;
  
  varset_t::const_iterator vi, vi_end;
  for(vi = a.local.begin(), vi_end = a.local.end(); vi != vi_end; ++vi)
    if(a.global[*vi] % 2 != I[*vi].byte % 2)
      c += 8.0f;

  return(c);
}

// Check for gaps, i.e. higher bytes of a variable being assigned to regs, while lower byte are not.
template <class I_t>
bool local_assignment_insane(const assignment &a, const I_t &I, var_t lastvar)
{
  varset_t::const_iterator v, v_end, v_old;
  
  for(v = a.local.begin(), v_end = a.local.end(); v != v_end;)
    {
      v_old = v;
      ++v;
      if(v == v_end)
        {
          if(*v_old != lastvar && I[*v_old].byte != I[*v_old].size - 1)
            return(true);
          break;
        }
      if(I[*v_old].v == I[*v].v)
        {
          if(I[*v_old].byte != I[*v].byte - 1)
            return(true);
        }
      else
        {
          if(*v_old != lastvar && I[*v_old].byte != I[*v_old].size - 1 || I[*v].byte)
            return(true);
        }
    }
	
  return(false);
}

// For early removel of assignments that cannot be extended to valid assignments.
template <class G_t, class I_t>
bool assignment_hopeless(const assignment &a, unsigned short int i, const G_t &G, const I_t &I, const var_t lastvar)
{
  // Can check for Ainst_ok() since A only contains 1-byte variables.
  if(OPTRALLOC_A && !Ainst_ok(a, i, G, I))
    return(true);

  if(local_assignment_insane(a, I, lastvar))
    return(true);

  const i_assignment_t &ia = a.i_assignment;

  // Code generator cannot handle variables that are only partially in IY.
  if(OPTRALLOC_IY &&
    (ia.registers[REG_IYL][1] >= 0 && (I[ia.registers[REG_IYL][1]].size != 2 || I[ia.registers[REG_IYL][1]].byte != 0) ||
    ia.registers[REG_IYH][1] >= 0 && (I[ia.registers[REG_IYH][1]].size != 2 || I[ia.registers[REG_IYH][1]].byte != 1) ||
    ia.registers[REG_IYL][0] >= 0 && (I[ia.registers[REG_IYL][0]].size != 2 || I[ia.registers[REG_IYL][0]].byte != 0) ||
    ia.registers[REG_IYH][0] >= 0 && (I[ia.registers[REG_IYH][0]].size != 2 || I[ia.registers[REG_IYH][0]].byte != 1)))
    return(true);

  // Can only check for HLinst_ok() in some cases.
  if(OPTRALLOC_HL &&
      (ia.registers[REG_L][1] >= 0 && ia.registers[REG_H][1] >= 0) &&
      (ia.registers[REG_L][0] >= 0 && ia.registers[REG_H][0] >= 0) &&
      !HLinst_ok(a, i, G, I))
    return(true);

  // Can only check for IYinst_ok() in some cases.
  if(OPTRALLOC_IY &&
      (ia.registers[REG_IYL][1] >= 0 && ia.registers[REG_IYH][1] >= 0) &&
      !((ia.registers[REG_IYL][0] >= 0) ^ (ia.registers[REG_IYH][0] >= 0)) &&
      !IYinst_ok(a, i, G, I))
    return(true);

  return(false);
}

template <class T_t>
void get_best_local_assignment_biased(assignment &a, typename boost::graph_traits<T_t>::vertex_descriptor t, const T_t &T)
{
  const assignment_list_t &alist = T[t].assignments;

  assignment_list_t::const_iterator ai, ai_end, ai_best;
  for(ai = ai_best = alist.begin(), ai_end = alist.end(); ai != ai_end; ++ai)
    {
      if(ai->s < ai_best->s)
        {
          varset_t::const_iterator vi, vi_end;
          for(vi = ai->local.begin(), vi_end = ai->local.end(); vi != vi_end; ++vi)
            if(OPTRALLOC_A && ai->global[*vi] == REG_A || OPTRALLOC_HL && (ai->global[*vi] == REG_H || ai->global[*vi] == REG_L) || OPTRALLOC_IY && (ai->global[*vi] == REG_IYH || ai->global[*vi] == REG_IYL))
              goto too_risky;
          ai_best = ai;
        }
too_risky:
      ;
    }
	
  a = *ai_best;
  
  std::set<var_t>::const_iterator vi, vi_end;
  for(vi = T[t].alive.begin(), vi_end = T[t].alive.end(); vi != vi_end; ++vi)
    a.local.insert(*vi);
}

template <class G_t, class I_t>
float rough_cost_estimate(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
  const i_assignment_t &ia = a.i_assignment;
    
  float c = 0.0f;

  c += weird_byte_order(a, I);

  if(OPTRALLOC_HL &&
    (ia.registers[REG_L][1] >= 0 && ia.registers[REG_H][1] >= 0) &&
    !((ia.registers[REG_L][0] >= 0) ^ (ia.registers[REG_H][0] >= 0)) &&
    !HLinst_ok(a, i, G, I))
    c += 8.0f;

  if(OPTRALLOC_A && ia.registers[REG_A][1] < 0)
    c += 0.03f;

  if(OPTRALLOC_HL && ia.registers[REG_L][1] < 0)
    c += 0.02f;

  // Using IY is rarely a good choice, so discard the IY-users first when in doubt.
  if(OPTRALLOC_IY)
    {
      varset_t::const_iterator vi, vi_end;
      for(vi = a.local.begin(), vi_end = a.local.end(); vi != vi_end; ++vi)
        if(a.global[*vi] == REG_IYL || a.global[*vi] == REG_IYH)
          c += 8.0f;
    }

  // An artifical ordering of assignments.
  if(ia.registers[REG_E][1] < 0)
    c += 0.0001f;
  if(ia.registers[REG_D][1] < 0)
    c += 0.00001f;

  if(a.marked)
    c -= 0.5f;

  varset_t::const_iterator v, v_end;
  for(v = a.local.begin(), v_end = a.local.end(); v != v_end; ++v)
    c -= *v * 0.01f;

  c -= a.local.size() * 0.2f;

  return(c);
}

template <class T_t, class G_t, class I_t>
void tree_dec_ralloc(T_t &T, const G_t &G, const I_t &I)
{
  con2_t I2(boost::num_vertices(I));
  for(unsigned int i = 0; i < boost::num_vertices(I); i++)
    {
      I2[i].v = I[i].v;
      I2[i].byte = I[i].byte;
      I2[i].size = I[i].size;
      I2[i].name = I[i].name;
    }
  typename boost::graph_traits<I_t>::edge_iterator e, e_end;
  for(boost::tie(e, e_end) = boost::edges(I); e != e_end; ++e)
    add_edge(boost::source(*e, I), boost::target(*e, I), I2);

  assignment ac;
  assignment_optimal = true;
  tree_dec_ralloc_nodes(T, find_root(T), G, I2, ac);

  const assignment &winner = *(T[find_root(T)].assignments.begin());

#ifdef DEBUG_RALLOC_DEC
  std::cout << "Winner: ";
  for(unsigned int i = 0; i < boost::num_vertices(I); i++)
  {
  	std::cout << "(" << i << ", " << int(winner.global[i]) << ") ";
  }
  std::cout << "\n";
  std::cout << "Cost: " << winner.s << "\n";
  std::cout.flush();
#endif

  // Todo: Make this an assertion
  if(winner.global.size() != boost::num_vertices(I))
    {
      std::cerr << "ERROR: No Assignments at root\n";
      exit(-1);
    }

  for(unsigned int v = 0; v < boost::num_vertices(I); v++)
    {
      symbol *sym = (symbol *)(hTabItemWithKey(liveRanges, I[v].v));
      if(winner.global[v] >= 0)
        {
          if((winner.global[v] != REG_A || !OPTRALLOC_A) && (winner.global[v] != REG_IYL && winner.global[v] != REG_IYH || !OPTRALLOC_IY))
            {
              sym->regs[I[v].byte] = regsZ80 + winner.global[v];
              sym->accuse = 0;
              sym->isspilt = false;
              sym->nRegs = I[v].size;
            }
          else if(winner.global[v] == REG_A)
            {
              sym->accuse = ACCUSE_A;
              sym->isspilt = false;
              sym->nRegs = 0;
              sym->regs[0] = 0;
            }
          else
            {
              sym->accuse = ACCUSE_IY;
              sym->isspilt = false;
              sym->nRegs = 0;
              sym->regs[I[v].byte] = 0;
            }
        }
      else
        {
          for(int i = 0; i < I[v].size; i++)
            sym->regs[i] = 0;
          sym->accuse = 0;
          sym->nRegs = I[v].size;
          //spillThis(sym); Leave it to regFix, which can do some spillocation compaction. Todo: Use Thorup instead.
          sym->isspilt = false;
        }
    }
    
  for(unsigned int i = 0; i < boost::num_vertices(G); i++)
    set_surviving_regs(winner, i, G, I);	// Never freed. Memory leak?
}

iCode *z80_ralloc2_cc(ebbIndex *ebbi)
{
  iCode *ic;

#ifdef DEBUG_RALLOC_DEC
  std::cout << "Processing " << currFunc->name << " from " << dstFileName << "\n"; std::cout.flush();
#endif

  cfg_t control_flow_graph;

  con_t conflict_graph;

  ic = create_cfg(control_flow_graph, conflict_graph, ebbi);

  if(z80_opts.dump_graphs)
    dump_cfg(control_flow_graph);

  if(z80_opts.dump_graphs)
    dump_con(conflict_graph);

  tree_dec_t tree_decomposition;

  thorup_tree_decomposition(tree_decomposition, control_flow_graph);

  nicify(tree_decomposition);

  alive_tree_dec(tree_decomposition, control_flow_graph);

  good_re_root(tree_decomposition);
  nicify(tree_decomposition);
  alive_tree_dec(tree_decomposition, control_flow_graph);

  if(z80_opts.dump_graphs)
    dump_tree_decomposition(tree_decomposition);

  tree_dec_ralloc(tree_decomposition, control_flow_graph, conflict_graph);

  return(ic);
}

