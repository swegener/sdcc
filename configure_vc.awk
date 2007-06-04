#!/usr/bin/awk

# configure_vc.awk - Genarate sdcc_vc.h using sdcc_vc_in.h as template
#                    and insert the version number definitions from configure.in
#
# Written By - Borut Razem borut.razem@siol.net
#
# This file is part of sdcc.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

BEGIN {
  # get the values from configure.in

  while (getline <"configure.in" > 0) {
    if ($0 ~ "^AC_INIT\\(.*\\)") {
      package = gensub("^AC_INIT\\(\\[([^]]*)\\].*", "\\1", "1", $0);
      version = gensub("^AC_INIT\\(\\[[^]]*\\], \\[([^]]*)\\].*", "\\1", "1", $0);
      bugreport = gensub("^AC_INIT\\(\\[[^]]*\\], \\[[^]]*\\], \\[([^]]*)\\].*", "\\1", "1", $0);

      version_high = gensub(/^([^.]*).([^.]*).([^.]*)/, "\\1", "1", version)
      version_lo = gensub(/^([^.]*).([^.]*).([^.]*)/, "\\2", "1", version)
      version_patch = gensub(/^([^.]*).([^.]*).([^.]*)/, "\\3", "1", version)
    }
  }

  print "/*"
  print " * sdcc_vc.h"
  print " * Generated automatically by configure_vc.awk, DO NOT edit!"
  print " * To make changes to sdcc_vc.h edit sdcc_vc_in.h instead."
  print " */" 
  print "" 
}

/^#undef SDCC_VERSION_HI$/ {
  print("#define SDCC_VERSION_HI " version_high);
  next
}

/^#undef SDCC_VERSION_LO$/ {
  print("#define SDCC_VERSION_LO " version_lo);
  next
}

/^#undef SDCC_VERSION_P$/ {
  print("#define SDCC_VERSION_P " version_patch);
  next
}

/^#undef SDCC_VERSION_STR$/ {
  print("#define SDCC_VERSION_STR " "\"" version "\"");
  next
}

{
  print
}
