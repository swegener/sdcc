clean:
	rm -f *core *[%~] *.[oa] *.output
	rm -f .[a-z]*~ \#*
	rm -f packihx

distclean: clean
	rm -f config.*
