BEGIN {
	 print "/* Generated file DO NOT Edit */"
	 print "/* To Make changes to rules   */"
	 print "/* edit file SDCCpeeph.def    */"
}

/^\/\// { next}

{ printf "\"" ;
  printf "%s",$0;
  print  "\\n\""; 
}


