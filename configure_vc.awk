#!/usr/bin/awk

# configure_vc.awk - Genarate sdcc_vc.h using sdcc_vc_in.h as template
#                    and insert the version number definitions from .version
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
  FS=".";
  getline <".version";
  h = $1;
  l = $2;
  p = $3;
  print "/*"
  print " * sdcc_vc.h"
  print " * Generated automatically by configure_vc.awk, DO NOT edit!"
  print " * To make changes to sdcc_vc.h edit sdcc_vc_in.h instead."
  print " */" 
  print "" 
}

/^#undef SDCC_VERSION_HI$/ {
  print("#define SDCC_VERSION_HI " h);
  next
}

/^#undef SDCC_VERSION_LO$/ {
  print("#define SDCC_VERSION_LO " l);
  next
}

/^#undef SDCC_VERSION_P$/ {
  print("#define SDCC_VERSION_P " p);
  next
}

/^#undef SDCC_VERSION_STR$/ {
  print("#define SDCC_VERSION_STR " "\"" h "." l "." p "\"");
  next
}

{
  print
}
