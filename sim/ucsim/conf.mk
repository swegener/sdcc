#
# Makefile targets to remake configuration
#

freshconf: $(srcdir)/configure main.mk ddconfig.h

$(srcdir)/configure: $(srcdir)/configure.in
	cd $(srcdir) && $(SHELL) autoconf

main.mk: $(srcdir)/main_in.mk $(srcdir)/configure.in
	$(SHELL) ./config.status

ddconfig.h: ddconfig_in.h config.status
	$(SHELL) ./config.status

config.status: $(srcdir)/configure
	$(SHELL) ./config.status --recheck

# End of conf.mk
