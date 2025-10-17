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

#ifndef __SOURCE_H__
#define __SOURCE_H__

#include <iostream>  // remove this
#include <vector>
#include <string>
#include <fstream>
#include <utility>

#define SENTINEL 128
#define MIN_BUFFER_SIZE 4096
#define MAX_BUFFER_SIZE 10 * 1024

namespace source {
    class Source {
        std::fstream in;

        // indices
        int b;
        int r;
        int e;
        // buffer
        unsigned char *buffer = NULL;
        size_t buffer_length = 0;
        // file offset
        size_t offset;

        unsigned char chr;  // the latest character read
        int chw;            // width of the latest character read (if read 1, otherwise 0)

        /* content length is from b to e excluding e */
        void GetBufferContent(int s, int e, unsigned char** c, size_t &c_len) {
            if (e - s <= 0)
            {
                c_len = 0;
                c = NULL;
                return;
            }
            // TODO: assert buffer is not nil
            unsigned char* content = new unsigned char[e - s];
            for (int i = s; i < e; ++i) {
                content[i - s] = this->buffer[i];
            }

            *c = content;
            c_len = e - s;
        }
        public:

            Source(std::string filepath);
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

            /* get the current file offset */
            size_t GetCurrentOffset();

            /* get the current character read */
            unsigned char GetCurrentChr();
    };
}


#endif // __SOURCE_H__