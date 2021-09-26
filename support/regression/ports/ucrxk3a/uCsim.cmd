set error non-classified off
set error unknown_code off
set error memory off
set error stack off
set hw simif rom 0x7fff
expr IIR=1
run
state
quit
