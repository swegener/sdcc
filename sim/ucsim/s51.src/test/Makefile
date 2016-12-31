PRJ		= test_mdu517

all:
	for p in $(PRJ); do \
		$(MAKE) -f $$p.mk ;\
	done

clean:
	for p in $(PRJ); do \
		$(MAKE) -f $$p.mk clean ;\
	done
