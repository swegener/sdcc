 virtual void fill_0_89() { page0[0x89 ]= wrap_0_89; set_disass(0, 0x89, "ADC A,%b", ' ', 2); }
 virtual void fill_0_99() { page0[0x99 ]= wrap_0_99; set_disass(0, 0x99, "ADC A,%d", ' ', 2); }
 virtual void fill_0_a9() { page0[0xa9 ]= wrap_0_a9; set_disass(0, 0xa9, "ADC A,%p", ' ', -2); }
 virtual void fill_0_b9() { page0[0xb9 ]= wrap_0_b9; set_disass(0, 0xb9, "ADC A,%e", ' ', 3); }
 virtual void fill_0_b7() { page0[0xb7 ]= wrap_0_b7; set_disass(0, 0xb7, "%T", ' ', 2); }
