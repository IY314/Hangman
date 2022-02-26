.PHONY: build mkdir

build: mkdir
	$(CXX) src/hangman.cc -o bin/hangman -Wall -Werror -Wextra -O2 -std=c++11

mkdir:
	mkdir -p bin
