#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ncurses.h>

#define WRONG 1
#define DISPLAY 2
#define HANGMAN 3
#define WIN 4
#define addcolor(c, t)     \
    attron(COLOR_PAIR(c)); \
    t;                     \
    attroff(COLOR_PAIR(c))

#define WORDSIZE 32
#define MAXGUESSES 6

#define HANGMEN                                                          \
    {                                                                    \
        " +---+\n |   |\n     |\n     |\n     |\n     |\n-----+-",       \
            " +---+\n |   |\n O   |\n     |\n     |\n     |\n-----+-",   \
            " +---+\n |   |\n O   |\n |   |\n     |\n     |\n-----+-",   \
            " +---+\n |   |\n O   |\n/|   |\n     |\n     |\n-----+-",   \
            " +---+\n |   |\n O   |\n/|\\  |\n     |\n     |\n-----+-",  \
            " +---+\n |   |\n O   |\n/|\\  |\n/    |\n     |\n-----+-",  \
            " +---+\n |   |\n O   |\n/|\\  |\n/ \\  |\n     |\n-----+-", \
    }

unsigned int getLineCount(const char* filename) {
    // open file
    FILE* fp = fopen(filename, "rt");
    if (fp == NULL) {
        perror("hangman: Could not open file: ");
        exit(1);
    }

    // count lines
    unsigned int lineCount = 0;
    while ((fscanf(fp, "%*[^\n]"), fscanf(fp, "%*c")) != EOF) ++lineCount;

    // close file and return
    fclose(fp);
    return lineCount;
}

void getWords(char** words, const char* filename) {
    // open file
    FILE* fp = fopen(filename, "rt");
    if (fp == NULL) {
        perror("hangman: Could not open file: ");
        exit(1);
    }
    // allocate file chunk
    char* word = malloc(WORDSIZE);

    for (unsigned int c = 0; fgets(word, WORDSIZE, fp) != NULL; ++c) {
        // remove trailing newline
        word[strlen(word) - 1] = '\0';

        // allocate word and copy to array
        words[c] = malloc(WORDSIZE);
        strcpy(words[c], word);
    }

    // free file chunk and close file
    free(word);
    fclose(fp);
}

void generateWord(char* word, char** words, unsigned int lineCount) {
    // generate random index
    unsigned int idx = rand() % lineCount;

    // copy word to pointer
    strcpy(word, words[idx]);
}

void init() {
    // init screen
    initscr();
    if (has_colors() == false) {
        endwin();
        puts("Your terminal does not support color");
        exit(1);
    }

    // init color
    start_color();
    init_pair(WRONG, COLOR_RED, COLOR_BLACK);
    init_pair(DISPLAY, COLOR_CYAN, COLOR_BLACK);
    init_pair(HANGMAN, COLOR_YELLOW, COLOR_BLACK);
    init_pair(WIN, COLOR_GREEN, COLOR_BLACK);

    // init random
    srand(time(NULL));

    // keypress detection
    raw();
    nonl();
}

void destroy(char** words, unsigned int lineCount) {
    for (unsigned int i = 0; i < lineCount; ++i) free(words[i]);
    free(words);
    endwin();
}

void game(char** words, unsigned int lineCount) {
    char* word = malloc(WORDSIZE);
    generateWord(word, words, lineCount);
    unsigned int wordLength = strlen(word);

    // init game logic
    char wrongGuesses[MAXGUESSES];
    unsigned int wrongCount = 0;
    char state = 0;

    // set up display
    char hangmen[MAXGUESSES + 1][50] = HANGMEN;
    char display[wordLength + 1];
    for (unsigned int i = 0; i < wordLength; ++i) display[i] = '_';
    display[wordLength] = '\0';

    // event loop
    while (true) {
        clear();

        // display wrong guess list
        addcolor(WRONG, addstr("Wrong guesses:\n"));
        for (unsigned int i = 0; i < wrongCount; ++i) {
            addcolor(WRONG, addch(wrongGuesses[i]));
            if (i != wrongCount - 1) addstr(", ");
        }
        addch('\n');

        // display word status and hangman
        addcolor(DISPLAY, addstr(display));
        addch('\n');
        addcolor(HANGMAN, addstr(hangmen[wrongCount]));
        addch('\n');

        if (state == 1) {
            addcolor(WIN, addstr(display));
            addstr("\nYou win!\nPress enter to play again");
            break;
        } else if (state == -1) {
            addstr("You lose, the word was '");
            addstr(word);
            addstr("'\nPress enter to play again");
            break;
        } else {
            // get character
            addstr("Guess a letter:\n");
            char guess = getch();
            if (guess == '\e') {
                state = -2;
                break;
            }

            // handle guess
            if (strchr(word, guess)) {
                // update display
                for (unsigned int i = 0; i < wordLength; ++i)
                    if (word[i] == guess) display[i] = guess;

                // check if every character has been guessed
                if (!strcmp(word, display)) state = 1;
            } else {
                // Increment wrong guesses count
                wrongGuesses[wrongCount++] = guess;

                // Check whether any guesses are left
                if (wrongCount == MAXGUESSES) state = -1;
            }
        }
    }
    free(word);
    if (state == -2) {
        destroy(words, lineCount);
        exit(0);
    }
}

int main(int argc, char** argv) {
    init();

    // get filename
    char filename[128];
    if (argc > 1)
        strcpy(filename, argv[1]);
    else
        strcpy(filename, "words.txt");

    unsigned int lineCount = getLineCount(filename);
    char** words = malloc(lineCount);
    getWords(words, filename);

    char ch;

    do game(words, lineCount);
    while ((ch = getch()) == '\n' || ch == '\r');

    // Destruction logic
    destroy(words, lineCount);
    return EXIT_SUCCESS;
}
