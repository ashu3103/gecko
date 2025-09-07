#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>
#include <string>

namespace errors {
    /* Enum to categorize different types of syntax errors */
    enum class ErrorType {
        UNEXPECTED_TOKEN,       // a token mismatch in an expression
        MISSING_TOKEN,          // a token missing to finish a statement
        INVALID_SYNTAX,         
        UNEXPECTED_END_OF_FILE,
        // Add more error types as needed
    };

    class Error {
        bool has_errors = false;

        bool GetHasErrors()
        {
            return this->has_errors;
        }

        void SetHasErrors(bool val)
        {
            this->has_errors = val;
        }

    public:
        Error();
        ~Error();
        /**
         * filing a complaint with details
         * @param line The line at which the error originated
         * @param col The column at which the error originated
         * @param msg The message to be printed
         * @param filename The translational unit
         */
        void ReportError(ErrorType err_type, int line, int col, std::string msg, std::string filename);
    };
}

#endif // ERRORS_H
