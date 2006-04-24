all:
	$(MAKE) -C ../mcs51 all
	cp ../mcs51/*.rel ../mcs51/*.lib .
	touch dummy.lib
	touch dummy.rel

clean:
	rm -f *.rel *.lib

distclean: clean
	rm -f Makefile