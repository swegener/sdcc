/*
 * Simulator of microcontrollers (sim.src/vcd.cc)
 *
 * Copyright (C) 2017,17 Drotos Daniel, Talker Bt.
 * 
 * To contact author send email to drdani@mazsola.iit.uni-miskolc.hu
 *
 */

/* This file is part of microcontroller simulator: ucsim.

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
02111-1307, USA. */
/*@1@*/

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// prj
#include "utils.h"

// sim
#include "argcl.h"

// local
#include "vcdcl.h"


class cl_vcd_var: public cl_memory_operator
{
 public:
  class cl_vcd_var *next_var;
  class cl_vcd *vcd;
  int bitnr_high, bitnr_low;
  t_mem bitmask;
  char var_id;

  cl_vcd_var(class cl_vcd *vcd, char vcd_id, class cl_memory_cell *cell, int bitnr_high, int bitnr_low);
  ~cl_vcd_var(void);

  t_mem write(t_mem val);
};


cl_vcd_var::cl_vcd_var(class cl_vcd *vcd, char var_id, class cl_memory_cell *cell, int bitnr_high, int bitnr_low):
  cl_memory_operator(cell)
{
  set_name(vcd->get_name());

  this->next_var = NULL;
  this->vcd = vcd;
  this->bitnr_high = bitnr_high;
  this->bitnr_low = bitnr_low;

  bitmask = ((1U << (bitnr_high - bitnr_low + 1)) - 1) << bitnr_low;

  if (!(this->var_id = var_id))
    {
      this->var_id = vcd->get_next_var_id();
      cell->append_operator(this);
    }
}


cl_vcd_var::~cl_vcd_var(void)
{
  cell->remove_operator(this);
}


t_mem
cl_vcd_var::write(t_mem val)
{
  if (vcd->is_running() && (val & bitmask) != (cell->get() & bitmask))
    vcd->report(this, val);

  return cl_memory_operator::write(val);
}


cl_vcd::cl_vcd(class cl_uc *auc, int aid, chars aid_string):
       cl_hw(auc, HW_DUMMY, aid, aid_string)
{
  filename= NULL;
  fd= 0;
  vars= NULL;
  reset_next_var_id();
  starttime= 0;
  timescale= 0;
  event= 0;
  state= -1;
  started= false;
  paused= false;
  dobreak= false;
  modul= chars("", "ucsim_vcd_%d", id);
}

int
cl_vcd::init(void)
{
  cl_hw::init();
  on= false;

  return 0;
}

void
cl_vcd::clear_vars(void)
{
  while (vars)
    {
      class cl_vcd_var *old = vars;
      vars = vars->next_var;
      delete old;
    }
}

void
cl_vcd::add_var(class cl_console_base *con, char var_id, class cl_memory_cell *cell, int bitnr_high, int bitnr_low)
{
  if (!cell)
    return;

  if (cell->get_flag(CELL_NON_DECODED))
    {
      if (con) con->dd_printf("Cell is not decoded\n");
      return;
    }

  if (started)
    {
      if (con) con->dd_printf("Already started\n");
      return;
    }

  if (bitnr_high == -1)
    bitnr_high = cell->get_width() - 1;

  if (bitnr_low == -1)
    bitnr_low = 0;

  class cl_vcd_var **var_p;

  for (var_p = &vars; *var_p; var_p = &((*var_p)->next_var))
    {
      if ((*var_p)->cell == cell &&
          (*var_p)->bitnr_high == bitnr_high && (*var_p)->bitnr_low == bitnr_low)
        {
          if (con) con->dd_printf("Already exists\n");
          return;
        }
    }

  *var_p = new cl_vcd_var(this, var_id, cell, bitnr_high, bitnr_low);
}

void
cl_vcd::del_var(class cl_console_base *con, class cl_memory_cell *cell, int bitnr_low, int bitnr_high)
{
  for (class cl_vcd_var **var_p = &vars; *var_p; var_p = &((*var_p)->next_var))
    {
      if ((*var_p)->cell == cell)
        {
          if ((*var_p)->bitnr_high == bitnr_high && (*var_p)->bitnr_low == bitnr_low)
            {
              class cl_vcd_var *old = *var_p;
              *var_p = (*var_p)->next_var;
              delete old;

              if (vars == NULL && !started)
                reset_next_var_id();

              return;
            }
        }
    }

  if (con) con->dd_printf("Not found\n");
}

void
cl_vcd::add_var(class cl_console_base *con, class cl_memory *m, t_addr a, int bitnr_high, int bitnr_low)
{
  if (!m->is_address_space())
    {
      if (con) con->dd_printf("Not an address space\n");
      return;
    }
  if (!m->valid_address(a))
    {
      if (con) con->dd_printf("Address must be between 0x%x and 0x%x\n",
                              AU(m->lowest_valid_address()),
                              AU(m->highest_valid_address()));
      return;
    }
  cl_memory_cell *c= ((cl_address_space*)m)->get_cell(a);
  add_var(con, 0, c, bitnr_high, bitnr_low);
}

void
cl_vcd::del_var(class cl_console_base *con, class cl_memory *m, t_addr a, int bitnr_low, int bitnr_high)
{
  if (!m->is_address_space())
    {
      if (con) con->dd_printf("%s is not an address space\n");
      return;
    }
  if (!m->valid_address(a))
    {
      if (con) con->dd_printf("Address must be between 0x%x and 0x%x\n",
                              AU(m->lowest_valid_address()),
                              AU(m->highest_valid_address()));
      return;
    }
  del_var(con, ((cl_address_space*)m)->get_cell(a), bitnr_low, bitnr_high);
}

bool
cl_vcd::parse_header(cl_console_base *con)
{
  int width = 0;
  char id = 0;

  clear_vars();
  state = 0;
  word[0] = 'X';
  word[1] = ' ';
  char *token = &word[2];

  while (read_word(2U))
    {
      if (!strcmp(token, "$end"))
        state = 0;
      else if (state == 0)
        {
          if (token[0] == '#')
            {
              event = timescale * strtod(&token[1], NULL);
              break;
            }
          else if (!strcmp(token, "$timescale"))
            state = 2;
          else if (!strcmp(token, "$dumpvars"))
            state = 3;
          else if (!strcmp(token, "$var"))
             state = 4;
        }
      else if (state == 2) // $timescale
        {
          int i = 0;
          if (token[0] >= '0' && token[0] <= '9')
            sscanf(token, "%lf%n", &timescale, &i);
          if (!strcmp(&token[i], "fs"))
            timescale *= 1e-15;
          else if (!strcmp(&token[i], "ps"))
            timescale *= 1e-12;
          else if (!strcmp(&token[i], "ns"))
            timescale *= 1e-09;
          else if (!strcmp(&token[i], "us"))
            timescale *= 1e-06;
          else if (!strcmp(&token[i], "ms"))
            timescale *= 1e-03;
        }
      else if (state == 3) // $dumpvars
        // FIXME: does it make sense to set the initial state of all vars?
        ; // set_mem_from_var(token); // FIXME: not implemented yet
      else if (state == 4) // $var [type]
        state = (!strcmp(token, "wire") ? 5 : 8);
      else if (state == 5) // $var [type] [bitwidth]
        {
          width = strtol(token, NULL, 0);
          state = 6;
        }
      else if (state == 6) // $var [type] [bitwidth] [id]
        {
          id = token[0];
          state = 7;
        }
      else if (state == 7) // $var [type] [bitwidth] [id] [name]
        {
          class cl_cmdline *cmdline = new cl_cmdline(NULL, word, con);
          cmdline->init();

          if (cmdline->params->count != 1)
            {
              con->dd_printf("Unable to interpret %s, ignoring\n", token);
              continue;
            }

          class cl_cmd_arg *param = (class cl_cmd_arg *)(cmdline->params->at(0));

          if (param->as_bit(uc))
            {
              if (param->value.bit.mem->is_address_space())
                add_var(con, id, ((cl_address_space *)param->value.bit.mem)->get_cell(param->value.bit.mem_address), param->value.bit.bitnr_low + width - 1, param->value.bit.bitnr_low);
              else
                con->dd_printf("Unable to interpret %s as a memory/bit reference in an address space\n", token);
            }
          else if (param->as_cell(uc))
            add_var(con, id, param->value.cell, width - 1, 0);
          else
            con->dd_printf("Unable to interpret %s as a memory/bit reference\n", token);

          state = 8;
        }
    }

  return true;
}

bool
cl_vcd::read_word(unsigned int i)
{
  int c;

  do
    {
      c = getc(fd);
    }
  while (c != EOF && strchr(" \n\r\t", c));

  if (c != EOF)
    {
      do
        {
          if (i < sizeof(word) - 1)
            word[i++] = c;
          c = getc(fd);
        }
      while (c > 0 && !strchr(" \n\r\t", c));
    }
  else
    on = false;

  word[i] = '\0';
  return (c != EOF);
}

void
cl_vcd::set_cmd(class cl_cmdline *cmdline, class cl_console_base *con)
{
  class cl_cmd_arg *params[5]= {
    cmdline->param(0),
    cmdline->param(1),
    cmdline->param(2),
    cmdline->param(3),
    cmdline->param(4)
  };

  if (cmdline->syntax_match(uc, BIT)) // ADD
    {
      add_var(con, params[0]->value.bit.mem, params[0]->value.bit.mem_address, params[0]->value.bit.bitnr_high, params[0]->value.bit.bitnr_low);
      return;
    }
  else if (cmdline->syntax_match(uc, MEMORY ADDRESS NUMBER NUMBER)) // ADD
    {
      add_var(con, params[0]->value.memory.memory, params[1]->value.address, params[2]->value.number, params[3]->value.number);
      return;
    }
  else if (cmdline->syntax_match(uc, MEMORY ADDRESS NUMBER)) // ADD
    {
      add_var(con, params[0]->value.memory.memory, params[1]->value.address, params[2]->value.number, params[2]->value.number);
      return;
    }
  else if (cmdline->syntax_match(uc, MEMORY ADDRESS)) // ADD
    {
      add_var(con, params[0]->value.memory.memory, params[1]->value.address, -1, -1);
      return;
    }
  else if (cmdline->syntax_match(uc, CELL NUMBER NUMBER)) // ADD
    {
      add_var(con, params[0]->value.cell, params[1]->value.number, params[2]->value.number);
      return;
    }
  else if (cmdline->syntax_match(uc, CELL NUMBER)) // ADD
    {
      add_var(con, params[0]->value.cell, params[1]->value.number, params[1]->value.number);
      return;
    }
  else if (cmdline->syntax_match(uc, CELL)) // ADD
    {
      add_var(con, params[0]->value.cell, -1, -1);
      return;
    }
  else if (cmdline->syntax_match(uc, STRING BIT)) // DEL|ADD
    {
      char *p1= params[0]->value.string.string;
      if (p1 && *p1)
        {
          if (strcmp(p1, "add") == 0)
            {
              add_var(con, params[1]->value.bit.mem, params[1]->value.bit.mem_address, params[1]->value.bit.bitnr_high, params[1]->value.bit.bitnr_low);
              return;
            }
          else if (strstr(p1, "del") == p1)
            {
              del_var(con, params[1]->value.bit.mem, params[1]->value.bit.mem_address, params[1]->value.bit.bitnr_low, params[1]->value.bit.bitnr_high);
              return;
            }
          else if (strcmp(p1, "new") == 0)
            {
              int nid= params[1]->as_number();
              if (uc->get_hw(id_string, nid, NULL) != NULL)
                {
                  con->dd_printf("Already exists\n");
                  return;
                }
              cl_hw *h= new cl_vcd(uc, nid, id_string);
              h->init();
              uc->add_hw(h);
              return;
            }
        }
    }
  else if (cmdline->syntax_match(uc, STRING MEMORY ADDRESS NUMBER NUMBER)) // DEL|ADD
    {
      char *p1= params[0]->value.string.string;
      if (p1 && *p1)
        {
          if (strcmp(p1, "add") == 0)
            add_var(con, params[1]->value.memory.memory, params[2]->value.address, params[3]->value.number, params[4]->value.number);
          else if (strstr(p1, "del") == p1)
            del_var(con, params[1]->value.memory.memory, params[2]->value.address, params[3]->value.number, params[4]->value.number);
          return;
        }
    }
  else if (cmdline->syntax_match(uc, STRING MEMORY ADDRESS NUMBER)) // DEL|ADD
    {
      char *p1= params[0]->value.string.string;
      if (p1 && *p1)
        {
          if (strcmp(p1, "add") == 0)
            add_var(con, params[1]->value.memory.memory, params[2]->value.address, params[3]->value.number, params[3]->value.number);
          else if (strstr(p1, "del") == p1)
            del_var(con, params[1]->value.memory.memory, params[2]->value.address, params[3]->value.number, params[3]->value.number);
          return;
        }
    }
  else if (cmdline->syntax_match(uc, STRING MEMORY ADDRESS)) // DEL|ADD
    {
      char *p1= params[0]->value.string.string;
      if (p1 && *p1)
        {
          if (strcmp(p1, "add") == 0)
            add_var(con, params[1]->value.memory.memory, params[2]->value.address, -1, -1);
          else if (strstr(p1, "del") == p1)
            del_var(con, params[1]->value.memory.memory, params[2]->value.address, -1, -1);
          return;
        }
    }
  else if (cmdline->syntax_match(uc, STRING CELL NUMBER NUMBER)) // DEL|ADD
    {
      char *p1= params[0]->value.string.string;
      if (p1 && *p1)
        {
          if (strcmp(p1, "add") == 0)
            add_var(con, params[1]->value.cell, params[2]->value.number, params[3]->value.number);
          else if (strstr(p1, "del") == p1)
            del_var(con, params[1]->value.cell, params[2]->value.number, params[3]->value.number);
          return;
        }
    }
  else if (cmdline->syntax_match(uc, STRING CELL NUMBER)) // DEL|ADD
    {
      char *p1= params[0]->value.string.string;
      if (p1 && *p1)
        {
          if (strcmp(p1, "add") == 0)
            add_var(con, params[1]->value.cell, params[2]->value.number, params[2]->value.number);
          else if (strstr(p1, "del") == p1)
            del_var(con, params[1]->value.cell, params[2]->value.number, params[2]->value.number);
          return;
        }
    }
  else if (cmdline->syntax_match(uc, STRING CELL)) // DEL|ADD
    {
      char *p1= params[0]->value.string.string;
      if (p1 && *p1)
        {
          if (strcmp(p1, "add") == 0)
            add_var(con, params[1]->value.cell, -1, -1);
          else if (strstr(p1, "del") == p1)
            del_var(con, params[1]->value.cell, -1, -1);
          return;
        }
    }
  else if (cmdline->syntax_match(uc, STRING NUMBER STRING)) // TIMESCALE
    {
      char *p1= params[0]->value.string.string;
      if (p1 && *p1 &&
          (strcmp(p1, "timescale") == 0))
        {
          char *p2= params[2]->value.string.string;
          if (!strcmp(p2, "fs"))
            timescale = params[1]->value.number * 1e15;
          else if (!strcmp(p2, "ps"))
            timescale = params[1]->value.number * 1e12;
          else if (!strcmp(p2, "ns"))
            timescale = params[1]->value.number * 1e9;
          else if (!strcmp(p2, "us"))
            timescale = params[1]->value.number * 1e6;
          else if (!strcmp(p2, "ms"))
            timescale = params[1]->value.number * 1e3;
          else
            con->dd_printf("Units must be ms, us, ns, ps or fs\n");
          return;
        }
    }
  else if (cmdline->syntax_match(uc, STRING STRING)) // TIMESCALE AUTO, FILE, MOD
    {
      char *p1= params[0]->value.string.string;
      char *p2= params[1]->value.string.string;
      if (p1 && p2 &&
          !strcmp(p1, "timescale") &&
          !strcmp(p2, "auto"))
        {
          timescale = 0;
          return;
        }
      if (started)
        {
          con->dd_printf("Already started\n");
          return;
        }
      if (p1 && *p1)
        {
          if (!p2 || !*p2)
            {
              con->dd_printf("Name missing\n");
              return;
            }
          if (strstr(p1, "mod") == p1)
            {
              modul= chars(p2);
              return;
            }
          if ((strcmp(p1, "output") == 0) ||
              (strcmp(p1, "file") == 0) ||
              (strcmp(p1, "input") == 0))
            {
              if (filename)
                free(filename);
              filename = strdup(p2);
              state = (!strcmp(p1, "input") ? 0 : -1);
              return;
            }
        }
    }
  else if (cmdline->syntax_match(uc, STRING)) // [RE]START, PAUSE, STOP
    {
      char *p1= params[0]->value.string.string;
      if (p1 && *p1)
        {
          if ((strstr(p1, "re") == p1) ||
              (strcmp(p1, "start") == 0))
            {
              if (!started)
                {
                  if (!filename)
                    {
                      con->dd_printf("No file specified\n");
                      return;
                    }
                  if (state != -1)
                    {
                      if ((fd= fopen(filename, "r")) == NULL)
                        {
                          con->dd_printf("File open error\n");
                          return;
                        }

                      if (!parse_header(con))
                        return;
                    }
                  else
                    {
                      if ((fd= fopen(filename, "w")) == NULL)
                        {
                          con->dd_printf("File open error\n");
                          return;
                        }

                      if (!timescale)
                        {
                          timescale = 1e3;
                          while (timescale * 1.0 / uc->xtal < 1.0)
                            timescale *= 1000.0;
                          if (fmod(timescale * 1.0 / uc->xtal, 1.0) > 0.0)
                            timescale *= 1000.0;
                        }

                      // generate vcd file header
                      time_t now = time(NULL);
                      fprintf(fd, "$date\n\t%s$end\n$version\n\tucsim\n$end\n$timescale ", ctime(&now));
                      if (timescale >= 1e15)
                        fprintf(fd, "%.0ffs", timescale * 1e-15);
                      else if (timescale >= 1e12)
                        fprintf(fd, "%.0fps", timescale * 1e-12);
                      else if (timescale >= 1e9)
                        fprintf(fd, "%.0fns", timescale * 1e-9);
                      else if (timescale >= 1e6)
                        fprintf(fd, "%.0fus", timescale * 1e-6);
                      else if (timescale >= 1e3)
                        fprintf(fd, "%.0fms", timescale * 1e-3);
                      else
                        fprintf(fd, "%.0fms", timescale * 1e3);

                      fprintf(fd, " $end\n$scope module %s $end\n", modul.c_str());

                      for (class cl_vcd_var *var = vars; var; var = var->next_var)
                        {
                          fprintf(fd, "$var wire %d %c ",
                                      (var->bitnr_low >= 0 ? var->bitnr_high - var->bitnr_low + 1 : var->cell->get_width()),
                                      var->var_id);

                          t_addr addr = 0;
                          cl_address_space *as = uc->address_space(var->cell, &addr);
                          t_index i;

                          if (var->bitnr_high >= 0)
                            {
                              if (uc->vars->by_addr.search(as, addr, var->bitnr_high, var->bitnr_low, i))
                                {
                                  fprintf(fd, "%s $end\n", uc->vars->by_addr.at(i)->get_name());
                                  continue;
                                }
                            }

                          if (uc->vars->by_addr.search(as, addr, -1, -1, i))
                            fprintf(fd, "%s", uc->vars->by_addr.at(i)->get_name());
                          else
                            {
                              fprintf(fd, "%s[", (as ? as->get_name() : "?"));
                              fprintf(fd, (as ? as->addr_format : "0x06x"), addr);
                              fprintf(fd, "]");
                            }

                          if (var->bitnr_high == var->bitnr_low)
                            fprintf(fd, ".%d", var->bitnr_low);
                          else if (var->bitnr_low != 0 || var->bitnr_high != var->cell->get_width() - 1)
                            fprintf(fd, "[%d:%d]", var->bitnr_high, var->bitnr_low);

                          fprintf(fd, " $end\n");
                        }

                      fputs("$upscope $end\n$enddefinitions $end\n$dumpvars\n", fd);

                      event = uc->get_rtime();
                      for (class cl_vcd_var *var = vars; var; var = var->next_var)
                        report(var, var->cell->get());
                      event = 0.0;
                      fputs("$end\n", fd);
                    }
                }

              if (!started || paused)
                {
                  double now = uc->get_rtime();
                  starttime += now;
                  event += now;
                }

              started = true;
              paused = false;
              if (state >= 0)
                on = true;

              return;
            }
          if (strstr(p1, "paus") == p1)
            {
              if (started)
                {
                  if (paused)
                    {
                      con->dd_printf("Unpaused\n");
                      double now = uc->get_rtime();
                      starttime += now;
                      event += now;
                      paused = false;
                      if (state >= 0)
                        on = true;
                    }
                  else
                    {
                      con->dd_printf("Paused\n");
                      double now = uc->get_rtime();
                      starttime -= now;
                      event -= now;
                      paused = true;
                      on = false;
                    }
                }
              else
                con->dd_printf("Not started");
              return;
            }
          if (strcmp(p1, "stop") == 0)
            {
              if (started)
                {
                  on = false;
                  starttime = 0.0;
                  if (fd)
                    fclose(fd);
                  fd= NULL;
                  if (state != -1)
                    clear_vars();
                  else if (!vars)
                    reset_next_var_id();
                }
              started= paused= false;
              return;
            }
          if (strcmp(p1, "break") == 0)
            {
              dobreak= !dobreak;
              con->dd_printf("Break on events %s\n", (dobreak ? "enabled" : "disabled"));
              return;
            }
          if (strcmp(p1, "info") == 0)
            {
              print_info(con);
              return;
            }
        }
    }

    con->dd_printf("set hardware vcd[id] add memory address [ [bit_high] bit_low]\n");
    con->dd_printf("set hardware vcd[id] del[ete] memory address [ [bit_high] bit_low]\n");
    con->dd_printf("set hardware vcd[id] input \"vcd_file_name\"\n");
    con->dd_printf("set hardware vcd[id] output|file \"vcd_file_name\"\n");
    con->dd_printf("set hardware vcd[id] mod[ule] module_name\n");
    con->dd_printf("set hardware vcd[id] start\n");
    con->dd_printf("set hardware vcd[id] pause\n");
    con->dd_printf("set hardware vcd[id] [re]start\n");
    con->dd_printf("set hardware vcd[id] stop\n");
    con->dd_printf("set hardware vcd[id] break\n");
    con->dd_printf("set hardware vcd[id] new id\n");
}

void
cl_vcd::report(class cl_vcd_var *var, t_mem v)
{
  double now = uc->get_rtime();
  if (event != now)
    {
      event = now;
      fprintf(fd, "#%.0f\n", event * timescale);
    }

  if (var->bitnr_low == var->bitnr_high)
    {
      fprintf(fd, "%c%c\n", ((v & var->bitmask) ? '1' : '0'), var->var_id);
    }
  else
    {
      putc('b', fd);
      for (int i = var->bitnr_high; i >= var->bitnr_low; i--)
        putc((v & (1U << i)) ? '1' : '0', fd);
      fprintf(fd, " %c\n", var->var_id);
    }
}

int
cl_vcd::tick(int cycles)
{
  if (state == -1 || !fd || !started || paused)
    return 0;

  double now = uc->get_rtime();
  bool event_occurred = false;

  while (event - now <= 0.000000000000001)
    {
      t_mem value = 0;
      while (read_word(0U))
        {
          if (state == 0 && word[0] != '1' && word[0] != '0')
            {
              if (word[0] == '#')
                {
                  event = starttime + timescale * strtod(&word[1], NULL);
                  break;
                }
              else if (word[0] == 'b' || word[0] == 'B')
                {
                  // Bit-vector value, next word is ID
                  for (char *ptr = word; *ptr; ptr++)
                    value = (value << 1) | (*ptr == '1' ? 1 : 0);
                  state = 1;
                }
              else if (word[0] == 'r' || word[0] == 'R')
                {
                  // Real value, next word is ID
                  // Unsupported
                  state = 1;
                }
            }
          else if (state == 1 || word[0] == '1' || word[0] == '0')
            {
              // ID for preceeding value or bit followed by ID
              char id;
              if (word[0] == '1')
                {
                  value = 1;
                  id = word[1];
                }
              else if (word[0] == '0')
                {
                  value = 0;
                  id = word[1];
                }
              else
                id = word[0];

              for (class cl_vcd_var *var = vars; var; var = var->next_var)
                {
                  if (var->var_id == id)
                    {
                      var->cell->write((var->cell->get() & (~var->bitmask)) | ((value << var->bitnr_low) & var->bitmask));
                      event_occurred = true;
                      break;
                    }
                }

              state = 0;
            }
          else if (state == 2)
            {
              // ID for preceeding value
              // Unsupported
              state = 0;
            }
        }
    }

  if (event_occurred && dobreak && !uc->fbrk_at(uc->PC))
    {
      // If break-on-event is enabled ensure that we are going to
      // break before the next instruction.
      class cl_brk *b= new cl_fetch_brk(uc->rom,
                          uc->make_new_brknr(),
                          uc->PC, brkDYNAMIC, 1);

      b->init();
      uc->fbrk->add(b);
      b->activate();
    }

  return 0;
}

void
cl_vcd::print_info(class cl_console_base *con)
{
  con->dd_printf("%s[%d] value change dump\n", id_string, id);
  con->dd_printf("  Modul:      %s\n", modul.c_str());
  con->dd_printf("  Started:    %s\n", (started ? "YES" : "no"));
  con->dd_printf("  Paused:     %s\n", (paused ? "YES" : "no"));
  con->dd_printf("  File:       %s\n", (filename ? filename : "(none)"));

  if (state != -1)
    {
      con->dd_printf("  Mode:       input\n");
      con->dd_printf("  Break:      %s\n", (dobreak ? "enabled" : "disabled"));

      if (timescale >= 1e-3)
        con->dd_printf("  Time scale: %.0f ms\n", timescale * 1e3);
      else if (timescale >= 1e-6)
        con->dd_printf("  Time scale: %.0f us\n", timescale * 1e6);
      else if (timescale >= 1e-9)
        con->dd_printf("  Time scale: %.0f ns\n", timescale * 1e9);
      else if (timescale >= 1e-12)
        con->dd_printf("  Time scale: %.0f ps\n", timescale * 1e12);
      else
        con->dd_printf("  Time scale: %.0f fs\n", timescale * 1e15);
    }
  else
    {
      con->dd_printf("  Mode:       output\n");

      if (!timescale)
        con->dd_printf("  Time scale: auto set on start\n");
      else
        {
          if (timescale >= 1e15)
            con->dd_printf("  Time scale: %.0f fs\n", timescale * 1e-15);
          else if (timescale >= 1e12)
            con->dd_printf("  Time scale: %.0f ps\n", timescale * 1e-12);
          else if (timescale >= 1e9)
            con->dd_printf("  Time scale: %.0f ns\n", timescale * 1e-9);
          else if (timescale >= 1e6)
            con->dd_printf("  Time scale: %.0 us\n", timescale * 1e-6);
          else
            con->dd_printf("  Time scale: %.0f ms\n", timescale * 1e-3);
        }
    }

  con->dd_printf("  Start time: %.15f s\n", starttime);
  con->dd_printf("  %s event: %.15f s\n", (state != -1 ? "Next" : "Last"), event);
  con->dd_printf("  Simul time: %.15f s\n", uc->get_rtime());

  con->dd_printf("  Variables:\n");
  con->dd_printf("    Address           Symbol\n");
  for (class cl_vcd_var *var = vars; var; var = var->next_var)
    {
      t_addr a= 0;
      cl_address_space *as = uc->address_space(var->cell, &a);

      con->dd_printf("    %s[", (as ? as->get_name() : "?"));
      con->dd_printf((as ? as->addr_format : "0x06x"), a);
      con->dd_printf("]");
      if (var->bitnr_high < 0)
        con->dd_printf("      ");
      else if (var->bitnr_high == var->bitnr_low)
        con->dd_printf(".%d    ", var->bitnr_high);
      else if (var->bitnr_low != 0 || var->bitnr_high != var->cell->get_width() - 1)
        con->dd_printf("[%d:%d] ", var->bitnr_high, var->bitnr_low);
      else
        con->dd_printf("      ");

      int i;

      if (uc->vars->by_addr.search(as, a, var->bitnr_high, var->bitnr_low, i))
        con->dd_printf("%s\n", uc->vars->by_addr.at(i)->get_name());
      else
        {
          if (uc->vars->by_addr.search(as, a, -1, -1, i))
            {
              const char *cname = uc->vars->by_addr.at(i)->get_name();
              con->dd_printf("%s", cname);
              if (var->bitnr_high >= 0)
                {
                  if (var->bitnr_high == var->bitnr_low)
                    con->dd_printf(".%d", var->bitnr_high);
                  else if (var->bitnr_low != 0 || var->bitnr_high != var->cell->get_width() - 1)
                    con->dd_printf("[%d:%d]", var->bitnr_high, var->bitnr_low);
                }
            }
          con->dd_printf("\n");
        }
    }

  //print_cfg_info(con);
}


/* End of sim.src/vcd.cc */
