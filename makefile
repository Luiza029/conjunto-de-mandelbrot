mandelbrot: main.o
	gcc main.o -o mandelbrot

main.o: main.c
	gcc -c main.c

run: mandelbrot
	./mandelbrot

clean:
	rm -f *.o mandelbrot

.PHONY: run clean