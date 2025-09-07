#include <iostream>
#include "errors.h"

int main() {

    errors::Error *e = new(errors::Error);
    e->ReportError(errors::ErrorType::INVALID_SYNTAX, 1, 1, "lode", "main.c");
    return 0;
}