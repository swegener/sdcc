# Very simple Makefile for converting the design doc into something useful.
TEX = latex

S = test_suite_spec.tex
PDF = $(S:.tex=.pdf)
PS = $(S:.tex=.ps)
HTML = $(S:.tex=.html)

all: $(PS) $(PDF) $(HTML)

%.ps: %.dvi
	dvips -f.ps $< > $@

%.pdf: %.dvi
	dvipdf $< $@

%.html: %.tex
	latex2html -no_subdir -split 0 $<

clean:
	rm -f $(PS) $(PDF) *.log *.aux *~
