PRJDIR = ..

clean:
	rm -f *core *[%~] *.[oa] *.output
	rm -f .[a-z]*~ \#*
	rm -f $(PRJDIR)/bin/packihx$(EXEEXT)

distclean realclean: clean
	rm -f config.* Makefile
