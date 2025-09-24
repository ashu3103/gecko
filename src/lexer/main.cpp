#include <iostream>
#include <fstream>
#include "scanner.h"

int main(int argc, char* argv[]) {

    if (argc != 2)
    {
        return 1;
    }
    scanner::Scanner snr(std::fstream(argv[1], std::ios::in | std::ios::binary));
    if (!snr.src.in.is_open())
    {
        std::cerr << "error opening file" << std::endl;
        return 1;
    }
    while (snr.NextToken());

    std::cout << "Streaming tokens" << std::endl;
    for (size_t i=0;i<snr.tokens.size();i++){
        std::cout << snr.tokens[i].tok << std::endl;
    }
    return 0;
}