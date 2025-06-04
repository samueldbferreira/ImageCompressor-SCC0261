all: programa

programa: compressor.c image.c image.h channels.c channels.h block.c block.h list.c list.h table.c table.h tree.c tree.h min-heap.c min-heap.h codes-table.c codes-table.h symbol-codes-list.c symbol-codes-list.h file-writer.c file-writer.h file-reader.c file-reader.h
	gcc compressor.c image.c channels.c block.c list.c table.c tree.c min-heap.c codes-table.c symbol-codes-list.c file-writer.c file-reader.c -o programa -lm

run: programa
	./programa

clean:
	rm -f programa *.o
