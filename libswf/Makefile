all: library tests

library:
	$(CC) -c src/swf.c -o obj/swf.o -Iinclude
	$(CC) -c src/rect.c -o obj/rect.o -Iinclude
	$(CC) -c src/compression.c -o obj/compression.o -Iinclude -lx
	ar r lib/libswf.a obj/swf.o obj/rect.o

tests:
	$(CC) test/test1.c -Llib -lswf -o bin/test1 -Iinclude

clean:
	rm -f obj/*
	rm -f lib/*
	rm -f bin/*