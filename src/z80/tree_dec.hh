// Philipp Klaus Krause, philipp@informatik.uni-frankfurt.de, pkk@spth.de, 2010 - 2011
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
//
// Some routines for tree-decompositions.
//
// A tree decomposition is a graph that has a set of vertex indices as bundled property, e.g.:
//
// struct tree_dec_node
// {
//	std::set<unsigned int> bag;
// };
// typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, tree_dec_node> tree_dec_t;
//
// The following are the routines that are most likely to be interesting for outside use:
//
// void nicify(T_t &T)
// Transforms a tree decomposition T into a nice tree decomposition
//
// void thorup_tree_decomposition(T_t &tree_decomposition, const G_t &cfg)
// Creates a tree decomposition T from a graph cfg using Thorup's heuristic.
//
// void tree_decomposition_from_elimination_ordering(T_t &T, std::list<unsigned int>& l, const G_t &G)
// Creates a tree decomposition T of a graph G from an elimination ordering l.
//
// void thorup_elimination_ordering(l_t &l, const J_t &J)
// Creates an elimination ordering l of a graph J using Thorup's heuristic.

#include <map>
#include <vector>
#include <set>
#include <stack>
#include <list>

#include <boost/tuple/tuple_io.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/adjacency_list.hpp>

// Thorup algorithm D.
// The use of the multimap makes the complexity of this O(|I|log|I|), which could be reduced to O(|I|).
template <class l_t>
void thorup_D(l_t &l, const std::multimap<unsigned int, unsigned int> &MJ, const std::multimap<unsigned int, unsigned int> &MS, const unsigned int n)
{
	std::map<unsigned int, unsigned int> m;

	l.clear();

	unsigned int i = 0;
	for(unsigned int j = n; j > 0;)
	{
		j--;
		if(m.find(j) == m.end())
			m[j] = i++;

		std::multimap<unsigned int, unsigned int>::const_iterator k, k_end;

		for(boost::tie(k, k_end) = MS.equal_range(j); k != k_end; ++k)
			if(m.find(k->second) == m.end())
				m[k->second] = i++;

		for(boost::tie(k, k_end) = MJ.equal_range(j); k != k_end; ++k)
			if(m.find(k->second) == m.end())
				m[k->second] = i++;
	}

	std::vector<unsigned int> v(n);

	std::map<unsigned int, unsigned int>::iterator mi;

	for(mi = m.begin(); mi != m.end(); ++mi)
		v[mi->first] = mi->second;

	for(i = 0; i < n; i++)
		l.push_back(v[i]);
}

// Thorup algorithm E.
// The use of the multimap makes the complexity of this O(|I|log|I|), which could be reduced to O(|I|).
template <class I_t>
void thorup_E(std::multimap<unsigned int, unsigned int> &M, const I_t &I)
{
	typedef typename boost::graph_traits<I_t>::adjacency_iterator adjacency_iter_t;
	typedef typename boost::graph_traits<I_t>::vertex_iterator vertex_iter_t;
	typedef typename boost::property_map<I_t, boost::vertex_index_t>::type index_map;
	index_map index = get(boost::vertex_index, I);

	std::stack<std::pair<unsigned int, unsigned int> > s;

	M.clear();

	s.push(std::pair<unsigned int, unsigned int>(0, boost::num_vertices(I)));

	for(unsigned int i = 0; i < boost::num_vertices(I); i++)
	{
		unsigned int j = i;
		adjacency_iter_t j_curr, j_end;

		for(boost::tie(j_curr, j_end) = boost::adjacent_vertices(i, I); j_curr != j_end; ++j_curr)
			if(index[*j_curr] > j)
				j = index[*j_curr];

		if(j == i)
			continue;

		while(s.top().second <= i)
		{
			M.insert(std::pair<unsigned int, unsigned int>(s.top().second, s.top().first));
			s.pop();
		}

		unsigned int i2 = i;	// Using i2 instead of i keeps this (or rather thorup_D) from crashing.

		while(j > s.top().second && s.top().second > i2)
		{
			i2 = s.top().first;
			s.pop();
		}

		s.push(std::pair<unsigned int, unsigned int>(i2, j));
	}
}

// Heuristically give an elimination ordering for a directed graph.
// For a description of this, including algorithms D and E, see
// Mikkel Thorup, "All Structured Programs have Small Tree-Width and Good Register Allocation", Appendix A.
// The use of the multimap makes the complexity of this O(|I|log|I|), could be reduced to O(|I|).
template <class l_t, class G_t>
void thorup_elimination_ordering(l_t &l, const G_t &G)
{
	// Should we do this? Or just use G as J? The Thorup paper seems unclear, it speaks of statements that contain jumps to other statements, but does it count as a jump, when they're just subsequent?
	boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> J;
	boost::copy_graph(G, J);
	for(unsigned int i = 0; i < boost::num_vertices(J) - 1; i++)
		remove_edge(i, i + 1, J);

	// Todo: Implement a graph adaptor for boost that allows to treat directed graphs as undirected graphs.
	boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> S;
	boost::copy_graph(J, S);

	std::multimap<unsigned int, unsigned int> MJ, MS;

	thorup_E(MJ, J);

	thorup_E(MS, S);

	thorup_D(l, MJ, MS, num_vertices(J));
}

// Finds a (the newest) bag that contains all vertices in X in the tree decomposition T.
template <class T_t>
typename boost::graph_traits<T_t>::vertex_iterator find_bag(const std::set<unsigned int> &X, const T_t &T)
{
	typedef typename boost::graph_traits<T_t>::vertex_iterator T_vertex_iter_t;
	typedef typename std::set<unsigned int>::iterator vertex_index_iter_t;

	T_vertex_iter_t t, t_end, t_found;
	vertex_index_iter_t v;

	for(boost::tie(t, t_end) = vertices(T), t_found = t_end; t != t_end; ++t)
	{
		for(v = X.begin(); v != X.end(); ++v)
			if(T[*t].bag.find(*v) == T[*t].bag.end())
				break;

		if(v == X.end())
			t_found = t;
	}

	if(t_found == t_end)	// Todo: Better error handling (throw exception?)
	{
		std::cerr << "find_bag() failed.\n";
		std::cerr.flush();
	}

	return(t_found);
}

// Add edges to make the vertices in X a clique in G.
template <class G_t>
void make_clique(const std::set<unsigned int> &X , G_t &G)
{
	std::set<unsigned int>::iterator n1, n2;
	for(n1 = X.begin(); n1 != X.end(); n1++)
		for(n2 = n1, ++n2; n2 != X.end(); ++n2)
			add_edge(*n1, *n2, G);
}

template <class T_t, class v_t, class G_t>
void add_vertices_to_tree_decomposition(T_t &T, const v_t v, const v_t v_end, G_t &G, std::vector<bool> &active)
{
	// Base case: Empty graph. Create an empty bag.
	if(v == v_end)
	{
		add_vertex(T);
		return;
	}

	// Todo: A more elegant solution, e.g. using subgraphs or filtered graphs.

	typedef typename boost::graph_traits<G_t>::adjacency_iterator adjacency_iter_t;
	typedef typename boost::property_map<G_t, boost::vertex_index_t>::type index_map;
	index_map index = get(boost::vertex_index, G);

	// Get the neigbours
	adjacency_iter_t n, n_end;
	std::set<unsigned int> neighbours;
	for(boost::tie(n, n_end) = boost::adjacent_vertices(*v, G); n != n_end; ++n)
		if(active[index[*n]])
			neighbours.insert(index[*n]);

	// Recurse
	active[*v] = false;
	make_clique(neighbours, G);
	v_t v_next = v;
	add_vertices_to_tree_decomposition(T, ++v_next, v_end, G, active);

	// Add new bag
	typename boost::graph_traits<T_t>::vertex_iterator t, s;
	t = find_bag(neighbours, T);
	s = add_vertex(T);
	add_edge(*t, *s, T);
	T[*s].bag = neighbours;
	T[*s].bag.insert(*v);

	//std::set<unsigned int>::iterator x;
	//std::cout << "Added (";
	//for(x = T[*s].begin(); x != T[*s].end(); ++x)
	//	std::cout << *x << " ";
	//std::cout << ") connecting " << *s << " to " << *t << "\n";
}

// Create a tree decomposition from en elimination ordering.
template <class T_t, class G_t>
void tree_decomposition_from_elimination_ordering(T_t &T, std::list<unsigned int>& l, const G_t &G)
{
	std::list<unsigned int>::reverse_iterator v, v_end;
	v = l.rbegin(), v_end = l.rend();

	// Todo: Implement a graph adaptor for boost that allows to treat directed graphs as undirected graphs.
	boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> G_sym;
	boost::copy_graph(G, G_sym);

	std::vector<bool> active(boost::num_vertices(G));
	active.assign(true, boost::num_vertices(G));

	add_vertices_to_tree_decomposition(T, v, v_end, G_sym, active);
}

template <class T_t, class G_t>
void thorup_tree_decomposition(T_t &tree_decomposition, const G_t &cfg)
{
	std::list<unsigned int> elimination_ordering;

	thorup_elimination_ordering(elimination_ordering, cfg);

	tree_decomposition_from_elimination_ordering(tree_decomposition, elimination_ordering, cfg);
}

// Ensure that all joins are at proper join nodes: Each node that has two children has the same bag as its children.
template <class T_t>
void nicify_joins(T_t &T, typename boost::graph_traits<T_t>::vertex_descriptor t)
{
	typedef typename boost::graph_traits<T_t>::adjacency_iterator adjacency_iter_t;

	adjacency_iter_t c, c_end;
	typename boost::graph_traits<T_t>::vertex_descriptor c0, c1;

	boost::tie(c, c_end) = adjacent_vertices(t, T);

	switch(out_degree(t, T))
	{
	case 0:
		return;
	case 1:
		nicify_joins(T, *c);
		return;
	case 2:
		break;
	default:
		c0 = *c++;
		c1 = *c;
		typename boost::graph_traits<T_t>::vertex_descriptor d;
		d = add_vertex(T);
		add_edge(d, c0, T);
		add_edge(d, c1, T);
		remove_edge(t, c0, T);
		remove_edge(t, c1, T);
		T[d].bag = T[t].bag;
		add_edge(t, d, T);
		nicify_joins(T, t);
		return;
	}

	c0 = *c++;
	c1 = *c;
	nicify_joins(T, c0);
	if(T[t].bag != T[c0].bag)
	{
		typename boost::graph_traits<T_t>::vertex_descriptor d;
		d = add_vertex(T);
		add_edge(d, c0, T);
		remove_edge(t, c0, T);
		T[d].bag = T[t].bag;
		add_edge(t, d, T);
	}
	nicify_joins(T, c1);
	if(T[t].bag != T[c1].bag)
	{
		typename boost::graph_traits<T_t>::vertex_descriptor d = add_vertex(T);
		add_edge(d, c1, T);
		remove_edge(t, c1, T);
		T[d].bag = T[t].bag;
		add_edge(t, d, T);
	}
}

// Ensure that all nodes' bags are either a subset or a superset of their successors'.
template <class T_t>
void nicify_diffs(T_t &T, typename boost::graph_traits<T_t>::vertex_descriptor t)
{
	typedef typename boost::graph_traits<T_t>::adjacency_iterator adjacency_iter_t;

	adjacency_iter_t c, c_end;
	typename boost::graph_traits<T_t>::vertex_descriptor c0, c1;

	boost::tie(c, c_end) = adjacent_vertices(t, T);

	switch(out_degree(t, T))
	{
	case 0:
		return;
	case 1:
		break;
	case 2:
		
		c0 = *c++;
		c1 = *c;
		nicify_diffs(T, c0);
		nicify_diffs(T, c1);
		return;
	default:
		std::cerr << "nicify_diffs error.\n";
		return;
	}

	c0 = *c;
	nicify_diffs(T, c0);

	if(std::includes(T[t].bag.begin(), T[t].bag.end(), T[c0].bag.begin(), T[c0].bag.end()) || std::includes(T[c0].bag.begin(), T[c0].bag.end(), T[t].bag.begin(), T[t].bag.end()))
		return;

	typename boost::graph_traits<T_t>::vertex_descriptor d = add_vertex(T);

	add_edge(d, c0, T);
	remove_edge(t, c0, T);
	std::set_intersection(T[t].bag.begin(), T[t].bag.end(), T[c0].bag.begin(), T[c0].bag.end(),std::inserter(T[d].bag, T[d].bag.begin()));
	add_edge(t, d, T);
}

// // Ensure that all nodes' bags' sizes differ by at most one to their successors'.
template <class T_t>
void nicify_diffs_more(T_t &T, typename boost::graph_traits<T_t>::vertex_descriptor t)
{
	typedef typename boost::graph_traits<T_t>::adjacency_iterator adjacency_iter_t;

	adjacency_iter_t c, c_end;
	typename boost::graph_traits<T_t>::vertex_descriptor c0, c1;

	boost::tie(c, c_end) = adjacent_vertices(t, T);

	switch(out_degree(t, T))
	{
	case 0:
		if(T[t].bag.size() > 1)
		{
			typename boost::graph_traits<T_t>::vertex_descriptor d = add_vertex(T);
			T[d].bag = T[t].bag;
			T[d].bag.erase(T[d].bag.begin());
			add_edge(t, d, T);
			nicify_diffs_more(T, t);
		}
		return;
	case 1:
		break;
	case 2:
		
		c0 = *c++;
		c1 = *c;
		nicify_diffs_more(T, c0);
		nicify_diffs_more(T, c1);
		return;
	default:
		std::cerr << "nicify_diffs_more error.\n";
		return;
	}

	c0 = *c;

	size_t c0_size, t_size;
	t_size = T[t].bag.size();
	c0_size = T[c0].bag.size();

	if(t_size <= c0_size + 1 && t_size + 1 >= c0_size)
	{
		nicify_diffs_more(T, c0);
		return;
	}

	typename boost::graph_traits<T_t>::vertex_descriptor d = add_vertex(T);
	add_edge(d, c0, T);
	remove_edge(t, c0, T);
	T[d].bag = T[t_size > c0_size ? t : c0].bag;
	std::set<unsigned int>::iterator i;
	for(i = T[d].bag.begin(); T[t_size < c0_size ? t : c0].bag.find(*i) != T[t_size < c0_size ? t : c0].bag.end(); ++i);
	T[d].bag.erase(i);
	add_edge(t, d, T);

	nicify_diffs_more(T, t);
}

// Transform a tree decomposition into a nice tree decomposition.
// TODO: Make this function no longer assume that the root is the first vertex.
template <class T_t>
void nicify(T_t &T)
{
	typename boost::graph_traits<T_t>::vertex_iterator t, t_end;
	boost::tie(t, t_end) = vertices(T);

	nicify_joins(T, *t);
	nicify_diffs(T, *t);
	nicify_diffs_more(T, *t);
}

