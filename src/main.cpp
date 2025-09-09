#include <iostream>
#include "errors.h"
#include "position.h"
#include "source.h"

int main() {
    position::Pos p(15, 20);
    
    p.InitFileContext("main.cpp");
    p.AddLineOffset(16);
    p.AddLineOffset(20);
    p.AddLineOffset(29);
    p.AddLineOffset(42);

    errors::Error *e = new errors::Error();
    e->ReportError(errors::ErrorType::INVALID_SYNTAX, p, "error detected");

    NextSize(2);

    return 0;
}