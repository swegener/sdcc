include ../Makefile.common

MANUAL = sdccman
TSS = test_suite_spec
CDB = cdbfileformat

all: $(MANUAL).pdf $(MANUAL).html/index.html $(MANUAL).txt \
	$(TSS).pdf $(TSS).html/index.html $(TSS).txt \
	$(CDB).pdf $(CDB).html/index.html $(CDB).txt

install:
	$(INSTALL) -d $(DESTDIR)$(docdir)
	cp -rf *.txt z80 avr $(DESTDIR)$(docdir)
	if [ -f $(MANUAL).html/index.html ]; then cp -rf $(MANUAL).html $(DESTDIR)$(docdir); fi
	if [ -f $(TSS).html/index.html    ]; then cp -rf $(TSS).html    $(DESTDIR)$(docdir); fi
	if [ -f $(CDB).html/index.html    ]; then cp -rf $(CDB).html    $(DESTDIR)$(docdir); fi
	if [ -f $(MANUAL).pdf             ]; then cp -rf $(MANUAL).pdf  $(DESTDIR)$(docdir); fi
	if [ -f $(TSS).pdf                ]; then cp -rf $(TSS).pdf     $(DESTDIR)$(docdir); fi
	if [ -f $(CDB).pdf                ]; then cp -rf $(CDB).pdf     $(DESTDIR)$(docdir); fi
	rm -rf `find $(DESTDIR)$(docdir) -name CVS -type d`
	rm -f $(DESTDIR)$(docdir)/*/WARNINGS $(DESTDIR)$(docdir)/*/*.pl $(DESTDIR)$(docdir)/*/images.*

uninstall:
	rm -rf $(DESTDIR)$(docdir)

$(MANUAL).html/index.html: $(MANUAL).tex $(MANUAL).ind ;#$(MANUAL).glo
	mkdir -p `dirname $@`
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -split 5 -show_section_numbers -local_icons -info "" -dir $(MANUAL).html $(MANUAL)
	mv `basename $< .tex`.orig.tex $<

$(TSS).html/index.html: $(TSS).tex
	mkdir -p `dirname $@`
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -split 0 -local_icons -info "" -dir $(TSS).html $(TSS)
	mv `basename $< .tex`.orig.tex $<

$(CDB).html/index.html: $(CDB).tex
	mkdir -p `dirname $@`
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -split 0 -show_section_numbers -local_icons -info "" -dir $(CDB).html $(CDB)
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

archive: all
	rm -rf sdcc-doc sdcc-doc.tar.bz2
	mkdir sdcc-doc
	rsync -rCt avr z80 *.pdf *.txt sdcc-doc

	for doc in $(MANUAL) $(TSS) $(CDB); do \
	  rsync -Rt $$doc.html/*.html $$doc.html/*.png $$doc.html/*.css sdcc-doc/; \
	done

	mkdir sdcc-doc/as
	rsync -rCt ../as/doc/* sdcc-doc/as/

	mkdir sdcc-doc/ucsim
	cd ../sim/ucsim/doc; rsync *.html *.jpg *.gif *.fig ../../../doc/sdcc-doc/ucsim/

	tar -c sdcc-doc | bzip2 -9 > sdcc-doc.tar.bz2

include clean.mk
