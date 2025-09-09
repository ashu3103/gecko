#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <vector>

namespace position {
    class Pos {
        public:
            // per-file context (remains constant till the next file is encountered)
            inline static std::vector<u_int32_t> line_offsets = {};
            inline static std::string filename = "";
            inline static bool initialized = false;

            void InitFileContext(std::string fname);
            void ResetFileContext();
            void AddLineOffset(u_int32_t off);

            // per-token byte positions
            u_int32_t start_offset;
            u_int32_t end_offset;

            Pos(u_int32_t s_off, u_int32_t e_off);
            ~Pos();

            int GetLineNumber();
            int GetColumnNumber();
    };
}

#endif // END_H