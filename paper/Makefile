vldb_bib: vldb_paper.bib
	bibtex vldb_paper

vldb_paper.pdf: vldb_paper.tex
	pdflatex vldb_paper.tex

clean:
	rm -rf *.log *.aux *.bbl *.blg *.dvi vldb_paper.pdf
