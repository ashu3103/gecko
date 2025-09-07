#include "errors.h"

// system
#include <iostream>

#define NUM_LINE_DIGITS 5

static int CountNumDigits(int line)
{
    int count = 0;

    while(line)
    {
        count++;
        line /= 10;
    }

    return count;
}

static std::string GenerateNSpaces(int n)
{
    std::string n_spaces = "";
    for (int i = 0; i < n; i++)
    {
        n_spaces += " ";
    }
    return n_spaces;
}

namespace errors {
    Error::Error() {}
    Error::~Error() {}

    void Error::ReportError(ErrorType err_type, int line, int col, std::string msg, std::string filename)
    {
        std::cout << filename << ":" << line << ":" << col << ": " << msg << std::endl;
        std::cout << GenerateNSpaces(NUM_LINE_DIGITS - CountNumDigits(line)) << line << " |   " << msg << std::endl;
        std:: cout << GenerateNSpaces(NUM_LINE_DIGITS) << " |   "  << GenerateNSpaces(col) << "^~~~~" << std::endl;
    }
}