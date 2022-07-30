// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_SAUCYREADER_H_
#define INCLUDE_COSY_SAUCYREADER_H_

#include <memory>
#include <string>

#include "cosy/Group.h"
#include "cosy/Permutation.h"
#include "cosy/StreamBuffer.h"

namespace cosy {

class SaucyReader {
 public:
    SaucyReader() {}
    ~SaucyReader() {}

    bool load(const std::string& symmetry_filename, unsigned int num_vars,
              Group *group);
};


}  // namespace cosy

#endif  // INCLUDE_COSY_SAUCYREADER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
