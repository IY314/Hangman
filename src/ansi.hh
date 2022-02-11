#ifndef ANSI_HH
#define ANSI_HH

#include <sstream>
#include <string>

#define RESET "\033[m"

#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37
#define BRIGHT_BLACK 90
#define BRIGHT_RED 91
#define BRIGHT_GREEN 92
#define BRIGHT_YELLOW 93
#define BRIGHT_BLUE 94
#define BRIGHT_MAGENTA 95
#define BRIGHT_CYAN 96
#define BRIGHT_WHITE 97

std::string color(int c, const std::string& t) {
    std::ostringstream oss;
    oss << "\033[" << c << 'm' << t << RESET;
    return oss.str();
}

#endif /* ANSI_HH */
