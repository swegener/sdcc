include ../Makefile.common

MANUAL = sdccman
TSS = test_suite_spec

all: $(MANUAL).html $(MANUAL).pdf $(MANUAL).txt \
	$(TSS).html $(TSS).pdf $(TSS).txt

install:
	$(INSTALL) -d $(docdir)
	cp -rf *.html *.txt z80 avr $(docdir)
	$(shell if [ -f $(MANUAL).html ]; then cp -rf $(MANUAL).html $(docdir); fi)
	$(shell if [ -f $(TSS).html    ]; then cp -rf $(TSS).html    $(docdir); fi)
	$(shell if [ -f *.pdf          ]; then cp -rf *.pdf          $(docdir); fi)

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

include clean.mk
