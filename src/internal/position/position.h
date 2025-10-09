#ifndef __POSITION_H__
#define __POSITION_H__

#include <iostream>
#include <string>
#include <vector>

namespace position {
    class Pos {
        public:
            /* file context (unique for each file scanned) */
            /* inline is used to initialize the static fields, otherwise they won't be allocated and therefore marked by linker as undefined */
            inline static std::vector<size_t> line_offsets = {}; /* capture the line offsets */
            inline static std::string filepath = "";             /* capture the file path to be scanned */

            /* should be called explicitely by the scanner constructor */
            static void InitFileContext(std::string fp)
            {
                filepath = fp;
                line_offsets.clear();
                line_offsets.push_back(0);
            }

            /* should be called explicitely by the scanner deconstructor */
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

#endif // __POSITION_H__