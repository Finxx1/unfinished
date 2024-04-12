engine: obj/main.o obj/render.o

obj/main.o: src/engine/main.c
	$(CC) $(CFLAGS) $< -o $@

obj/render.o: src/engine/render.c
	$(CC) $(CFLAGS) $< -o $@

