mandelbrot: main.o funcoes.o
	gcc main.o funcoes.o -o mandelbrot

main.o: main.c funcoes.h
	gcc -c main.c

funcoes.o: funcoes.c funcoes.h
	gcc -c funcoes.c

run: mandelbrot
	./mandelbrot

clean:
	rm -f *.o mandelbrot

.PHONY: run clean