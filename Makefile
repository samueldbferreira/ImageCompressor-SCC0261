all: programa

programa: compressor.c image.c image.h list.c list.h tree.c tree.h min-heap.c min-heap.h
	gcc compressor.c image.c list.c tree.c min-heap.c -o programa

run: programa
	./programa

clean:
	rm -f programa *.o
