// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_CNFREADER_H_
#define INCLUDE_COSY_CNFREADER_H_

#include <vector>
#include <string>

#include "cosy/CNFModel.h"
#include "cosy/Literal.h"
#include "cosy/StreamBuffer.h"
#include "cosy/Logging.h"

namespace cosy {

class CNFReader {
 public:
    CNFReader();
    ~CNFReader();

    bool load(const std::string &filename, CNFModel *model);

 private:
    DISALLOW_COPY_AND_ASSIGN(CNFReader);
};
}  // namespace cosy

#endif  // INCLUDE_COSY_CNFREADER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
