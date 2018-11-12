// Philipp Klaus Krause, philipp@informatik.uni-frankfurt.de, pkk@spth.de, 2011-2018
//
// (c) 2011-2012 Goethe-Universität Frankfurt
// (c) 2018 Albert-Ludwigs-Universität Frankfurt
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
// A Chaitin-style stack allocator.

#ifndef SDCCSALLOC_HH
#define SDCCSALLOC_HH 1

#include <boost/graph/adjacency_list.hpp>

#include <boost/icl/discrete_interval.hpp>
#include <boost/icl/interval_set.hpp>

extern "C"
{
#include "SDCCmem.h"
#include "SDCCglobl.h"
}

// #define DEBUG_SALLOC

struct scon_node
{
  symbol *sym;
  int color;
  boost::icl::interval_set<int> free_stack;
};

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, scon_node> scon_t;

static bool clash (const symbol *s1, const symbol *s2)
{
  wassert(s1);
  wassert(s2);

  if(!s1->isspilt && !(IS_AGGREGATE(s1->type) || s1->allocreq && (s1->addrtaken || isVolatile(s1->type)))) // Spill location
    {
      for(const symbol *s = (const symbol *)setFirstItem (s1->usl.itmpStack); s; s = (const symbol *)setNextItem (s1->usl.itmpStack))
        if(clash(s, s2))
           return(true);
      return(false);
    }
  if(!s2->isspilt && !(IS_AGGREGATE(s2->type) || s2->allocreq && (s2->addrtaken || isVolatile(s2->type)))) // Spill location
    {
      for(const symbol *s = (const symbol *)setFirstItem (s2->usl.itmpStack); s; s = (const symbol *)setNextItem (s2->usl.itmpStack))
        if(clash(s1, s))
           return(true);
      return(false);
    }

  return(bitVectBitValue (s1->clashes, s2->key));
}

template<class G_t, class I_t, class SI_t>
static void set_spilt(G_t &G, const I_t &I, SI_t &scon)
{
  std::map<int, var_t> symbol_to_sindex;
  symbol *sym;
  var_t j, j_mark;

  // Add variables that need to be on the stack due to having had their address taken (or for a few other reasons, such as being too large or too many to behandled by the register allocator).
  for(sym = static_cast<symbol *>(setFirstItem(istack->syms)), j = 0; sym; sym = static_cast<symbol *>(setNextItem(istack->syms)))
    {
      if(sym->_isparm)
        continue;
        
      if(/*!(IS_AGGREGATE(sym->type) || sym->allocreq && (sym->addrtaken || isVolatile(sym->type)))*/sym->for_newralloc)
        continue;

      if(!sym->isspilt && !(IS_AGGREGATE(sym->type) || sym->allocreq && (sym->addrtaken || isVolatile(sym->type)))) // Looks like a spill location - check if it is already covered by live ranges below.
        {
          bool covered = true;
          for (const symbol *s = (const symbol *)setFirstItem (sym->usl.itmpStack); s; s = (const symbol *)setNextItem (sym->usl.itmpStack))
            if (!s->for_newralloc)
              {
#ifdef DEBUG_SALLOC  
                std::cout << "Adding " << sym->name << " for " << s->name << " to be allocated to stack. (" << s->for_newralloc << ")\n",
#endif
                covered = false;
                break;
              }
          if(covered)
            continue;
        }
      
      boost::add_vertex(scon);
      scon[j].sym = sym;
      scon[j].color = -1;
      j++;

      // std::cout << "Symbol " << sym->name << " needs stack space.\n";
    }
  j_mark = j;
  
  // Add edges due to scope (see C99 standard, verse 1233, which requires things to have different addresses, not allowing us to allocate them to the same location, even if we otherwise could).
  for(unsigned int i = 0; i < boost::num_vertices(scon); i++)
     for(unsigned int j = i + 1; j < boost::num_vertices(scon); j++)
        {
          if (!(scon[i].sym->addrtaken) || !(scon[i].sym->addrtaken))
            continue;
          short p = btree_lowest_common_ancestor(scon[i].sym->block, scon[j].sym->block);
          if(p == scon[i].sym->block || p == scon[j].sym->block)
            boost::add_edge(i, j, scon);
        }

  // Set stack live ranges
  for(unsigned int i = 0; i < boost::num_vertices(G); i++)
    {
      for(unsigned int j = 0; j < boost::num_vertices(scon); j++)
        {
          short p = btree_lowest_common_ancestor(G[i].ic->block, scon[j].sym->block);
          if(p == G[i].ic->block || p == scon[j].sym->block)
            G[i].stack_alive.insert(j);
        }
    }

  // Add variables that have been spilt in register allocation.
  for(unsigned int i = 0; i < boost::num_vertices(G); i++)
    {
      cfg_alive_t::const_iterator v, v_end;
      for (v = G[i].alive.begin(), v_end = G[i].alive.end(); v != v_end; ++v)
        {
          var_t vs;

          symbol *const sym = (symbol *)(hTabItemWithKey(liveRanges, I[*v].v));

          if (sym->regs[0] || sym->accuse || sym->remat || !sym->nRegs || sym->usl.spillLoc && sym->usl.spillLoc->_isparm)
            continue;

          if (symbol_to_sindex.find(I[*v].v) == symbol_to_sindex.end())
            {
              boost::add_vertex(scon);
              scon[j].sym = sym;
              scon[j].color = -1;
              symbol_to_sindex[I[*v].v] = j;
              j++;
            }

          vs = symbol_to_sindex[I[*v].v];
        
          G[i].stack_alive.insert(vs); // Needs to be allocated on the stack.
        }
    }

  // Add edges to conflict graph.
  typename boost::graph_traits<I_t>::edge_iterator e, e_end;
  for (boost::tie(e, e_end) = boost::edges(I); e != e_end; ++e)
    {
      if (I[boost::source(*e, I)].v == I[boost::target(*e, I)].v || symbol_to_sindex.find(I[boost::source(*e, I)].v) == symbol_to_sindex.end() || symbol_to_sindex.find(I[boost::target(*e, I)].v) == symbol_to_sindex.end())
        continue;
        
      boost::add_edge(symbol_to_sindex[I[boost::source(*e, I)].v], symbol_to_sindex[I[boost::target(*e, I)].v], scon);
    }
    
  // Add conflicts between variables that had their address taken and those that have been spilt by register allocation.
  // TODO: More exact live range analysis for variables that had their address taken (to reduce stack space consumption further, by reducing the number of conflicts here).
  for(unsigned int i = 0; i < j_mark; i++)
    for(unsigned int j = 0; j < boost::num_vertices(scon); j++)
      {
        if (i == j)
          continue;
        if(!scon[i].sym->isspilt && !(IS_AGGREGATE(scon[i].sym->type) || scon[i].sym->allocreq && (scon[i].sym->addrtaken || isVolatile(scon[i].sym->type)))) // Spill location
          {
            if (clash (scon[i].sym, scon[j].sym))
              boost::add_edge(i, j, scon);
            continue;
          }
        short p = btree_lowest_common_ancestor(scon[i].sym->block, scon[j].sym->block);
        if(p == scon[i].sym->block || p == scon[j].sym->block)
          boost::add_edge(i, j, scon);
      }

  // Ugly hack: Regparms.
  for(sym = static_cast<symbol *>(setFirstItem(istack->syms)), j = boost::num_vertices(scon); sym; sym = static_cast<symbol *>(setNextItem(istack->syms)))
    {
      if(!sym->_isparm || !IS_REGPARM(sym->etype) || !sym->onStack || !sym->allocreq)
        continue;
      
      boost::add_vertex(scon);
      scon[j].sym = sym;
      scon[j].color = -1;

      // Extend liverange to cover everything.
      for(unsigned int i = 0; i < boost::num_vertices(G); i++)
        G[i].stack_alive.insert(j);

      // Conflict with everything.
      for(unsigned int i = 0; i < j; i++)
        boost::add_edge(i, j, scon);

      j++;
    }
}

template <class SI_t>
void color_stack_var(const var_t v, SI_t &SI, int start, int *ssize)
{
  symbol *const sym = SI[v].sym;
  const int size = getSize(sym->type);
  
  SI[v].color = start;

  if(sym->isspilt && sym->usl.spillLoc)
    SPEC_STAK(sym->usl.spillLoc->etype) = sym->usl.spillLoc->stack = (port->stack.direction > 0) ? start + 1 : -start - size;
  else
    SPEC_STAK(sym->etype) = sym->stack = (port->stack.direction > 0) ? start + 1 : -start - size;
    
  if(ssize)
    *ssize = (start + size > *ssize) ? start + size : *ssize;

#ifdef DEBUG_SALLOC    
  std::cout << "Placing " << sym->name << " at [" << start << ", " << (start + size - 1) << "]\n";
#endif
    
  // Mark stack location as used for all conflicting variables.
  typename boost::graph_traits<SI_t>::adjacency_iterator n, n_end;
  for(boost::tie(n, n_end) = boost::adjacent_vertices(v, SI); n != n_end; ++n)
    SI[*n].free_stack -= boost::icl::discrete_interval<int>::type(start, start + size);
}

// Place a single variable on the stack greedily.
template <class SI_t>
void color_stack_var_greedily(const var_t v, SI_t &SI, int alignment, int *ssize)
{
  int start;
  symbol *const sym = SI[v].sym;
  const int size = getSize(sym->type);
 
  // Find a suitable free stack location.
  boost::icl::interval_set<int>::iterator si;
  for(si = SI[v].free_stack.begin();; ++si)
    {
       start = boost::icl::first(*si);
       
       // Adjust start address for alignment
       if(start % alignment)
         start = start + alignment - start % alignment;
                    
       if(boost::icl::last(*si) >= start + size - 1)
         break; // Found one.
    }
    
  color_stack_var(v, SI, start, ssize);
}

template <class SI_t>
void chaitin_ordering(const SI_t &SI, std::list<var_t> &ordering)
{
  std::vector<bool> marked(boost::num_vertices(SI));
  unsigned int num_marked, i, d, mind, minn;
  std::stack<var_t> stack;
  
  for(num_marked = 0; num_marked < boost::num_vertices(SI); num_marked++)
    {
      mind = UINT_MAX;
      minn = -1;
      for(i = 0; i < boost::num_vertices(SI); i++)
        {
          if(marked[i])
            continue;
          
          typename boost::graph_traits<const SI_t>::adjacency_iterator n, n_end;
          for(boost::tie(n, n_end) = boost::adjacent_vertices(i, SI), d = 0; n != n_end; ++n)
             d += !marked[*n];
             
          if(d < mind)
            {
              mind = d;
              minn = i;
            }
        }
        
      stack.push(minn);
      marked[minn] = true;
    }
    
  while(!stack.empty())
    {
      ordering.push_back(stack.top());
      stack.pop();
    }
}

template <class SI_t>
void chaitin_salloc(SI_t &SI)
{
  std::list<var_t> ordering;
  
  chaitin_ordering(SI, ordering);
  
  for(unsigned int i = 0; i < boost::num_vertices(SI); i++)
      SI[i].free_stack.insert(boost::icl::discrete_interval<int>::type(0, 1 << 15));
    
  int ssize = 0;
  
  clearStackOffsets();
  
  std::list<var_t>::const_iterator i, i_end;
  for(i = ordering.begin(), i_end = ordering.end(); i != i_end; ++i)
    color_stack_var_greedily(*i, SI, (getSize(SI[*i].sym->type) == 2 || getSize(SI[*i].sym->type) == 4 || getSize(SI[*i].sym->type) == 8) ? getSize(SI[*i].sym->type) : 1, &ssize); // Todo: Allow unaligned - need to implement better handling of coalescing first (including conditional conflicts on assignment) for Z80: Allowing unaligned on long long possible (i.e. eliminating == 8 from list above) if genAssign can use lddr in placec of ldir.
  
  if(currFunc)
    {
      currFunc->stack += ssize;
      SPEC_STAK (currFunc->etype) += ssize;
    }
}

static
void dump_scon(const scon_t &scon)
{
  if(!currFunc)
    return;

  std::ofstream dump_file((std::string(dstFileName) + ".dumpsalloccon" + currFunc->rname + ".dot").c_str());

  std::string *name = new std::string[boost::num_vertices(scon)];
  for(var_t i = 0; static_cast<boost::graph_traits<scon_t>::vertices_size_type>(i) < boost::num_vertices(scon); i++)
    {
      int start = scon[i].color;
      std::ostringstream os;
      os << i;
      if (scon[i].sym->name)
        os << " : " << scon[i].sym->name << " : " << getSize(scon[i].sym->type) << " [" << start << "," << (start + getSize(scon[i].sym->type) - 1) << "]";
      name[i] = os.str();
    }
  boost::write_graphviz(dump_file, scon, boost::make_label_writer(name));
  delete[] name;
}
#endif

