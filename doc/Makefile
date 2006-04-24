include ../Makefile.common

ifdef $(LYX_SYSDIR)
  LYXFLAGS += -sysdir $(LYX_SYSDIR)
endif

MANUAL = sdccman
TSS = test_suite_spec
CDB = cdbfileformat

SOURCES = $(MANUAL).lyx $(TSS).lyx $(CDB).lyx

TXT  = $(SOURCES:.lyx=.txt)
PDF  = $(SOURCES:.lyx=.pdf)
TEX  = $(SOURCES:.lyx=.tex)
HTML_DIR   = $(SOURCES:.lyx=.html)
HTML_INDEX = $(SOURCES:.lyx=.html/index.html)

all: $(TXT) $(PDF) $(HTML_INDEX)

install:
	$(INSTALL) -d $(DESTDIR)$(docdir)
	cp -rf *.txt z80 avr $(DESTDIR)$(docdir)
	if [ -f $(MANUAL).html/index.html ]; then cp -rf $(MANUAL).html $(DESTDIR)$(docdir); fi
	if [ -f $(TSS).html/index.html    ]; then cp -rf $(TSS).html    $(DESTDIR)$(docdir); fi
	if [ -f $(CDB).html/index.html    ]; then cp -rf $(CDB).html    $(DESTDIR)$(docdir); fi
	if [ -f $(MANUAL).pdf             ]; then cp -rf $(MANUAL).pdf  $(DESTDIR)$(docdir); fi
	if [ -f $(TSS).pdf                ]; then cp -rf $(TSS).pdf     $(DESTDIR)$(docdir); fi
	if [ -f $(CDB).pdf                ]; then cp -rf $(CDB).pdf     $(DESTDIR)$(docdir); fi
	rm -rf `find $(DESTDIR)$(docdir) -name .svn -type d`
	rm -f $(DESTDIR)$(docdir)/*/WARNINGS $(DESTDIR)$(docdir)/*/*.pl $(DESTDIR)$(docdir)/*/images.*

uninstall:
	rm -rf $(DESTDIR)$(docdir)

$(MANUAL).html/index.html: $(MANUAL).tex $(MANUAL).ind
	rm -rf $(MANUAL).html 
	mkdir -p `dirname $@`
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -show_section_numbers -split 5 -local_icons -dir $(MANUAL).html `basename $<`
	mv `basename $< .tex`.orig.tex $<
	rm -f $(MANUAL).html/WARNINGS $(MANUAL).html/labels.pl

$(TSS).html/index.html: $(TSS).tex
	rm -rf `dirname $@`
	mkdir -p `dirname $@`
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -split 0 -local_icons -dir $(TSS).html `basename $<`
	mv `basename $< .tex`.orig.tex $<
	rm -f $(TSS).html/WARNINGS $(TSS).html/labels.pl

$(CDB).html/index.html: $(CDB).tex
	rm -rf `dirname $@`
	mkdir -p `dirname $@`
	# fixes latex2html problems with two consecutive dashes for long-options: --stack-auto
	# fixes latex2html problems with \tabularnewline
	mv $< `basename $< .tex`.orig.tex
	sed 's,-\\/-,-\\/-\\/-,g' < `basename $< .tex`.orig.tex | \
	sed 's,\\tabularnewline$$,\\\\,g' > $<
	-latex2html -show_section_numbers -split 0 -local_icons -dir $(CDB).html `basename $<`
	mv `basename $< .tex`.orig.tex $<
	rm -f $(CDB).html/WARNINGS $(CDB).html/labels.pl

%.txt: %.lyx
	lyx -e text $(LYXFLAGS) $<

%.pdf: %.tex
	@echo "### Create pdf from tex: $< -> $@"
	pdflatex $*
	-makeindex $*
	pdflatex $*
	pdflatex $*
	pdfopt $*.pdf tmp.pdf
	rm $*.pdf
	mv tmp.pdf $*.pdf

#%.ps:  %.pdf
#	dvips $*

%.dvi: %.tex
	latex $<

%.tex: %.lyx
	lyx -e latex $< $(LYXFLAGS)

#%.ind: %.dvi
#	latex $*
#	-makeindex $*
##	-makeindex -s l2hidx.ist $*
	#latex $*
#	latex $*

#%.glo: %.dvi
#	# the glossary, not implemented yet
#	# makeindex -s l2hglo.ist -o $@ $<

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
