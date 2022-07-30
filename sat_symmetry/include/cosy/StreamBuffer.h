// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_STREAMBUFFER_H_
#define INCLUDE_COSY_STREAMBUFFER_H_

#include <zlib.h>

#include <cctype>
#include <string>

#include "cosy/Logging.h"
#include "cosy/Literal.h"

static const int kBufferSize = 4096;

namespace cosy {

class StreamBuffer {
 public:
    explicit StreamBuffer(const std::string& filename);
    explicit StreamBuffer(const char * filename);
    ~StreamBuffer();

    int readInt();
    void skipWhiteSpaces();
    void skipLine();

    int operator*();
    void operator++();

 private:
    const std::string _filename;
    gzFile _in;
    unsigned char _buffer[kBufferSize];
    unsigned int _index;
    unsigned int _size;

    unsigned char read();
};

}  // namespace cosy


#endif  // INCLUDE_COSY_STREAMBUFFER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
