C = pdflatex
CAPS = 0_Portada.tex 2_Preeliminares.tex 1_Introduccion.tex
BIBS = Bibliografia/bibliografia.bib
MAIN = main.tex
OUT = main.pdf

all: $(OUT)

$(OUT): $(MAIN) $(CAPS) $(BIBS)
	$(C) $(MAIN)
	bibtex main
	$(C) $(MAIN)
	$(C) $(MAIN)

clean:
	rm *.log *.aux *.lot *.lof *.toc
