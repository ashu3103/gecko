#include <error/errors.h>

#define NUM_LINE_DIGITS 5

static std::string GetLineFromOffset(position::Pos pos, int lineno);
static int CountNumDigits(int line);
static std::string GenerateNSpaces(int n);

namespace errors {
    /* Definition of the global error flags */
    bool has_errors = false;
    ErrorType error_status = ErrorType::NO_ERROR;
    /* Helper function to report errors at a specific position */
    void ReportError(ErrorType err_type, position::Pos pos, std::string msg)
    {
        /* set has errors flags */
        has_errors = true;
        error_status = err_type;

        int line = pos.GetLineNumber();
        int col = pos.GetColumnNumber();

        std::cout << pos.filepath << ":" << line << ":" << col << ": " << msg << std::endl;
        std::cout << GenerateNSpaces(NUM_LINE_DIGITS - CountNumDigits(line)) << line << " |   " << GetLineFromOffset(pos, line) << std::endl;
        std:: cout << GenerateNSpaces(NUM_LINE_DIGITS) << " |   "  << GenerateNSpaces(col - 1) << "^~~~~" << std::endl;
    }
}

/* count the number of digits in a number string */
static int
CountNumDigits(int line) {
    int count = 0;
    while(line)
    {
        count++;
        line /= 10;
    }
    return count;
}

/* generate n whitespaces, used to align message with the printing format */
static std::string
GenerateNSpaces(int n) {
    std::string n_spaces = "";
    for (int i = 0; i < n; i++)
    {
        n_spaces += " ";
    }
    return n_spaces;
}

/* Get the exact line given the line number */
// TODO(ashu3103): may create some overheads
static std::string
GetLineFromOffset(position::Pos pos, int lineno)
{
    std::ifstream file(pos.filepath);  // Open file for reading
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + pos.filepath);
    }

    std::string line = "";
    int currentLine = 1;

    // Read line by line until we reach the desired one
    while (std::getline(file, line)) {
        if (currentLine == lineno) {
            break;
        }
        currentLine++;
    }

    file.close();
    
    if (lineno > currentLine)
    {
        throw std::out_of_range("Line number exceeds total lines in file");
    }
    return line;
}