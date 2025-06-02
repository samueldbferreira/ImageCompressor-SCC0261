all: programa

programa: compressor.c image.c image.h list.c list.h table.c table.h tree.c tree.h min-heap.c min-heap.h symbol-codes-list.c symbol-codes-list.h codes-table.c codes-table.h
	gcc compressor.c image.c list.c table.c tree.c min-heap.c symbol-codes-list.c codes-table.c -o programa

run: programa
	./programa

clean:
	rm -f programa *.o
