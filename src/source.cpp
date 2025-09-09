#include "source.h"

static size_t NextSize(size_t c_sz);
static void MemCopy(void* dst, const void* src, size_t nbytes);

namespace source {
    Source::Source() {
        this->b = -1;
        this->r = 0;
        this->e = 0;
        this->chr = ' ';
        this->chw = 0;

        this->buffer = NULL;  // TODO: initialize the buffer
        this->line = 0;
        this->col = 0;
    }

    Source::~Source() {
        // deallocate the buffer
        if (this->buffer)
            delete[] this->buffer;
        this->buffer = NULL;
    }

    void Source::Start() {
        this->b = this->r - this->chw;
    }

    void Source::Stop() {
        this->b = -1;
    }

    std::string Source::Segment() {
        std::string res = "";
        for (size_t i = this->b; i < this->r; ++i) {
            res += this->buffer[i];
        }
        return res;
    }

    void Source::ResizeOrFlushBuffer(int sz) {
        unsigned char* content = NULL;
        size_t content_length;
        size_t new_size;
        size_t b;

        b = this->r;
        if (this->b >= 0)
        {
            b = this->b;
        }
        
        this->GetBufferContent(content, content_length);
        // Check the need to grow the buffer
        if (this->buffer_length <= content_length*2) {
            new_size = NextSize(sz);
            // create new resized buffer
            delete[] this->buffer;
            this->buffer = new unsigned char[new_size];
            this->buffer_length = new_size;
        } else {
            new_size = sz;
        }

        // Copy the contents to the new buffer
        MemCopy(this->buffer, content, content_length);
        this->buffer[content_length] = SENTINEL;

        this->b = 0;
        this->r = this->r - b;
        this->e = this->e - b;
    }

}

static size_t NextSize(size_t c_sz) {
    if (c_sz < MIN_BUFFER_SIZE) return MIN_BUFFER_SIZE;
    if (c_sz <= MAX_BUFFER_SIZE) return 2*c_sz;

    return MAX_BUFFER_SIZE + c_sz;
}

static void MemCopy(void* dst, const void* src, size_t nbytes) {
    // assuming dst and src have atleast nbytes space, HINT: use of memcpy will improve perf
    auto* d = static_cast<unsigned char*>(dst);
    auto* s = static_cast<const unsigned char*>(src);

    for (size_t i = 0; i < nbytes; ++i) {
        d[i] = s[i];
    }
}