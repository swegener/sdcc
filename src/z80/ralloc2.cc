// Philipp Klaus Krause, philipp@informatik.uni-frankfurt.de, 2010 - 2011

#include <iostream>
#include <limits>
#include <utility>
#include <sstream>
#include <fstream>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/connected_components.hpp>

#include "tree_dec.hh"

extern "C"
{
#include "sdccconf.h"
#include "SDCCsymt.h"
#include "SDCCicode.h"
#include "SDCCBBlock.h"
#include "SDCCy.h"

#include "z80.h"
#include "ralloc.h"
}

struct assignment
{
	float s;

	std::map<unsigned short int, unsigned short int> local;	// Entries: (var, reg)
	std::vector<signed char> global;	// Entries: gloabal[var] = reg (-1 if no reg assigned)

	bool operator<(const assignment& a) const
	{
		std::map<unsigned short int, unsigned short int>::const_iterator i, ai, i_end, ai_end;

		i_end = local.end();
		ai_end = a.local.end();

		for(i = local.begin(), ai = a.local.begin();; ++i, ++ai)
		{
			if(i == i_end)
				return(true);
			if(ai == ai_end)
				return(false);

			if(i->first < ai->first)
				return(true);
			if(i->first > ai->first)
				return(false);

			if(i->second < ai->second)
				return(true);
			if(i->second > ai->second)
				return(false);
		}
	}
};

struct tree_dec_node
{
	std::set<unsigned int> bag;
	std::set<unsigned short int> alive;
	std::list<assignment> assignments;
};

struct cfg_node
{
	iCode *ic;
	std::multimap<int, unsigned short int> operands;
	std::set<unsigned short int> alive;
};

struct con_node
{
	int v;
	int byte;
	char *name;
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, tree_dec_node> tree_dec_t;
typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, con_node> con_t;
typedef boost::adjacency_matrix<boost::undirectedS, con_node> con2_t;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, cfg_node> cfg_t;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> cfg_sym_t;

void add_operand_to_cfg_node(cfg_node &n, operand *o, std::map<std::pair<int, int>, unsigned int> &sym_to_index)
{
	unsigned int k;
	if(o && IS_SYMOP(o) && sym_to_index.find(std::pair<int, int>(OP_SYMBOL(o)->key, 0)) != sym_to_index.end())
	{
		if(n.operands.find(OP_SYMBOL(o)->key) == n.operands.end())
			for(k = 0; k < OP_SYMBOL(o)->nRegs; k++)
				n.operands.insert(std::pair<int, unsigned short int>(OP_SYMBOL(o)->key, sym_to_index[std::pair<int, int>(OP_SYMBOL(o)->key, k)]));
	}
}

// A quick-and-dirty function to get the CFG from sdcc.
void create_cfg(cfg_t &cfg, con_t &con, ebbIndex *ebbi)
{
	eBBlock **ebbs = ebbi->bbOrder;
	iCode *start_ic;
	iCode *ic;
	unsigned int i, j, k, l;

	std::map<int, unsigned int> key_to_index;
	std::map<std::pair<int, int>, unsigned int> sym_to_index;

	start_ic = iCodeLabelOptimize(iCodeFromeBBlock (ebbs, ebbi->count));
	//start_ic = joinPushes(start_ic);

	for(ic = start_ic, i = 0, j = 0; ic; ic = ic->next, i++)
	{
		boost::add_vertex(cfg);
		key_to_index[ic->key] = i;

		cfg[i].ic = ic;

		for(unsigned int j2 = 0; j2 <= operandKey; j2++)
		{
			if(bitVectBitValue(ic->rlive, j2))
			{
				symbol *sym = (symbol *)(hTabItemWithKey(liveRanges, j2));

				if(!sym->for_newralloc)
					continue;

				// Add node to conflict graph:
				if(sym_to_index.find(std::pair<int, int>(j2, 0)) != sym_to_index.end())
					continue;

				for(k = 0; k < sym->nRegs; k++)
				{
					boost::add_vertex(con);
					con[j].v = j2;
					con[j].byte = k;
					con[j].name = sym->name;
					sym_to_index[std::pair<int, int>(j2, k)] = j;
					for(l = 0; l < k; l++)
						boost::add_edge(j - l - 1, j, con);
					j++;
				}
			}
		}		
	}
		
	// Get control flow graph from sdcc.
	for(ic = start_ic; ic; ic = ic->next)
	{
		if(ic->op != GOTO && ic->op != RETURN && ic->op != JUMPTABLE && ic->next)
			boost::add_edge(key_to_index[ic->key], key_to_index[ic->next->key], cfg);

		if(ic->op == GOTO)
			boost::add_edge(key_to_index[ic->key], key_to_index[eBBWithEntryLabel(ebbi, ic->label)->sch->key], cfg);
		else if(ic->op == RETURN)
			boost::add_edge(key_to_index[ic->key], key_to_index[eBBWithEntryLabel(ebbi, returnLabel)->sch->key], cfg);
		else if(ic->op == IFX)
			boost::add_edge(key_to_index[ic->key], key_to_index[eBBWithEntryLabel(ebbi, IC_TRUE(ic) ? IC_TRUE(ic) : IC_FALSE(ic))->sch->key], cfg);
		else if(ic->op == JUMPTABLE)
			for(symbol *lbl = (symbol *)(setFirstItem (IC_JTLABELS (ic))); lbl; lbl = (symbol *)(setNextItem (IC_JTLABELS (ic))))
				boost::add_edge(key_to_index[ic->key], key_to_index[eBBWithEntryLabel(ebbi, lbl)->sch->key], cfg);

		for(int i = 0; i <= operandKey; i++)
		{
			if(sym_to_index.find(std::pair<int, int>(i, 0)) == sym_to_index.end())
				continue;

			if(bitVectBitValue(ic->rlive, i))
			{
				symbol *isym = (symbol *)(hTabItemWithKey(liveRanges, i));
				for(k = 0; k < isym->nRegs; k++)
					cfg[key_to_index[ic->key]].alive.insert(sym_to_index[std::pair<int, int>(i, k)]);
			}
		}

		add_operand_to_cfg_node(cfg[key_to_index[ic->key]], IC_RESULT(ic), sym_to_index);
		add_operand_to_cfg_node(cfg[key_to_index[ic->key]], IC_LEFT(ic), sym_to_index);
		add_operand_to_cfg_node(cfg[key_to_index[ic->key]], IC_RIGHT(ic), sym_to_index);
	}

	// Get conflict graph from sdcc
	for(i = 0; i < num_vertices(con); i++)
	{
		symbol *isym = (symbol *)(hTabItemWithKey(liveRanges, con[i].v));
		for(j = 0; j <= operandKey; j++)
			if(bitVectBitValue(isym->clashes, j))
			{
				symbol *jsym = (symbol *)(hTabItemWithKey(liveRanges, j));
				if(sym_to_index.find(std::pair<int, int>(j, 0)) == sym_to_index.end())
					continue;
				for(k = 0; k < isym->nRegs; k++)
					boost::add_edge(i, sym_to_index[std::pair<int, int>(j, k)], con);
			}
	}

	// Check for unconnected live ranges, some might have survived dead code elimination.
	// Todo: Improve efficiency, e.g. using subgraph or filtered_graph.
	// Todo: Split live ranges instead?
	for(int i = boost::num_vertices(con) - 1; i >= 0; i--)
	{
		cfg_sym_t cfg2;
		boost::copy_graph(cfg, cfg2);
		for(int j = boost::num_vertices(cfg) - 1; j >= 0; j--)
		{
			if(cfg[j].alive.find(i) == cfg[j].alive.end())
			{
				boost::clear_vertex(j, cfg2);
				boost::remove_vertex(j, cfg2);
			}
		}
		std::vector<int> component(num_vertices(cfg2));
		if(boost::connected_components(cfg2, &component[0]) > 1)
		{
			//std::cerr << "Non-connected liverange found and spilt:" << con[i].name << "\n";
			for(k = 0; k < boost::num_vertices(cfg) - 1; k++)
				cfg[k].alive./*insert*/erase(i);
		}
	}
}

// Computes live ranges for tree decomposition from live ranges from cfg.
void alive_tree_dec(tree_dec_t &tree_dec, const cfg_t &cfg)
{
	for(unsigned int i = 0; i < num_vertices(tree_dec); i++)
	{
		std::set<unsigned int>::iterator v;
		for(v = tree_dec[i].bag.begin(); v != tree_dec[i].bag.end(); ++v)
			tree_dec[i].alive.insert(cfg[*v].alive.begin(), cfg[*v].alive.end());
	}
}

#define REG_C 0
#define REG_B 1
#define REG_E 2
#define REG_D 3
#define NUM_REGS (IS_GB ? 2 : 4)

void print_assignment(const assignment &a)
{
	std::map<unsigned short int, unsigned short int>::const_iterator i;
	std::cout << "[";
	for(i = a.local.begin(); i != a.local.end(); ++i)
		std::cout << "(" << i->first << ", " << i->second << "), ";
	std::cout << "c: " << a.s << "]";
}

template <class I_t>
bool assignment_ok_conflict(const assignment &a, const I_t &I)
{
	std::map<unsigned short int, unsigned short int>::const_iterator i, j;

	for(i = a.local.begin(); i != a.local.end(); ++i)
		for(j = i, ++j; j != a.local.end(); ++j)
		{
			// Different registers - always OK.
			if(i->second != j->second)
				continue;

			// Same registers - check for conflict.
			if(boost::edge(i->first, j->first, I).second)
				return(false);
		}
	return(true);
}

template <class I_t>
void assignment_introduce_variable(std::list<assignment> &alist, unsigned short int v, const I_t &I)
{
	std::list<assignment>::iterator ai, ai_end, ai2;

	for(ai = alist.begin(), ai_end = alist.end(); ai != ai_end; ai = ai2)
	{
		ai2 = ai;
		++ai2;
		
		assignment a = *ai;

		unsigned short int r;
		for(r = 0; r < NUM_REGS; r++)
		{
			a.local[v] = r;

			if(assignment_ok_conflict(a, I))
				alist.insert(ai2, a);
		}
	}
}

template <class G_t, class I_t>
float default_operand_cost(operand *o, const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
	float c = 0.0f;

	std::map<int, unsigned short int>::const_iterator oi, oi_end;
	std::map<unsigned short int, unsigned short int>::const_iterator vi;

	unsigned short int byteregs[4];	// Todo: Change this when sdcc supports variables larger than 4 bytes.
	unsigned short int size;

	if(o && IS_SYMOP(o))
	{
		boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL(o)->key);
		if(oi != oi_end)
		{
			unsigned short int v = oi->second;

			// In registers.
			if((vi = a.local.find(v)) != a.local.end())
			{
				c += 1.0f;
				byteregs[I[v].byte] = vi->second;
				size = 1;

				while(++oi != oi_end)
				{
					v = oi->second;
					c += ((vi = a.local.find(v)) != a.local.end() ? 1.0f : std::numeric_limits<float>::infinity());
					byteregs[I[v].byte] = vi->second;
					size++;
				}

				// Penalty for not placing 2- and 4-byte variables in register pairs
				// Todo: Extend this once the register allcoator can use registers other than bc, de:
				if((size == 2 || size == 4) && (byteregs[1] != byteregs[0] + 1 || byteregs[0] != REG_C && byteregs[0] != REG_E))
					c += 2.0f;
				if(size == 4 && (byteregs[3] != byteregs[2] + 1 || byteregs[2] != REG_C && byteregs[2] != REG_E))
					c += 2.0f;
			}
			// Spilt.
			else
			{
				c += 4.0f;
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

template <class G_t, class I_t>
float default_instruction_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
	float c = 0.0f;

	const iCode *ic = G[i].ic;

	unsigned short int v;

	c += default_operand_cost(IC_RESULT(ic), a, i, G, I);
	c += default_operand_cost(IC_LEFT(ic), a, i, G, I);
	c += default_operand_cost(IC_RIGHT(ic), a, i, G, I);
	
	return(c);
}

// Treat assignment separately to handle coalescing.
template <class G_t, class I_t>
float assign_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
	float c = 0.0f;
	
	const iCode *ic = G[i].ic;
	
	unsigned short int v;

	operand *right = IC_RIGHT(ic);
	operand *result = IC_RESULT(ic);
	
	if(!right || !IS_SYMOP(right) || !result || !IS_SYMOP(result))
		return(default_instruction_cost(a, i, G, I));

	signed char byteregs[4] = {-1, -1, -1, -1};	// Todo: Change this when sdcc supports variables larger than 4 bytes.
	
	std::map<int, unsigned short int>::const_iterator oi, oi_end;
	std::map<unsigned short int, unsigned short int>::const_iterator vi;

	int size1 = 0, size2 = 0;
	
	boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL(right)->key);
	if(oi != oi_end)
	{
		unsigned short int v = oi->second;

		if((vi = a.local.find(v)) == a.local.end())
			return(default_instruction_cost(a, i, G, I));

		c += 1.0f;
		byteregs[I[v].byte] = vi->second;
		size1 = 1;

		while(++oi != oi_end)
		{
			v = oi->second;
			c += ((vi = a.local.find(v)) != a.local.end() ? 1.0f : std::numeric_limits<float>::infinity());
			byteregs[I[v].byte] = vi->second;
			size1++;
		}
	}
	
	if(!size1)
		return(default_instruction_cost(a, i, G, I));
	
	boost::tie(oi, oi_end) = G[i].operands.equal_range(OP_SYMBOL(result)->key);
	if(oi != oi_end)
	{
		unsigned short int v = oi->second;

		if((vi = a.local.find(v)) == a.local.end())
			return(default_instruction_cost(a, i, G, I));

		c += 1.0f;
		if(byteregs[I[v].byte] == vi->second)
			c -= 2.0f;
		size2 = 1;

		while(++oi != oi_end)
		{
			v = oi->second;
			c += ((vi = a.local.find(v)) != a.local.end() ? 1.0f : std::numeric_limits<float>::infinity());
			if(byteregs[I[v].byte] == vi->second)
				c -= 2.0f;
			size2++;
		}
	}
	
	if(!size2)
		return(default_instruction_cost(a, i, G, I));

	return(c);
}

template <class G_t, class I_t>
float ifx_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
	const iCode *ic = G[i].ic;
	
	return(default_operand_cost(IC_COND(ic), a, i, G, I));
}

template <class G_t, class I_t>
float jumptab_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
	const iCode *ic = G[i].ic;
	
	return(default_operand_cost(IC_JTCOND(ic), a, i, G, I));
}

// Cost function.
template <class G_t, class I_t>
float instruction_cost(const assignment &a, unsigned short int i, const G_t &G, const I_t &I)
{
	const iCode *ic = G[i].ic;

	switch(ic->op)
	{
	case '=':
		return(assign_cost(a, i, G, I));
	case IFX:
		return(ifx_cost(a, i, G, I));
	case JUMPTABLE:
		return(jumptab_cost(a, i, G, I));
	default:
		return(default_instruction_cost(a, i, G, I));
	}
}

// Handle Leaf nodes in the nice tree decomposition
template <class T_t, class G_t, class I_t>
void tree_dec_ralloc_leaf(T_t &T, typename boost::graph_traits<T_t>::vertex_descriptor t, const G_t &G, const I_t &I)
{
	//std::cout << "Leaf:\n";

	assignment a;
	std::list<assignment> &alist = T[t].assignments;

	a.s = 0;
	a.global.resize(boost::num_vertices(I), -1);
	alist.push_back(a);

	std::set<unsigned short int>::iterator v;
	for(v = T[t].alive.begin(); v != T[t].alive.end(); ++v)
		assignment_introduce_variable(alist, *v, I);

	// Summation of costs and early removal of assignments.
	unsigned short int i = *(T[t].bag.begin());
	std::list<assignment>::iterator ai;
	for(ai = alist.begin(); ai != alist.end();)
	{
		if((ai->s += instruction_cost(*ai, i, G, I)) == std::numeric_limits<float>::infinity())
			ai = alist.erase(ai);
		else
			++ai;
	}

	/*std::list<assignment>::iterator ai;
	for(ai = alist.begin(); ai != alist.end(); ++ai)
		print_assignment(*ai);
	std::cout << "\n";*/
}

struct assignment_rep
{
	std::list<assignment>::iterator i;
	float s;

	bool operator<(const assignment_rep& a) const
	{
		return(s < a.s);
	}
};

// Ensure that we never get more than z80_opts.max_allocs_per_node assignments at a single node of the tree decomposition.
// Tries to drop the worst ones first (but never drop the empty assignment, as it's the only one guaranteed to be always valid).
void drop_worst_assignments(std::list<assignment> &alist)
{
	unsigned int i;
	size_t alist_size;
	std::list<assignment>::iterator ai, an;

	if((alist_size = alist.size()) * NUM_REGS <= z80_opts.max_allocs_per_node)
		return;

	//std::cerr << "Too many assignments here:" << z80_opts.max_allocs_per_node << "Dropping some.\n";

	assignment_rep arep[alist_size];

	for(i = 0, ai = alist.begin(); i < alist_size; ++ai, i++)
	{
		arep[i].i = ai;
		arep[i].s = ai->s;
	}

	std::nth_element(arep + 1, arep + z80_opts.max_allocs_per_node / NUM_REGS, arep + alist_size);

	for(i = z80_opts.max_allocs_per_node / NUM_REGS; i < alist_size; i++)
		alist.erase(arep[i].i);
}

// Handle introduce nodes in the nice tree decomposition
template <class T_t, class G_t, class I_t>
void tree_dec_ralloc_introduce(T_t &T, typename boost::graph_traits<T_t>::vertex_descriptor t, const G_t &G, const I_t &I)
{
	typedef typename boost::graph_traits<T_t>::adjacency_iterator adjacency_iter_t;
	adjacency_iter_t c, c_end;
	std::list<assignment>::iterator ai;
	boost::tie(c, c_end) = adjacent_vertices(t, T);

	//std::cout << "Introduce:\n";

	std::list<assignment> &alist = T[t].assignments;

	std::swap(alist, T[*c].assignments);

	std::set<unsigned short> new_vars;
	std::set_difference(T[t].alive.begin(), T[t].alive.end(), T[*c].alive.begin(), T[*c].alive.end(), std::inserter(new_vars, new_vars.end()));

	std::set<unsigned short int>::iterator v;
	for(v = new_vars.begin(); v != new_vars.end(); ++v)
	{
		drop_worst_assignments(alist);
		assignment_introduce_variable(alist, *v, I);
	}

	// Summation of costs and early removal of assignments.
	std::set<unsigned short> new_inst;
	std::set_difference(T[t].bag.begin(), T[t].bag.end(), T[*c].bag.begin(), T[*c].bag.end(), std::inserter(new_inst, new_inst.end()));
	unsigned short int i = *(new_inst.begin());

	for(ai = alist.begin(); ai != alist.end();)
	{
		if((ai->s += instruction_cost(*ai, i, G, I)) == std::numeric_limits<float>::infinity())
			ai = alist.erase(ai);
		else
			++ai;
	}

	/*std::list<assignment>::iterator ai;
	for(ai = alist1.begin(); ai != alist1.end(); ++ai)
		print_assignment(*ai);
	std::cout << "\n";*/
}

// Handle forget nodes in the nice tree decomposition
template <class T_t, class G_t, class I_t>
void tree_dec_ralloc_forget(T_t &T, typename boost::graph_traits<T_t>::vertex_descriptor t, const G_t &G, const I_t &I)
{
	typedef typename boost::graph_traits<T_t>::adjacency_iterator adjacency_iter_t;
	adjacency_iter_t c, c_end;
	boost::tie(c, c_end) = adjacent_vertices(t, T);

	//std::cout << "Forget: " << t << "\n";

	std::list<assignment> &alist = T[t].assignments;

	std::swap(alist, T[*c].assignments);

	std::set<unsigned short> old_inst;
	std::set_difference(T[*c].bag.begin(), T[*c].bag.end(), T[t].bag.begin(), T[t].bag.end(), std::inserter(old_inst, old_inst.end()));
	unsigned short int i = *(old_inst.begin());

	std::set<unsigned short> old_vars;
	std::set_difference(T[*c].alive.begin(), T[*c].alive.end(), T[t].alive.begin(), T[t].alive.end(), std::inserter(old_vars, old_vars.end()));

	std::list<assignment>::iterator ai, aif;

	// Restrict assignments (locally) to current variables.
	for(ai = alist.begin(); ai != alist.end(); ++ai)
	{
		std::map<unsigned short int, unsigned short int>::iterator mi, mi2, m_end;
		for(mi = ai->local.begin(), m_end = ai->local.end(); mi != m_end; mi = mi2)
		{
			mi2 = mi;
			++mi2;
			if(old_vars.find(mi->first) != old_vars.end())
			{
				ai->global[mi->first] = mi->second;
				ai->local.erase(mi);
			}
		}
	}

	alist.sort();

	// Collapse (locally) identical assignments.
	for(ai = alist.begin(); ai != alist.end();)
	{
		aif = ai;

		for(++ai; ai != alist.end() && aif->local == ai->local;)
		{
			if(aif->s > ai->s)
			{
				alist.erase(aif);
				aif = ai;
				++ai;
			}
			else
			{
				alist.erase(ai);
				ai = aif;
				++ai;
			}
		}
	}

	/*for(ai = alist1.begin(); ai != alist1.end(); ++ai)
		print_assignment(*ai);
	std::cout << "\n";*/
}

// Handle join nodes in the nice tree decomposition
template <class T_t, class G_t, class I_t>
void tree_dec_ralloc_join(T_t &T, typename boost::graph_traits<T_t>::vertex_descriptor t, const G_t &G, const I_t &I)
{
	typedef typename boost::graph_traits<T_t>::adjacency_iterator adjacency_iter_t;
	adjacency_iter_t c, c_end, c2, c3;
	boost::tie(c, c_end) = adjacent_vertices(t, T);

	//std::cout << "Join:\n";

	c2 = c;
	++c;
	c3 = c;

	std::list<assignment> &alist1 = T[t].assignments;
	std::list<assignment> &alist2 = T[*c2].assignments;
	std::list<assignment> &alist3 = T[*c3].assignments;

	alist2.sort();
	alist3.sort();

	std::list<assignment>::iterator ai2, ai3;
	for(ai2 = alist2.begin(), ai3 = alist3.begin(); ai2 != alist2.end() && ai3 != alist3.end();)
	{
		if(ai2->local == ai3->local)
		{
			ai2->s += ai3->s;
			// Avoid double-counting instruction costs.
			std::set<unsigned int>::iterator bi;
			for(bi = T[t].bag.begin(); bi != T[t].bag.end(); ++bi)
				ai2->s -= instruction_cost(*ai2, *bi, G, I);
			for(unsigned int i = 0; i < ai2->global.size(); i++)
				ai2->global[i] = ((ai2->global[i] != -1) ? ai2->global[i] : ai3->global[i]);
			alist1.push_back(*ai2);
			++ai2;
			++ai3;
		}
		else if(*ai2 < *ai3)
		{
			++ai2;
			continue;
		}
		else if(*ai3 < *ai2)
		{
			++ai3;
			continue;
		}
	}

	alist2.clear();
	alist3.clear();

	/*std::list<assignment>::iterator ai;
	for(ai = alist1.begin(); ai != alist1.end(); ++ai)
		print_assignment(*ai);
	std::cout << "\n";*/
}

// Handle nodes in the tree decomposition, by detecting their type and calling the appropriate function. Recurses.
template <class T_t, class G_t, class I_t>
void tree_dec_ralloc_nodes(T_t &T, typename boost::graph_traits<T_t>::vertex_descriptor t, const G_t &G, const I_t &I)
{
	typedef typename boost::graph_traits<T_t>::adjacency_iterator adjacency_iter_t;

	adjacency_iter_t c, c_end;
	typename boost::graph_traits<T_t>::vertex_descriptor c0, c1;

	boost::tie(c, c_end) = adjacent_vertices(t, T);

	switch(out_degree(t, T))
	{
	case 0:
		tree_dec_ralloc_leaf(T, t, G, I);
		break;
	case 1:
		c0 = *c;
		tree_dec_ralloc_nodes(T, c0, G, I);
		T[c0].bag.size() < T[t].bag.size() ? tree_dec_ralloc_introduce(T, t, G, I) : tree_dec_ralloc_forget(T, t, G, I);
		break;
	case 2:
		c0 = *c++;
		c1 = *c;
		tree_dec_ralloc_nodes(T, c0, G, I);
		tree_dec_ralloc_nodes(T, c1, G, I);
		tree_dec_ralloc_join(T, t, G, I);
		break;
	default:
		std::cerr << "Not nice.\n";
		break;
	}
}

template <class T_t, class G_t, class I_t>
void tree_dec_ralloc(T_t &T, const G_t &G, const I_t &I)
{
	con2_t I2(boost::num_vertices(I));
	for(unsigned int i = 0; i < boost::num_vertices(I); i++)
	{
		I2[i].v = I[i].v;
		I2[i].byte = I[i].byte;
		I2[i].name = I[i].name;
	}
	typename boost::graph_traits<I_t>::edge_iterator e, e_end;
	for(boost::tie(e, e_end) = boost::edges(I); e != e_end; ++e)
		add_edge(boost::source(*e, I), boost::target(*e, I), I2);
	

	typename boost::graph_traits<T_t>::vertex_iterator t, t_end;
	boost::tie(t, t_end) = boost::vertices(T);
	tree_dec_ralloc_nodes(T, *t, G, I2);

	
	std::map<unsigned short int, unsigned short int>::const_iterator i;

	/*std::cout << "Winner: ";
	std::cout << "[";
	for(i = T[*t].assignments.begin()->global.begin(); i != T[*t].assignments.begin()->global.end(); ++i)
		std::cout << "(" << i->first << ", " << i->second << "), ";
	std::cout << "] s: " << T[*t].assignments.begin()->s << "\n";*/
	
	const assignment &winner = *(T[*t].assignments.begin());
	
	// Todo: Make this an assertion
	if(winner.global.size() != boost::num_vertices(I))
		std::cerr << "ERROR\n";
	
	for(unsigned int v = 0; v < boost::num_vertices(I); v++)
	{
		symbol *sym = (symbol *)(hTabItemWithKey(liveRanges, I[v].v));
		if(winner.global[v] >= 0)
			sym->regs[I[v].byte] = regsZ80 + winner.global[v];
		else
			spillThis(sym);
	}
	
	//std::cout << "Applied.\n";
}

// Dump con, with numbered nodes, show live variables at each node.
void dump_con(const con_t &con)
{
	std::ofstream dump_file((std::string(dstFileName) + ".dumpcon" + currFunc->rname + ".dot").c_str());

	std::string name[num_vertices(con)];
	for(unsigned int i = 0; i < num_vertices(con); i++)
	{
		std::ostringstream os;
		os << i;
		if(con[i].name)
			os << " : " << con[i].name << ":" << con[i].byte;
		name[i] = os.str();
	}
	write_graphviz(dump_file, con, boost::make_label_writer(name));
}

// Dump cfg, with numbered nodes, show live variables at each node.
void dump_cfg(const cfg_t &cfg)
{
	std::ofstream dump_file((std::string(dstFileName) + ".dumpcfg" + currFunc->rname + ".dot").c_str());

	std::string name[num_vertices(cfg)];
	for(unsigned int i = 0; i < num_vertices(cfg); i++)
	{
		std::ostringstream os;
		os << i << ", " << cfg[i].ic->key << ": ";
		std::set<unsigned short int>::iterator v;
		for(v = cfg[i].alive.begin(); v != cfg[i].alive.end(); ++v)
			os << *v << " ";
		name[i] = os.str();
	}
	write_graphviz(dump_file, cfg, boost::make_label_writer(name));
}

// Dump tree decomposition, show bag and live variables at each node.
void dump_tree_decomposition(const tree_dec_t &tree_dec)
{
	std::ofstream dump_file((std::string(dstFileName) + ".dumpdec" + currFunc->rname + ".dot").c_str());

	unsigned int w = 0;

	std::string name[num_vertices(tree_dec)];
	for(unsigned int i = 0; i < num_vertices(tree_dec); i++)
	{
		if(tree_dec[i].bag.size() > w)
			w = tree_dec[i].bag.size();
		std::ostringstream os;
		std::set<unsigned int>::iterator v1;
		for(v1 = tree_dec[i].bag.begin(); v1 != tree_dec[i].bag.end(); ++v1)
			os << *v1 << " ";
		os << ": ";
		std::set<unsigned short int>::iterator v2;
		for(v2 = tree_dec[i].alive.begin(); v2 != tree_dec[i].alive.end(); ++v2)
			os << *v2 << " ";
		name[i] = os.str();
	}
	write_graphviz(dump_file, tree_dec, boost::make_label_writer(name));

	//std::cout << "Width: " << (w  - 1) << "(" << currFunc->name << ")\n";
}

void z80_ralloc2_cc(ebbIndex *ebbi)
{
	//std::cout << "Processing " << currFunc->name << " from " << dstFileName << "\n";
	//std::cout.flush();

	cfg_t control_flow_graph;

	con_t conflict_graph;

	create_cfg(control_flow_graph, conflict_graph, ebbi);

	if(z80_opts.dump_graphs)
		dump_cfg(control_flow_graph);

	if(z80_opts.dump_graphs)
		dump_con(conflict_graph);

	tree_dec_t tree_decomposition;

	thorup_tree_decomposition(tree_decomposition, control_flow_graph);

	nicify(tree_decomposition);

	alive_tree_dec(tree_decomposition, control_flow_graph);

	if(z80_opts.dump_graphs)
		dump_tree_decomposition(tree_decomposition);

	tree_dec_ralloc(tree_decomposition, control_flow_graph, conflict_graph);
}

