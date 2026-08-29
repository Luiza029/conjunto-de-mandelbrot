mandelbrot: main.o funcoes.o
	gcc main.o funcoes.o -fopenmp -o mandelbrot

main.o: main.c funcoes.h
	gcc -c -fopenmp main.c

funcoes.o: funcoes.c funcoes.h
	gcc -c -fopenmp funcoes.c

run: mandelbrot
	./mandelbrot

clean:
	rm -f *.o mandelbrot

.PHONY: run clean