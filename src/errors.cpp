#include "errors.h"

#define NUM_LINE_DIGITS 5

static int CountNumDigits(int line);
static std::string GenerateNSpaces(int n);

namespace errors {
    Error::Error() {}
    Error::~Error() {}

    void Error::ReportError(ErrorType err_type, position::Pos pos, std::string msg)
    {
        // set has errors flag
        Error::SetHasErrors(true);

        int line = pos.GetLineNumber();
        int col = pos.GetColumnNumber();

        std::cout << pos.filename << ":" << line << ":" << col << ": " << msg << std::endl;
        std::cout << GenerateNSpaces(NUM_LINE_DIGITS - CountNumDigits(line)) << line << " |   " << msg << std::endl; // TODO: instead of message print line
        std:: cout << GenerateNSpaces(NUM_LINE_DIGITS) << " |   "  << GenerateNSpaces(col) << "^~~~~" << std::endl;
    }
}

static int CountNumDigits(int line) {
    int count = 0;
    while(line)
    {
        count++;
        line /= 10;
    }
    return count;
}

static std::string GenerateNSpaces(int n) {
    std::string n_spaces = "";
    for (int i = 0; i < n; i++)
    {
        n_spaces += " ";
    }
    return n_spaces;
}