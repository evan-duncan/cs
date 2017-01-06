CC = gcc
LIBS = -ledit
CFLAGS = -Wall -c
C_FILES = $(wildcard src/*.c)
OBJ_FILES = $(addprefix obj/, $(notdir $(C_FILES:.c=.o)))

lydia: $(OBJ_FILES)
	$(CC) $(LIBS) -o $@ $^

obj/%.o: src/%.c
	$(CC) -std=c99 $(CFLAGS) -o $@ $<

clean:
	rm -rf obj/*.o lydia

repl: lydia
	./lydia
