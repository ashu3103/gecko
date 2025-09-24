#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <vector>

namespace position {
    class Pos {
        public:
            // per-file context (remains constant till the next file is encountered)
            inline static std::vector<size_t> line_offsets = {};
            inline static std::string filename = "";
            inline static bool initialized = false;

            void InitFileContext(std::string fname);
            void ResetFileContext();
            void AddLineOffset(size_t off);

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