// Philipp Klaus Krause, philipp@informatik.uni-frankfurt.de, pkk@spth.de, 2011
//
// (c) 2011 Goethe-Universität Frankfurt
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
// Optimal placement of bank switching instructions for named address spaces.

#include "SDCCnaddr.hpp"

int
switchAddressSpacesOptimally (iCode *ic, ebbIndex *ebbi)
{
  cfg_t control_flow_graph;
  tree_dec_naddr_t tree_decomposition;

  create_cfg_naddr(control_flow_graph, ic, ebbi);
  annotate_cfg_naddr(control_flow_graph);

  if(options.dump_graphs)
    dump_cfg_naddr(control_flow_graph);

  thorup_tree_decomposition(tree_decomposition, control_flow_graph);
  nicify(tree_decomposition);

  return(tree_dec_address_switch(tree_decomposition, control_flow_graph));
}

