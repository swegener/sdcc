PRJDIR = ../..

clean:
	rm -f *core *[%~] *.[oa] *.output
	rm -f .[a-z]*~ \#*
	rm -f $(PRJDIR)/bin/sdcclib$(EXEEXT)

distclean realclean: clean
