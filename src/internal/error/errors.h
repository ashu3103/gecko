#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <position/position.h>

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

const std::unordered_map<errors::ErrorType, std::string> errorStatusMap = {
    {errors::ErrorType::INVALID_CHARACTER, "Invalid character"},
    {errors::ErrorType::INVALID_OPERAND_TYPE, "Invalid operand type"},
    {errors::ErrorType::DIVISION_BY_ZERO, "Division by zero"},
};

namespace errors {
    /* Enum to categorize different types of syntax errors */
    enum class ErrorType {
        NO_ERROR,
        UNEXPECTED_TOKEN,       // a token mismatch in an expression
        MISSING_TOKEN,          // a token missing to finish a statement
        INVALID_SYNTAX,         
        UNEXPECTED_END_OF_FILE,
        INVALID_CHARACTER,      // lexer error, if any invalid character is encountered
        INVALID_OPERAND_TYPE,
        DIVISION_BY_ZERO,
        // Add more error types as needed
    };

    /* error flags */
    extern bool has_runtime_errors;
    extern bool has_errors;
    extern ErrorType error_status;

    /**
     * filing a complaint with details
     * @param err_type The error type
     * @param pos The position of the token
     * @param msg The message to be printed
     */
    void ReportError(ErrorType err_type, position::Pos pos, std::string msg);

    /**
     * generate a runtime error
     * @param err_type The error type
     * @param pos The position of the token
     * @param msg The message
     */
    std::string GenerateRuntimeError(ErrorType err_type, position::Pos pos, std::string msg);

    static std::string GenericErrMsg(ErrorType err_type)
    {
        if (errorStatusMap.find(err_type) != errorStatusMap.end())
        {
            return errorStatusMap.at(err_type);
        }

        return "";
    }
}

#endif // __ERRORS_H__
