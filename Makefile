all: compressor decompressor

compressor: compressor.c image.c channels.c block.c list.c table.c tree.c min-heap.c codes-table.c symbol-codes-list.c file-writer.c file-reader.c
	$(CC) $(CFLAGS) compressor.c image.c channels.c block.c list.c table.c tree.c min-heap.c codes-table.c symbol-codes-list.c file-writer.c file-reader.c -o compressor -lm

decompressor: decompressor.c image.c channels.c block.c list.c table.c tree.c min-heap.c codes-table.c symbol-codes-list.c file-writer.c file-reader.c
	$(CC) $(CFLAGS) decompressor.c image.c channels.c block.c list.c table.c tree.c min-heap.c codes-table.c symbol-codes-list.c file-writer.c file-reader.c -o decompressor -lm

run-compressor: compressor
	./compressor

run-decompressor: decompressor
	./decompressor

clean:
	rm -f compressor decompressor *.o
