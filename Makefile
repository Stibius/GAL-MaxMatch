.PHONY: all time clean doc

all:
	g++ -c graph.cpp -o graph.o
	g++ -c main.cpp -o main.o
	g++ graph.o main.o -o main

time:
	g++ -D MEASURE_TIME -c graph.cpp -o graph.o
	g++ -c main.cpp -o main.o
	g++ graph.o main.o -o main

clean:
	rm -f main main.o graph.o graph_time.o maxMatch.dvi maxMatch.ps maxMatch.aux maxMatch.log maxMatch.pdf

doc:
	latex maxMatch.tex
	dvips -t a4 maxMatch.dvi
	ps2pdf maxMatch.ps
	