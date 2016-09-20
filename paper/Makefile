vldb_bib: vldb_paper.bib
	bibtex vldb_paper

vldb_paper.pdf: vldb_paper.tex
	pdflatex vldb_paper.tex

IEEE_bib: IEEE_paper.bib
	bibtex IEEE_paper

IEEE_paper.pdf: IEEE_paper.tex
	pdflatex IEEE_paper.tex
clean:
	rm -rf *.log *.aux *.bbl *.blg *.dvi vldb_paper.pdf IEEE_paper.pdf
