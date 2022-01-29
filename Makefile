mkdir:
	mkdir -p bin

build: mkdir
	$(CXX) src/hangman.cc -o bin/hangman -Wall -Werror -Wextra -O2 -std=c++11
