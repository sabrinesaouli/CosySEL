// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_CLAUSE_H_
#define INCLUDE_COSY_CLAUSE_H_

#include <string>
#include <vector>

#include "cosy/Literal.h"
#include "cosy/Logging.h"

namespace cosy {

class Clause {
 public:
    static Clause* create(const std::vector<Literal>& literals);

    // Non-sized delete because this is a tail-padded class.
    void operator delete(void* p) {
        ::operator delete(p);  // non-sized delete
    }

    // Allows for range based iteration: for (Literal literal : clause) {}.
    const Literal*  begin() const { return &(_literals[0]); }
    const Literal*  end() const { return &(_literals[_size]); }
    Literal* literals() { return &(_literals[0]); }

    int  size() const { return _size; }

 private:
    // Packed data, use 32 bits
    unsigned int _size : 32;

    // This class store the literals inline, and literals_ mark the starts of
    // the variable length portion.
    Literal _literals[0];

    DISALLOW_COPY_AND_ASSIGN(Clause);
};

}  // namespace cosy

#endif  // INCLUDE_COSY_CLAUSE_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
