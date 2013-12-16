# -*- makefile -*-

LATEX	= latex 
PDFLATEX = pdflatex


DVIPS		= dvips

.SUFFIXES: .tex .dvi .ps .pdf


.tex.dvi:
	@ if test ! -f $*.ind; then echo "" > $*.ind; fi
	@ $(LATEX) $*
	@ if ( grep 'Writing index file' $*.log > /dev/null ); \
	then makeindex $* ; $(LATEX) $* ; fi
	@ if ( grep 'LaTeX Warning: Label(s) may' $*.log > /dev/null ); \
	then $(LATEX) $* ; fi
	@ if ( grep 'LaTeX Warning: Citation' $*.log > /dev/null ); \
	then bibtex $* ; $(LATEX) $* ; fi
	@ if ( grep 'LaTeX Warning: Label(s) may' $*.log > /dev/null ); \
	then $(LATEX) $* ; fi

.dvi.ps:
	$(DVIPS) -t letter -o $*.ps $*

.tex.pdf:
	@$(PDFLATEX) $*
	@if ( grep 'LaTeX Warning: Label(s) may' $*.log > /dev/null ); \
	then $(PDFLATEX) $* ; else :; fi
	@if ( grep 'Writing index file' $*.log > /dev/null ); \
	then makeindex $* ; $(PDFLATEX) $* ; fi
	@if ( grep "Warning: Citation" $*.log > /dev/null ); then \
	  bibtex $* ; \
	  $(PDFLATEX) $* ; \
	fi
	@if ( grep 'Warning: Label(s) may' $*.log > /dev/null || \
	      grep 'Warning: Citation' $*.log > /dev/null); \
	then $(PDFLATEX) $* ; else :; fi


#
# Default rule
#

NAME		= coverpage
DISTNAME	= solodkyy_et_al

OTHER_TEX	= $(shell ls sec-*.tex)

default: $(NAME).pdf

ps:      $(NAME).ps
dvi:     $(NAME).dvi

$(NAME).bbl: $(NAME).pdf
	bibtex $(NAME)

#
# LaTeX stuff
#
$(NAME).dvi: $(NAME).tex $(OTHER_TEX)
$(NAME).ps: $(NAME).dvi
$(NAME).pdf: $(NAME).tex $(OTHER_TEX)

all: $(NAME).pdf 

# Other dependencies

# flatex
flatex: flatex.c

$(NAME).flt: flatex $(NAME).tex $(NAME).bbl $(OTHER_TEX)
	./flatex $(NAME).tex

# distribution
dist: $(NAME).flt $(NAME).pdf 
	mkdir $(DISTNAME)
	cp $(NAME).flt $(DISTNAME)/$(DISTNAME).tex
	cp $(NAME).pdf $(DISTNAME)/$(DISTNAME).pdf
	cp *.cls $(DISTNAME)/
#	cp $(HOME)/texmf/tex/latex/oslutils.sty $(DISTNAME)/
#	cp llncs.cls $(DISTNAME)/
#	cd $(DISTNAME) && \
#	  mkdir listings && \
#	  cd listings && \
#	  unzip ../../listings-1.1.zip && \
#	  tex listings.ins && \
#	  cd .. && \
#	  ln -s listings/*.sty listings/listings.cfg .
	tar zcvf $(DISTNAME).tar.gz $(DISTNAME)/
	rm -rf $(DISTNAME)/

listings-1.1.zip:
	wget -O listings-1.1.zip http://www.atscire.de/products/listings/listings-1.1.zip

#
# Standard rules
#

clean:
	/bin/rm -f *.dvi *.o $(NAME).ps $(NAME).pdf $(NAME).flt $(NAME).log $(NAME).blg $(NAME).bbl $(NAME).ind $(NAME).aux $(NAME).blg $(NAME).out $(NAME).pag flatex $(DISTNAME).tar.gz
	/bin/rm -rf ./$(DISTNAME)/


squeaky:
	/bin/rm -f *.log *.aux *.dvi *.blg *.toc *.bbl *.lof *.lot \
		$(NAME).pdf $(NAME).dvi $(NAME).ps $(NAME).flt flatex listings-1.1.zip $(DISTNAME).tar.gz
	/bin/rm -rf ./$(DISTNAME)/
