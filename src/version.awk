BEGIN {
print "/*"
print " * version.h"
print " * control long build version number"
print " *"
print " * Created automatically with version.awk script"
print " *"
print " */"
print ""
print "#ifndef __VERSION_H__"
print "#define __VERSION_H__"
print ""

FS="[ \t.]"
}

/Revision/ { printf "#define SDCC_BUILD_NUMBER\t\"%s\"\n", $3 }

END {
print ""
print "#ifndef SDCC_BUILD_NUMBER"
print "#define SDCC_BUILD_NUMBER	\"0\""
print "#endif"
print ""
print "#endif"
}
