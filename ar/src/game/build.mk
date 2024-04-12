game: obj/game.o

obj/game.o: src/game/game.c
	$(CC) $(CFLAGS) $< -o $@

