PRJ		= t1 t2 t3 t4 s1 s2 s3 \
		  stm8l_disc_serial \
		  stm8l_disc_led

all:
	for p in $(PRJ); do \
		$(MAKE) -f $$p.mk ;\
	done

clean:
	for p in $(PRJ); do \
		$(MAKE) -f $$p.mk clean ;\
	done
