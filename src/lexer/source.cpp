#include <source.h>
#include <position/position.h>
#include <iostream>

static size_t NextSize(size_t c_sz);
static void MemCopy(void* dst, const void* src, size_t nbytes);

namespace source {
    Source::Source(std::string filepath) {
        this->in = std::fstream(filepath, std::ios::in | std::ios::binary);
        if (!this->in.is_open())
        {
            throw std::runtime_error("Could not open file: " + filepath);
        }

        this->b = -1;
        this->r = 0;
        this->e = 0;
        this->chr = ' ';
        this->chw = 0;

        this->buffer_length = 0;
        Source::ResizeOrFlushBuffer(0);  // initialize buffer
        this->buffer[0] = SENTINEL;
        this->offset = 0;
    }

    Source::~Source() {
        this->in.close();
        // deallocate the buffer
        if (this->buffer)
            delete[] this->buffer;
        this->buffer = NULL;
    }

    /* set the begin index to the first outstanding buffer index */
    void Source::Start() {
        this->b = this->r - this->chw;
    }

    /* set the begin index to some invalid index, to unable any processing untill Start() is called */
    void Source::Stop() {
        this->b = -1;
    }

    /* returns the buffer content read from the begin index */
    std::string Source::Segment() {
        std::string res = "";
        for (int i = this->b; i < this->r - this->chw; ++i) {
            res += this->buffer[i];
        }
        return res;
    }

    void Source::ResizeOrFlushBuffer(int sz) {
        unsigned char* content = NULL;
        size_t content_length = 0;
        size_t new_size;
        int b;

        b = this->r;
        if (this->b >= 0)
        {
            b = this->b;
        }
        
        this->GetBufferContent(b, this->e, content, content_length);
        // Check the need to grow the buffer
        if (this->buffer_length <= content_length*2) {
            new_size = NextSize(sz);
            // create new resized buffer
            if (this->buffer)
            {
                delete[] this->buffer;
            }
            this->buffer = new unsigned char[new_size];
            this->buffer_length = new_size;
            // Memset(this->buffer, 0, this->buffer_length);
        } else {
            new_size = sz;
        }

        // Copy the contents to the new buffer
        if (content)
            MemCopy(this->buffer, content, content_length);

        this->buffer[this->buffer_length - 1] = SENTINEL;

        if (this->b >= 0)
        {
            this->b = 0;
        }
        this->r = this->r - b;
        this->e = this->e - b;
    }

    size_t Source::GetCurrentOffset()
    {
        return this->offset;
    }

    unsigned char Source::GetCurrentChr()
    {
        return this->chr;
    }

    void Source::Fill()
    {
        size_t bytes_read = 0;
        /* try resizing or collapsing the buffer */
        Source::ResizeOrFlushBuffer(this->buffer_length);

        /* fill the buffer, leave the last character for sentinal */
        this->in.read((char*)this->buffer, this->buffer_length - 1);
        bytes_read = this->in.gcount();
        /* update buffer */
        this->e += bytes_read;
        this->buffer[this->e] = SENTINEL;
    }

    void Source::NextChr()
    {
        this->offset += this->chw;

        if (this->chr == '\n')
        {
            position::Pos::AddLineOffset(this->offset);
        }

        /* Try filling more bytes to buffer */
        if (this->e == this->r)
            Source::Fill();

        /* EOF */
        if (this->e == this->r)
        {
            this->chr = SENTINEL;
            this->chw = 0;
            return;
        }

        /* read and return one character */
        this->chw = 1;
        this->chr = this->buffer[this->r];
        this->r += this->chw;
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
