/*
 * Simulator of microcontrollers (sim.src/var.cc)
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

#include <string.h>
#include <ctype.h>

#include "varcl.h"


/* Cell only variable */

cl_cvar::cl_cvar(chars iname, class cl_memory_cell *icell, chars adesc, int ibitnr_high, int ibitnr_low):
  cl_base()
{
  if (ibitnr_low < ibitnr_high)
    {
      bitnr_low= ibitnr_low;
      bitnr_high= ibitnr_high;
    }
  else
    {
      bitnr_low= ibitnr_high;
      bitnr_high= ibitnr_low;
    }
  desc= adesc;
  
  set_name(iname);
  
  cell= icell;
}

int
cl_cvar::init(void)
{
  return 0;
}

t_mem
cl_cvar::write(t_mem val)
{
  if (!cell)
    return 0;
  return cell->write(val);
}

t_mem
cl_cvar::read()
{
  if (!cell)
    return 0;
  return cell->read();
}

t_mem
cl_cvar::set(t_mem val)
{
  if (!cell)
    return 0;
  return cell->set(val);
}

void
cl_cvar::print_info(cl_console_base *con) const
{
  con->dd_printf("%s ", get_name("?"));
  t_mem m= cell->get();
  if (bitnr_high >= 0)
    {
      if (bitnr_high != bitnr_low)
        {
          con->dd_printf("[%u:%u] = 0b", bitnr_high, bitnr_low);
          for (int i= bitnr_high; i >= bitnr_low; i--)
            con->dd_printf("%c", (m & (1U << i)) ? '1' : '0');
          m &= ((1U << (bitnr_high - bitnr_low + 1)) - 1) << bitnr_low;
        }
      else
        {
          m &= (1U << bitnr_low);
          con->dd_printf(".%u = %c", bitnr_low, m ? '1' : '0');
        }
    }
  else
    {
      con->dd_printf(" = 0x");
      con->dd_printf(/*mem->data_format*/"%08x", m);
    }
  con->dd_printf(",%uU", MU(m));
  con->dd_printf(",%d", MI(m));
  if ((MU(m) < 0x100) && isprint(MI(m)))
    con->dd_printf(",'%c'", MI(m));
  con->dd_printf("\n");
  if (desc && desc[0])
    con->dd_printf("  %s\n", desc.c_str());
}


/* Variable pointing to memory location */

cl_var::cl_var(chars iname, class cl_memory *imem, t_addr iaddr, chars adesc, int ibitnr_high, int ibitnr_low):
  cl_cvar(iname, NULL, adesc, ibitnr_high, ibitnr_low)
{
  mem= imem;
  addr= iaddr;
}

int
cl_var::init(void)
{
  if (!mem ||
      !mem->is_address_space() ||
      !mem->valid_address(addr))
    return 0;
  cell= static_cast<cl_address_space *>(mem)->get_cell(addr);
  return 0;
}

void
cl_var::print_info(cl_console_base *con) const
{
  con->dd_printf("%s ", get_name("?"));
  con->dd_printf("%s", mem->get_name("?"));
  con->dd_printf("[");
  con->dd_printf(mem->addr_format, addr);
  con->dd_printf("]");
  t_mem m= mem->get(addr);
  if (bitnr_high >= 0)
    {
      if (bitnr_high != bitnr_low)
        {
          con->dd_printf("[%u:%u] = 0b", bitnr_high, bitnr_low);
          for (int i= bitnr_high; i >= bitnr_low; i--)
            con->dd_printf("%c", (m & (1U << i)) ? '1' : '0');
          m &= ((1U << (bitnr_high - bitnr_low + 1)) - 1) << bitnr_low;
        }
      else
        {
          m &= (1U << bitnr_low);
          con->dd_printf(".%u = %c", bitnr_low, m ? '1' : '0');
        }
    }
  else
    {
      con->dd_printf(" = 0x");
      con->dd_printf(mem->data_format, m);
    }
  con->dd_printf(",%uU", MU(m));
  con->dd_printf(",%d", MI(m));
  if ((MU(m) < 0x100) && isprint(MI(m)))
    con->dd_printf(",'%c'", MI(m));
  con->dd_printf("\n");
  if (desc && desc[0])
    con->dd_printf("  %s\n", desc.c_str());
}


cl_var_by_name_list::~cl_var_by_name_list(void)
{
}

const void *
cl_var_by_name_list::key_of(const void *item) const
{
  return static_cast<const class cl_var *>(item)->get_name();
}

int
cl_var_by_name_list::compare(const void *key1, const void *key2)
{
  if (key1 && key2)
    return strcmp(static_cast<const char *>(key1), static_cast<const char *>(key2));
  return 0;
}


cl_var_by_addr_list::~cl_var_by_addr_list(void)
{
}

int
cl_var_by_addr_list::compare_addr(class cl_var *var, class cl_memory *mem, t_addr addr)
{
  int ret;
  class cl_memory *vmem= var->get_mem();
  t_addr vaddr= var->get_addr();
  if (!vmem)
    return 0;
  
  if (!(ret = vmem->get_uid() - mem->get_uid()))
    ret = vaddr - addr;

  return ret;
}

int
cl_var_by_addr_list::compare_addr_and_bits(class cl_var *var, class cl_memory *mem, t_addr addr, int bitnr_high, int bitnr_low)
{
  int ret;
  class cl_memory *vmem= var->get_mem();
  t_addr vaddr= var->get_addr();
  if (!vmem)
    return 0;
  
  if (!(ret = vmem->get_uid() - mem->get_uid()) &&
     !(ret = vaddr - addr) &&
     (!(ret = (var->bitnr_high < 0
               ? (bitnr_high < 0 ? 0 : -1)
               : (bitnr_high < 0
                  ? 1
                  : bitnr_high - var->bitnr_high)))))
    ret = (var->bitnr_low < 0
           ? (bitnr_low < 0 ? 0 : -1)
           : (bitnr_low < 0
              ? 1
              : var->bitnr_low - bitnr_low));

  return ret;
}

int
cl_var_by_addr_list::compare(const void *key1, const void *key2)
{
  class cl_var *k1 = (cl_var*)(key1);
  class cl_var *k2 = (cl_var*)(key2);
  int ret;

  // An addr may have multiple names as long as they are all different.
  if (!(ret = compare_addr_and_bits(k1, k2->get_mem(), k2->get_addr(), k2->bitnr_high, k2->bitnr_low)))
    ret = strcmp(k1->get_name(), k2->get_name());

  return ret;
}

bool
cl_var_by_addr_list::search(class cl_memory *mem, t_addr addr, t_index &index)
{
  t_index l  = 0;
  t_index h  = count - 1;
  bool    res= false;

  while (l <= h)
    {
      t_index i= (l + h) >> 1;
      t_index c= compare_addr((cl_var *)(key_of(Items[i])), mem, addr);
      if (c < 0) l= i + 1;
      else
        {
          h= i - 1;
          if (c == 0)
            {
              res= true;
              // We want the _first_ name for the given addr.
              for (l = i; l > 0 && !compare_addr((cl_var *)(key_of(Items[l-1])), mem, addr); l--);
            }
        }
    }

  index= l;
  return(res);
}

bool
cl_var_by_addr_list::search(class cl_memory *mem, t_addr addr, int bitnr_high, int bitnr_low, t_index &index)
{
  t_index l  = 0;
  t_index h  = count - 1;
  bool    res= false;

  while (l <= h)
    {
      t_index i= (l + h) >> 1;
      t_index c= compare_addr_and_bits((cl_var *)(key_of(Items[i])), mem, addr, bitnr_high, bitnr_low);
      if (c < 0) l= i + 1;
      else
        {
          h= i - 1;
          if (c == 0)
            {
              res= true;
              // We want the _first_ name for the given addr.
              for (l = i; l > 0 && !compare_addr_and_bits((cl_var *)(key_of(Items[l-1])), mem, addr, bitnr_high, bitnr_low); l--);
            }
        }
    }

  index= l;
  return(res);
}

bool
cl_var_list::del(const char *name)
{
  t_index name_i;
  bool found = by_name.search(name, name_i);

  if (found)
    {
      cl_cvar *var = by_name.at(name_i);

      by_addr.disconn(var);
      by_name.disconn_at(name_i);
    }

  return found;
}

class cl_cvar *
cl_var_list::add(class cl_cvar *item)
{
  const char *name = item->get_name();

  if (!del(name))
    {
      int l = strlen(name);
      if (l > max_name_len)
        max_name_len = l;
    }

  by_name.add(item);
  if (item->is_mem_var())
    by_addr.add(item);

  return item;
}

class cl_cvar *
cl_var_list::add(chars name, class cl_memory *mem, t_addr addr, int bitnr_high, int bitnr_low, chars desc)
{
  class cl_cvar *var;

  var = new cl_var(name, mem, addr, desc, bitnr_high, bitnr_low);
  var->init();
  return add(var);
}

class cl_cvar *
cl_var_list::add(chars name, const char *cellname, int bitnr_high, int bitnr_low, chars desc)
{
  int i;
  if (by_name.search(cellname, i))
    {
      class cl_cvar *var = (cl_var*)by_name.at(i);
      return add(name, var->get_mem(), var->get_addr(), bitnr_high, bitnr_low, desc);
    }

  return NULL;
}

void
cl_var_list::add(chars prefix, class cl_memory *mem, t_addr base, const struct var_def *def, size_t n)
{
  chars regname = chars("");
  t_addr offset = 0;

  for (; n; def++, n--)
    {
      class cl_var *var;

      if (def->bitnr_high < 0)
        {
          regname = prefix + def->name;

          if (def->bitnr_low < 0)
            {
              offset = -1;

              int i;
              if (by_name.search(regname, i))
                {
                  cl_var *var = (cl_var*)by_name.at(i);
                  if (var->get_mem() == mem)
                    offset = var->get_addr() - base;
                }
            }
          else
            {
              offset = def->bitnr_low;

              var = new cl_var(regname, mem, base + offset, def->desc, -1, -1);
              var->init();
              add(var);

              regname += "_";
            }
        }
      else if (mem && offset >= 0)
        {
          var = new cl_var(regname + def->name, mem, base + offset, def->desc, def->bitnr_high, def->bitnr_low);
          var->init();
          add(var);
        }
    }
}

t_mem
cl_var_list::read(chars name)
{
  bool found;
  t_index i;
  class cl_cvar *v;
  found= by_name.search(name, i);
  if (found)
    {
      v= by_name.at(i);
      return v->read();
    }
  return 0;
}

/* End of sim.src/var.cc */
