/*
 * Simulator of microcontrollers (sim.src/varcl.h)
 *
 * Copyright (C) @@S@@,@@Y@@ Drotos Daniel, Talker Bt.
 * 
 * To contact author send email to drdani@mazsola.iit.uni-miskolc.hu
 *
 */

/*
  This file is part of microcontroller simulator: ucsim.

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
  02111-1307, USA.
*/
/*@1@*/

#ifndef SIM_VARCL_HEADER
#define SIM_VARCL_HEADER


#include "pobjcl.h"

#include "newcmdcl.h"

#include "memcl.h"


class cl_var: public cl_base
{
 public:
  class cl_memory *mem;
  t_addr addr;
  int bitnr_high, bitnr_low;
  chars desc;
 protected:
  class cl_memory_cell *cell;
 public:
  cl_var(chars iname, class cl_memory *imem, t_addr iaddr, chars adesc, int ibitnr_high= -1, int ibitnr_low= -1);
  int init(void);
  class cl_memory_cell *get_cell(void) const { return cell; }

  t_mem write(t_mem val);
  t_mem set(t_mem val);

  void print_info(cl_console_base *con) const;
};


class cl_var_by_name_list: public cl_sorted_list
{
 public:
  cl_var_by_name_list(): cl_sorted_list(10, 10, "symlist") {}
  ~cl_var_by_name_list(void);

  const class cl_var *at(t_index index)
  {
    const void *a= cl_sorted_list::at(index);
    const class cl_var *v= (const class cl_var *)a;
    return /*static_cast<const cl_var *>*/(v);
  }

 private:
  virtual const void *key_of(const void *item) const;
  virtual int compare(const void *key1, const void *key2);
};

class cl_var_by_addr_list: public cl_sorted_list
{
 public:
  cl_var_by_addr_list(): cl_sorted_list(10, 10, "symlist_by_addr") {}
  ~cl_var_by_addr_list(void);

  const class cl_var *at(t_index index)
  {
    const void *a= cl_sorted_list::at(index);
    const class cl_var *v= (const class cl_var *)a;
    return /*static_cast<const cl_var *>*/(v);
  }
  bool search(const class cl_memory *mem, t_addr addr, t_index &index);
  bool search(const class cl_memory *mem, t_addr addr, int bitnr_high, int bitnr_low, t_index &index);

 private:
  int compare_addr(const class cl_var *var, const class cl_memory *mem, t_addr addr) const;
  int compare_addr_and_bits(const class cl_var *var, const class cl_memory *mem, t_addr addr, int bitnr_high, int bitnr_low) const;

  virtual int compare(const void *key1, const void *key2);
};

struct var_def {
  const char *name;
  int bitnr_high, bitnr_low;
  const char *desc;
};

#define var_thiscell()	-1, -1
#define var_offset(N)	-1, N
#define var_bit(N)	N, N
#define var_bitset(H,L)	H, L

class cl_var_list: public cl_base
{
 private:
  int max_name_len;

 public:
  class cl_var_by_name_list by_name;
  class cl_var_by_addr_list by_addr;

 public:
  cl_var_list() {max_name_len = 0;}

  /*! \brief Add the given cl_var replacing any that already exist with the same name.
   */
  cl_var *add(cl_var *item);

  /*! \brief Create and add (or replace) a var naming a set of bits in the cell given by mem and addr.
   */
  cl_var *add(chars name, class cl_memory *mem, t_addr addr, int bitnr_high, int bitnr_low, chars desc);

  /*! \brief Create and add (or replace) a var naming a set of bits in the cell given by a named var.
   */
  cl_var *add(chars name, const char *cellname, int bitnr_high, int bitnr_low, chars desc);

  /*! \brief Create and add (or replace) a var labelling a cell.
   */
  cl_var *add(chars name, class cl_memory *mem, t_addr addr, chars desc) {
    return add(name, mem, addr, -1, -1, desc);
  }

  /*! \brief Create and add (or replace) vars using the given list of definitions.
   */
  void add(chars prefix, class cl_memory *mem, t_addr base, const struct var_def *def, size_t n);

  /*! \brief Delete the var with the given name.
   */
  bool del(const char *name);

  /*! \brief Return the length of the longest var name.
   */
  int get_max_name_len(void) { return max_name_len; }
};


#endif

/* End of sim.src/varcl.h */
