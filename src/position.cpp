#include "position.h"

static int GetLineIndex(u_int32_t off, std::vector<u_int32_t>& line_offs);
namespace position {
    /* Constructor/Destructor */
    Pos::Pos(u_int32_t s_off, u_int32_t e_off) {
        this->start_offset = s_off;
        this->end_offset = e_off;
    }

    Pos::~Pos() {}

    /* Manipulate File Context fields */
    void Pos::AddLineOffset(u_int32_t off) {
        this->line_offsets.push_back(off);
    }

    void Pos::InitFileContext(std::string fname) {
        if (this->initialized)
        {
            // TODO:error
        }

        this->filename = fname;
        this->line_offsets.clear();

        /* first line starts at offset 0 */
        line_offsets.push_back(0);
        this->initialized = true;
    }

    void Pos::ResetFileContext() {
        this->filename.clear();
        this->line_offsets.clear();
        this->initialized = false;
    }

    /* Get the position of the token */
    int Pos::GetLineNumber() {
        return GetLineIndex(this->start_offset, this->line_offsets) + 1;
    }

    int Pos::GetColumnNumber() {
        int line_off = this->line_offsets[GetLineIndex(this->start_offset, this->line_offsets)];
        return start_offset - line_off + 1;
    }
}

/* Binary Search over the line offsets to get the line number of the token */
static int GetLineIndex(u_int32_t off, std::vector<u_int32_t>& line_offs) {
    int n = line_offs.size();
    int s = 0;
    int e = n - 1;
    int res = -1;

    while(s <= e) {
        int mid = (e - s)/2 + s;
        if (line_offs[mid] <= off) {
            res = mid;
            s = mid + 1;
        } else {
            e = mid - 1;
        }
    }

    return res;
}
