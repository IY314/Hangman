IFLAGS := -Iinclude
WFLAGS := -Wall -Werror -Wextra
LFLAGS := -lncurses
CFLAGS := -c ${WFLAGS} ${IFLAGS}
.PHONY: all link mkdir

all: mkdir hangman.o link

link: mkdir
	${CC} bin/*.o -o bin/hangman ${LFLAGS}

mkdir:
	mkdir -p bin

hangman.o: mkdir src/hangman.c
	${CC} src/hangman.c -o bin/hangman.o ${CFLAGS}
