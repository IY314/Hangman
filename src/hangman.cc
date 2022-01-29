#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#define HANGMEN {\
    " +---+\n |   |\n O   |\n/|\\  |\n/ \\  |\n     |\n-----+-", \
    " +---+\n |   |\n O   |\n/|\\  |\n/    |\n     |\n-----+-",  \
    " +---+\n |   |\n O   |\n/|\\  |\n     |\n     |\n-----+-",  \
    " +---+\n |   |\n O   |\n/|   |\n     |\n     |\n-----+-",   \
    " +---+\n |   |\n O   |\n |   |\n     |\n     |\n-----+-",  \
    " +---+\n |   |\n O   |\n     |\n     |\n     |\n-----+-",  \
    " +---+\n |   |\n     |\n     |\n     |\n     |\n-----+-",  \
}

std::vector<std::string> get_words(const std::string& filename) {
    std::ifstream i(filename);
    if (i.fail()) {
        std::cout << "words.txt file not found\n";
        std::exit(1);
    }
    std::vector<std::string> vec;
    std::string line;
    while (std::getline(i, line)) {
        vec.push_back(line);
    }
    i.close();
    return vec;
}

std::string generate_word(const std::vector<std::string>& words) {
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<unsigned long> distribution(0, words.size() - 1);
    return words[distribution(generator)];
}

bool str_contains(const std::string& str, const std::string& sub, int pos = 0) {
    return str.find(sub, pos) != std::string::npos;
}

std::vector<int> str_find_all(const std::string& str, const std::string& sub) {
    int pos = -1;
    std::vector<int> matches;
    while (true) {
        unsigned long idx = str.find(sub, pos + 1);
        if (idx == std::string::npos) {
            break;
        }
        matches.push_back(idx);
        pos = idx;
    }
    return matches;
}

int main(int argc, char *argv[]) {
    std::string filename;
    if (argc > 1) filename = argv[1];
    else filename = "words.txt";
    std::vector<std::string> words = get_words(filename);
    std::string word = generate_word(words);
    std::vector<std::string> hangmen HANGMEN;
    unsigned long guesses = 6;
    char display[word.size() + 1];
    for (unsigned long i = 0; i < word.size(); ++i) display[i] = '_';
    display[word.size()] = '\0';
    while (true) {
        std::cout << display << '\n';
        std::cout << hangmen[guesses];
        std::cout << "\nGuess a letter: ";
        std::string guess;
        std::getline(std::cin, guess);
        if (guess.size() != 1) std::cout << "Invalid character\n";
        else {
            if (str_contains(word, guess)) {
                for (int i : str_find_all(word, guess)) display[i] = guess[0];
                if (word == display) {
                    std::cout << display << "\nYou win!\n";
                    break;
                }
            } else {
                if (!--guesses) {
                    std::cout << "You lose, the word was '" << word << "'\n";
                    break;
                }
            }
        }
    }
    return 0;
}
