include ../Makefile.common

MANUAL = sdccman
TSS = test_suite_spec
CDB = cdbfileformat

all: $(MANUAL).pdf $(MANUAL).html $(MANUAL).txt \
	$(TSS).pdf $(TSS).html $(TSS).txt \
	$(CDB).pdf $(CDB).html $(CDB).txt

install:
	$(INSTALL) -d $(docdir)
	cp -rf *.txt z80 avr $(docdir)
	$(shell if [ -f $(MANUAL).html ]; then cp -rf $(MANUAL).html $(docdir); fi)
	$(shell if [ -f $(TSS).html    ]; then cp -rf $(TSS).html    $(docdir); fi)
	$(shell if [ -f $(CDB).html    ]; then cp -rf $(CDB).html    $(docdir); fi)
	$(shell if [ -f *.pdf          ]; then cp -rf *.pdf          $(docdir); fi)

uninstall:
	rm -rf $(docdir)

$(MANUAL).html: $(MANUAL).tex $(MANUAL).ind ;#$(MANUAL).glo
	mkdir -p $@
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -split 5 -show_section_numbers -dir $(MANUAL).html $(MANUAL)
	mv `basename $< .tex`.orig.tex $<

$(TSS).html: $(TSS).tex
	mkdir -p $@
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -split 0 -dir $(TSS).html $(TSS)
	mv `basename $< .tex`.orig.tex $<

$(CDB).html: $(CDB).tex
	mkdir -p $@
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -split 0 -show_section_numbers -dir $(CDB).html $(CDB)
	mv `basename $< .tex`.orig.tex $<

%.txt: %.lyx
	lyx -e text $<

%.pdf: %.tex %.dvi %.ind
	dvipdf $*
# pdflatex creates documents for acrobat reader >= 5.0	
#	which pdflatex > /dev/null && pdflatex $* || dvipdf $*

%.ps:  %.pdf
	dvips $*

%.dvi: %.tex
	latex $<

%.tex: %.lyx
	lyx -e latex $<

%.ind: %.dvi
	latex $*
	-makeindex $*
#	-makeindex -s l2hidx.ist $*
	latex $*
	latex $*

%.glo: %.dvi
	# the glossary, not implemented yet
	# makeindex -s l2hglo.ist -o $@ $<

include clean.mk
