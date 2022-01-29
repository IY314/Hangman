# Hangman
I made this repo because I needed to one-up my friends in making Hangman (they used Python, so I used C++).

## Building
```sh
git clone https://github.com/IY314/Hangman.git
cd Hangman
make build
```

## Running
You must first specify a `words.txt` file (a filename can be provided as an argument to the program, or it will assume a file named `words.txt` in the current directory exists and use that).
```sh
./hangman <filename>  # Use the file named <filename>
./hangman             # Use the words.txt file in the current directory
```
