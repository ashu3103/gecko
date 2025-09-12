/* 
A character reader library from a file source.

Implements a dynamic buffer to store a stream of characters from a file reader. The buffer only allows ASCII
characters and a Sentinal character (value: 128) to mark the end of the buffer.

It contains three indices that are used to access the buffer.
- b (begin) :- Represents the starting of a segment, every character before this point is read and can be discarded from buffer.
- r (read)  :- Represents the next unread character in the buffer.
- e (end)   :- Represents the last point of the buffer, often points to a sentinel character.

Segment is nothing but the substring of buffer read from the point `b`
*/

#ifndef SOURCE_H
#define SOURCE_H

#include <vector>
#include <string>
#include <fstream>
#include <utility>

#define SENTINEL 128
#define MIN_BUFFER_SIZE 4096
#define MAX_BUFFER_SIZE 0

namespace source {
    class Source {
        std::fstream in;

        // indices
        size_t b;
        size_t r;
        size_t e;
        // buffer
        unsigned char *buffer = NULL;
        size_t buffer_length = 0;
        // location
        int line;
        int col;

        unsigned char chr;  // the latest character read
        int chw;            // width of the latest character read (if read 1, otherwise 0)

        /* content length is from b to e excluding e */
        void GetBufferContent(unsigned char* &c, size_t &c_len) {
            // TODO: assert buffer is not nil
            unsigned char* content = new unsigned char[this->e - this->b];
            for (size_t i = this->b; i < this->e; ++i) {
                content[i] = this->buffer[i];
            }
            c = content;
            c_len = this->e - this->b;
        }

        public:
            Source(std::fstream&& in);
            ~Source();

            void Start();
            void Stop();
            std::string Segment();

            /* If segment size exceeds the buffer length grow otherwise flush the read characters from the buffer */
            void ResizeOrFlushBuffer(int sz);

            /* Fill the buffer */
            void Fill();

            /* read the next character */
            void NextChr();
    };
}


#endif // SOURCE_H