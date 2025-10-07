#include "position.h"

static int GetLineIndex(size_t off, std::vector<size_t>& line_offs);
namespace position {
    /* Constructor/Destructor */
    Pos::Pos(size_t s_off, size_t e_off) {
        this->start_offset = s_off;
        this->end_offset = e_off;
    }

    Pos::~Pos() {}

    /* Get the position of the token */
    int Pos::GetLineNumber() {
        return GetLineIndex(this->start_offset, Pos::line_offsets) + 1;
    }

    int Pos::GetColumnNumber() {
        int line_off = line_offsets[GetLineIndex(this->start_offset, Pos::line_offsets)];
        return start_offset - line_off + 1;
    }
}

/* Binary Search over the line offsets to get the line number of the token */
static int GetLineIndex(size_t off, std::vector<size_t>& line_offs) {
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
