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
	latex2html -split 5 -show_section_numbers -dir $(MANUAL).html $(MANUAL)

$(TSS).html: $(TSS).tex
	latex2html -split 0 -dir $(TSS).html $(TSS)

%.txt: %.lyx
	lyx -e text $<

%.pdf: %.dvi
	pdflatex $*

%.dvi: %.tex
	latex $<

%.tex: %.lyx
	lyx -e latex $<

%.ind: %.dvi
	makeindex -s l2hidx.ist $*

%.glo: %.dvi
	# the glossary, not implemented yet
	# makeindex -s l2hglo.ist -o $@ $<


clean:
	# remove intermediate file, not the final pdf's and html's
	# because these are needed for the distribution
	rm -rf *.tex *.aux *.dvi *.idx *.ilg *.ind *.log *.toc *~ \#* \
		*.ps */*.css */*.pl *.gif core

superclean:
	$(MAKE) clean
	# now get rid of the generated pdf's and html's as well
	rm -rf *.pdf $(MANUAL).html $(TSS).html