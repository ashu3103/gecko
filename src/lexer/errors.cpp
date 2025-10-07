#include "errors.h"

#define NUM_LINE_DIGITS 5

static std::string GetLineFromOffset(position::Pos pos, int lineno);
static int CountNumDigits(int line);
static std::string GenerateNSpaces(int n);

namespace errors {
    bool has_errors = false;

    void ReportError(ErrorType err_type, position::Pos pos, std::string msg)
    {
        // set has errors flag
        has_errors = true;

        int line = pos.GetLineNumber();
        int col = pos.GetColumnNumber();

        std::cout << pos.filepath << ":" << line << ":" << col << ": " << msg << std::endl;
        std::cout << GenerateNSpaces(NUM_LINE_DIGITS - CountNumDigits(line)) << line << " |   " << GetLineFromOffset(pos, line) << std::endl;
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

static std::string
GetLineFromOffset(position::Pos pos, int lineno)
{
    std::ifstream file(pos.filepath);  // Open file for reading
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + pos.filepath);
    }

    std::string line;
    int currentLine = 1;

    // Read line by line until we reach the desired one
    while (std::getline(file, line)) {
        if (currentLine == lineno) {
            return line;
        }
        currentLine++;
    }

    throw std::out_of_range("Line number exceeds total lines in file");
}