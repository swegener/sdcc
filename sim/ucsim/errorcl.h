/*
 * Simulator of microcontrollers (errorcl.h)
 *
 * Copyright (C) 2001,01 Drotos Daniel, Talker Bt.
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

#ifndef ERRORCL_HEADER
#define ERRORCL_HEADER

// prj
#include "pobjcl.h"
#include "stypes.h"

extern struct id_element error_on_off_names[];

enum error_on_off {
  ERROR_PARENT,
  ERROR_ON,
  ERROR_OFF
};

const int err_stop= (err_unknown|err_error);

/*
#define ERROR_METHODS(NAME) \
enum error_type \
cl_error_##NAME ::get_type(void)\
{\
  return(err_##NAME##_class.get_type());\
}\
bool \
cl_error_##NAME ::is_on(void)\
{\
  return(err_##NAME##_class.is_on());\
}
*/

#define ERROR_CLASS_DECL(NAME) \
extern class cl_error_class error_##NAME##_class;\
class cl_error_##NAME

/*
#define ERROR_CLASS_DEF(TYPE,NAME,CLASS_NAME) \
class cl_error_class error_##NAME##_class(TYPE, CLASS_NAME);
*/
/*
#define ERROR_CLASS_DEF_ON(TYPE,NAME,CLASS_NAME,ON) \
class cl_error_class error_##NAME##_class(TYPE, CLASS_NAME, ON);
*/

#define ERROR_CLASS_DEF_PARENT(TYPE,NAME,CLASS_NAME,PARENT) \
class cl_error_class error_##NAME##_class(TYPE, CLASS_NAME, &(PARENT));

#define ERROR_CLASS_DEF_PARENT_ON(TYPE,NAME,CLASS_NAME,PARENT,ON) \
class cl_error_class error_##NAME##_class(TYPE, CLASS_NAME, &(PARENT), ON);


extern class cl_list *registered_errors;

class cl_error_class: public cl_base
{
protected:
  enum error_type type;
  //char *name;
  enum error_on_off on;
public:
  cl_error_class(enum error_type typ, char *aname);
  cl_error_class(enum error_type typ, char *aname,
		 enum error_on_off be_on);
  cl_error_class(enum error_type typ, char *aname,
		 class cl_error_class *parent);
  cl_error_class(enum error_type typ, char *aname,
		 class cl_error_class *parent,
		 enum error_on_off be_on);
  
  enum error_on_off get_on(void) { return(on); }
  void set_on(enum error_on_off val);
  bool is_on(void);
  enum error_type get_type(void);
  char *get_type_name(void);
  //char *get_name(void);
};

extern class cl_error_class error_class_base;


class cl_commander; //forward

class cl_error: public cl_base
{
protected:
  class cl_error_class *classification;
public:
  bool inst;	// Occured during instruction execution
  t_addr PC;	// Address of the instruction
public:
  cl_error(void);
  virtual ~cl_error(void);
  virtual int init(void);
 
public:
  virtual enum error_type get_type(void);
  virtual enum error_on_off get_on(void);
  virtual bool is_on(void);
  virtual class cl_error_class *get_class(void) { return(classification); }

  virtual void print(class cl_commander *c);
  virtual char *get_type_name();
};


#endif

/* End of sim.src/errorcl.h */
