import sys, re
import string

lines = sys.stdin.readlines()

failures = 0
cases = 0
tests = 0

for line in lines:
    if (re.search(r'^--- Summary:', line)):
        (summary, data, rest) = re.split(r':', line)
        (nfailures, ntests, ncases) = re.split(r'/', data)
        failures = failures + string.atof(nfailures)
        tests = tests + string.atof(ntests)
        cases = cases + string.atof(ncases)

print "%.0f failues, %.0f tests, %.0f test cases" % (failures, tests, cases)
