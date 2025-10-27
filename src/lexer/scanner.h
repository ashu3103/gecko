/**
 * scans out the next token each time nextToken is called
 * 
 */
#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <source.h>
#include <token.h>
#include <position/position.h>

#include <iostream>
#include <string>
#include <vector>

using namespace token;

namespace scanner {

    class Scanner {
        public:
            size_t start_off;
            size_t end_off;
            source::Source src;
            std::vector<Token> tokens;

            Scanner(std::string filepath);
            ~Scanner();

            bool NextToken();
            void Ident();
            void Number();
            void String(bool &ok);
    };
}

#endif // __SCANNER_H__