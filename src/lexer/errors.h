#ifndef ERRORS_H
#define ERRORS_H

#include "position.h"

#include <iostream>
#include <fstream>
#include <string>

namespace errors {
    /* Enum to categorize different types of syntax errors */
    enum class ErrorType {
        UNEXPECTED_TOKEN,       // a token mismatch in an expression
        MISSING_TOKEN,          // a token missing to finish a statement
        INVALID_SYNTAX,         
        UNEXPECTED_END_OF_FILE,
        INVALID_CHARACTER,
        // Add more error types as needed
    };

    /* tracks the global errors */
    extern bool has_errors;

    /**
     * filing a complaint with details
     * @param err_type The error type
     * @param pos The position of the token
     * @param msg The message to be printed
     * @param filename The translational unit
     */
    void ReportError(ErrorType err_type, position::Pos pos, std::string msg);
}

#endif // ERRORS_H
