set error non-classified off
set error unknown_code off
set error memory off
set error stack off
break xram r 0x7654
port0_on=0
port1_on=0
port2_on=0
port3_on=0
run
state
quit
