/*
  dbuf.h - Dynamic buffer interface
  version 1.1.0, March 28th, 2003

  Copyright (c) 2003 Borut Razem

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Borut Razem
  borut.razem@siol.net
*/


#ifndef __DBUF_H
#define __DBUF_H

#ifndef _WIN32
#include <ansidecl.h>
#else
#define PARAMS(paramlist)   paramlist
#endif

struct dbuf_s {
  size_t alloc;   /* size of allocated buffer in bytes */
  size_t len;     /* actual size of the buffer in bytes */
  void *buf;      /* pointer to the buffer, allocated on heap */
};


#ifdef __cplusplus
extern "C" {
#endif

struct dbuf_s *dbuf_new PARAMS ((size_t size));
int dbuf_init PARAMS ((struct dbuf_s *dbuf, size_t size));
int dbuf_set_size PARAMS ((struct dbuf_s *dbuf, size_t size));
int dbuf_append PARAMS ((struct dbuf_s *dbuf, const void *buf, size_t size));
const void *dbuf_get_buf PARAMS ((struct dbuf_s *dbuf));
size_t dbuf_get_size PARAMS ((struct dbuf_s *dbuf));
const char *dbuf_c_str PARAMS ((struct dbuf_s *dbuf));
int dbuf_trim(struct dbuf_s *dbuf);
void *dbuf_detach PARAMS ((struct dbuf_s *dbuf));
void dbuf_destroy PARAMS ((struct dbuf_s *dbuf));
void dbuf_delete PARAMS ((struct dbuf_s *dbuf));

#ifdef __cplusplus
}
#endif

#endif  /* __DBUF_H */
