include ../Makefile.common

MANUAL = sdccman
TSS = test_suite_spec

all: $(MANUAL).html $(MANUAL).pdf $(MANUAL).txt \
	$(TSS).html $(TSS).pdf $(TSS).txt

install:
	$(INSTALL) -d $(docdir)
	cp -rf *.html *.txt *.pdf $(MANUAL).html $(TSS).html z80 avr $(docdir)

uninstall:
	rm -rf $(docdir)


$(MANUAL).html: $(MANUAL).tex $(MANUAL).ind ;#$(MANUAL).glo
	mkdir -p $@
	latex2html -split 5 -show_section_numbers -dir $(MANUAL).html $(MANUAL)

$(TSS).html: $(TSS).tex
	mkdir -p $@
	latex2html -split 0 -dir $(TSS).html $(TSS)

%.txt: %.lyx
	lyx -e text $<

%.pdf: %.dvi
	which pdflatex > /dev/null && pdflatex $* || dvipdf $*

%.dvi: %.tex
	latex $<

%.tex: %.lyx
	lyx -e latex $<

%.ind: %.dvi
	makeindex -s l2hidx.ist $*

%.glo: %.dvi
	# the glossary, not implemented yet
	# makeindex -s l2hglo.ist -o $@ $<

# Deleting all files created by building the program
# --------------------------------------------------
clean:
# The directories are still in CVS, so don't delete them
#	rm -rf $(MANUAL).html $(TSS).html
	rm -f $(MANUAL).html/* $(TSS).html/*
	rm -f $(MANUAL).txt $(TSS).txt *.pdf *.dvi *.tex *.ind *.glo *.aux *.idx *.ilg *.log *.toc

# Deleting all files created by configuring or building the program
# -----------------------------------------------------------------
distclean: clean

# Like clean but some files may still exist
# -----------------------------------------
mostlyclean: clean

# Deleting everything that can reconstructed by this Makefile. It deletes
# everything deleted by distclean plus files created by bison, etc.
# -----------------------------------------------------------------------
realclean: distclean

