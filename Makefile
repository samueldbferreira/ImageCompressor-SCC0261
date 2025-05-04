all: programa

programa: compressor.c list.c list.h
	gcc compressor.c list.c -o programa

run: programa
	./programa

clean:
	rm -f programa *.o
