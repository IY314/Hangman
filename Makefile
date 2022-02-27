WFLAGS := -Wall -Werror -Wextra
LFLAGS := -lncurses
CFLAGS := -c ${WFLAGS}

.PHONY: all link clean

all: link

link: hangman.o
	${CC} bin/*.o -o bin/hangman ${LFLAGS}

clean:
	rm -rf bin

bin:
	mkdir -p bin

hangman.o: bin src/hangman.c
	${CC} src/hangman.c -o bin/hangman.o ${CFLAGS}
