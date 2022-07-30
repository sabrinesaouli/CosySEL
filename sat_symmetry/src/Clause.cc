// Copyright 2017 Hakan Metin - LIP6

# include "cosy/Clause.h"

namespace cosy {

// static
Clause* Clause::create(const std::vector<Literal>& literals) {
    Clause *clause = reinterpret_cast<Clause*>(
        ::operator new(sizeof(Clause) + literals.size() * sizeof(Literal)));

    clause->_size  = literals.size();
    for (unsigned int i = 0; i < literals.size(); i++) {
        clause->_literals[i] = literals[i];
    }

    return clause;
}

}  // namespace cosy
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
