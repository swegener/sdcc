long leftShiftLong (long l) { 
  /* PENDING: Disabled.
     return (l << 3); 
  */
  return l;
} 


int leftShiftIntMasked (int v) { 
  /* PENDING: Disabled.
     return ((v & 0xff00U) << 3); 
  */
  return v;
} 

int leftShiftIntMasked2 (int v) { 
    return ((v & 0xff) << 8); 
} 


int leftShiftIntMasked3 (int v) { 
    return ((v & 0xff) << 3); 
}
