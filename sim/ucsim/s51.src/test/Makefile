PRJ		= test_mdu517 test_mdu88x \
		  stn stp strr sts stt stxr stxrw stxw

all:
	for p in $(PRJ); do \
		$(MAKE) -f $$p.mk ;\
	done

clean:
	for p in $(PRJ); do \
		$(MAKE) -f $$p.mk clean ;\
	done
	rm -f *.out *.sout *.tim *.csv *.sim
	rm -f st*[56][0-9][0-9].cmd st*[56]*.txt
