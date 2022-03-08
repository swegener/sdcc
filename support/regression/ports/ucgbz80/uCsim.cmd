set error non-classified off
set error unknown_code off
set error memory off
set error stack off
set hw simif xram 0xff03
pc 0x100
run
state
quit
