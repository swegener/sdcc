/*
  dbuf.c - Dynamic buffer implementation
  version 1.1.2, May 17th, 2006

  Copyright (c) 2002-2006 Borut Razem

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


#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dbuf.h"


/*
 * Assure that the buffer is large enough to hold
 * size bytes; enlarge it if necessary.
 */

static int dbuf_expand(struct dbuf_s *dbuf, size_t size)
{
  assert(dbuf->alloc != 0);
  assert(dbuf->buf != NULL);

  if (dbuf->len + size > dbuf->alloc) {
    /* new_allocated_size = current_allocated_size * 2^n */
    /* can this be optimized? */
    while (dbuf->len + size >= dbuf->alloc)
      dbuf->alloc += dbuf->alloc;

    if ((dbuf->buf = realloc(dbuf->buf, dbuf->alloc)) == NULL)
      return 0;
  }

  return 1;
}


/*
 * Initialize the dbuf structure and
 * allocate buffer to hold size bytes.
 */

int dbuf_init(struct dbuf_s *dbuf, size_t size)
{
  assert(size != 0);

  if (size == 0)
    size = 1;

  dbuf->len = 0;
  dbuf->alloc = size;
  return ((dbuf->buf = malloc(dbuf->alloc)) != NULL);
}


/*
 * Allocate new dbuf structure on the heap
 * and initialize it.
 *
 * See: dbuf_delete()
 */

struct dbuf_s *dbuf_new(size_t size)
{
  struct dbuf_s *dbuf;

  dbuf = (struct dbuf_s *)malloc(sizeof(struct dbuf_s));
  if (dbuf != NULL) {
    if (dbuf_init(dbuf, size) == 0) {
      free(dbuf);
      return NULL;
    }
  }
  return dbuf;
}


/*
 * Set the buffer size. Buffer size can be only decreased.
 */

int dbuf_set_size(struct dbuf_s *dbuf, size_t size)
{
  assert(dbuf != NULL);
  assert(dbuf->alloc != 0);
  assert(size < dbuf->len);

  if (size <= dbuf->len) {
    dbuf->len = size;
    return 1;
  }

  return 0;
}


/*
 * Append the buf to the end of the buffer.
 */

int dbuf_append(struct dbuf_s *dbuf, const void *buf, size_t size)
{
  assert(dbuf != NULL);
  assert(dbuf->alloc != 0);
  assert(dbuf->buf != NULL);

  if (dbuf_expand(dbuf, size) != 0) {
    memcpy(&(((char *)dbuf->buf)[dbuf->len]), buf, size);
    dbuf->len += size;
    return 1;
  }

  return 0;
}


/*
 * Add '\0' character at the end of the buffer without
 * count it in the dbuf->len.
 */

const char *dbuf_c_str(struct dbuf_s *dbuf)
{
  assert(dbuf != NULL);
  assert(dbuf->alloc != 0);
  assert(dbuf->buf != NULL);

  /* only if not already null terminated */
  if (dbuf->len == dbuf->alloc ||
    ((char *)dbuf->buf)[dbuf->len] != '\0') {
    dbuf_expand(dbuf, 1);
    ((char *)dbuf->buf)[dbuf->len] = '\0';
  }

  return dbuf->buf;
}


/*
 * Get the buffer pointer.
 */

const void *dbuf_get_buf(struct dbuf_s *dbuf)
{
  assert(dbuf != NULL);
  assert(dbuf->alloc != 0);
  assert(dbuf->buf != NULL);

  return dbuf->buf;
}


/*
 * Get the buffer size.
 */

size_t dbuf_get_size(struct dbuf_s *dbuf)
{
  assert(dbuf != NULL);
  assert(dbuf->alloc != 0);
  assert(dbuf->buf != NULL);

  return dbuf->len;
}


/*
 * Trim the allocated buffer to required size
 */

int dbuf_trim(struct dbuf_s *dbuf)
{
  void *buf;

  assert(dbuf != NULL);
  assert(dbuf->alloc != 0);
  assert(dbuf->buf != NULL);

  buf = realloc(dbuf->buf, dbuf->len);

  if (buf != NULL) {
    dbuf->alloc = dbuf->len;
    dbuf->buf = buf;
  }

  return buf != NULL;
}


/*
 * Detach the buffer from dbuf structure.
 * The dbuf structure can be reused by
 * reinitializing it.
 *
 * See: dbuf_init()
 */

void *dbuf_detach(struct dbuf_s *dbuf)
{
  void *ret;

  assert(dbuf != NULL);
  assert(dbuf->alloc != 0);
  assert(dbuf->buf != NULL);

  ret = dbuf->buf;
  dbuf->buf = NULL;
  dbuf->len = 0;
  dbuf->alloc = 0;

  return ret;
}


/*
 * Destroy the dbuf structure and
 * free the buffer
 */

void dbuf_destroy(struct dbuf_s *dbuf)
{
  free(dbuf_detach(dbuf));
}


/*
 * Delete dbuf structure on the heap:
 * destroy it and free the allocated space.
 * The user's responsablity is not to use
 * the pointer any more: the best think to do
 * is to set the pointer to NULL value.
 *
 * See dbuf_new()
 */

void dbuf_delete(struct dbuf_s *dbuf)
{
  dbuf_destroy(dbuf);
  free(dbuf);
}
