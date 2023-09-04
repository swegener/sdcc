set error non-classified off
set error unknown_code off
set error memory off
set error stack off
set opt selfjump_stop 0
break xram r 0x7654
set hw simif xram 0x7654
expr /n port0_on=0
expr /n port1_on=0
expr /n port2_on=0
expr /n port3_on=0
run
state
quit
