include ../Makefile.common

MANUAL = sdccman
TSS = test_suite_spec
CDB = cdbfileformat

all: $(MANUAL).pdf $(MANUAL).html/index.html $(MANUAL).txt \
	$(TSS).pdf $(TSS).html/index.html $(TSS).txt \
	$(CDB).pdf $(CDB).html/index.html $(CDB).txt

install:
	$(INSTALL) -d $(docdir)
	cp -rf *.txt z80 avr $(docdir)
	if [ -f $(MANUAL).html/index.html ]; then cp -rf $(MANUAL).html $(docdir); fi
	if [ -f $(TSS).html/index.html    ]; then cp -rf $(TSS).html    $(docdir); fi
	if [ -f $(CDB).html/index.html    ]; then cp -rf $(CDB).html    $(docdir); fi
	if [ -f $(MANUAL).pdf             ]; then cp -rf $(MANUAL).pdf  $(docdir); fi
	if [ -f $(TSS).pdf                ]; then cp -rf $(TSS).pdf     $(docdir); fi
	if [ -f $(CDB).pdf                ]; then cp -rf $(CDB).pdf     $(docdir); fi
	find $(docdir) -name CVS -type d -exec rm -r {} \;
	rm -f $(docdir)/*/WARNINGS $(docdir)/*/*.pl $(docdir)/*/images.*

uninstall:
	rm -rf $(docdir)

$(MANUAL).html/index.html: $(MANUAL).tex $(MANUAL).ind ;#$(MANUAL).glo
	mkdir -p `dirname $@`
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -split 5 -show_section_numbers -local_icons -dir $(MANUAL).html $(MANUAL)
	mv `basename $< .tex`.orig.tex $<

$(TSS).html/index.html: $(TSS).tex
	mkdir -p `dirname $@`
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -split 0 -local_icons -dir $(TSS).html $(TSS)
	mv `basename $< .tex`.orig.tex $<

$(CDB).html/index.html: $(CDB).tex
	mkdir -p `dirname $@`
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -split 0 -show_section_numbers -local_icons -dir $(CDB).html $(CDB)
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
