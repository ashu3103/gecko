#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <string>
#include <vector>

namespace position {
    class Pos {
        public:
            inline static std::vector<size_t> line_offsets = {};
            inline static std::string filepath = "";

            static void InitFileContext(std::string fp)
            {
                filepath = fp;
                line_offsets.clear();
                line_offsets.push_back(0);
            }

            static void ResetFileContext()
            {
                filepath = "";
                line_offsets.clear();
            }

            static void AddLineOffset(size_t off)
            {
                line_offsets.push_back(off);
            }
            // per-token byte positions
            size_t start_offset;
            size_t end_offset;

            Pos(size_t s_off, size_t e_off);
            ~Pos();

            int GetLineNumber();
            int GetColumnNumber();
    };
}

#endif // END_H