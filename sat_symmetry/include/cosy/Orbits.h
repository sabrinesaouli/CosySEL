// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_ORBITS_H_
#define INCLUDE_COSY_ORBITS_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "cosy/DisjointSets.h"
#include "cosy/Literal.h"
#include "cosy/Permutation.h"

namespace cosy {

class Orbits {
 public:
    Orbits() {}
    ~Orbits() {}

    void assign(const std::vector<Permutation*> permutations);

    const std::vector< std::vector<BooleanVariable> >::const_iterator begin() {
        return _orbits.begin();
    }
    const std::vector< std::vector<BooleanVariable> >::const_iterator end() {
        return _orbits.end();
    }

    // const std::vector< std::vector<BooleanVariable> > orbits() const;

    int64 numberOfOrbits() const { return _orbits.size(); }
 private:
    std::vector<BooleanVariable> _symmetrics;
    std::vector< std::vector<BooleanVariable> > _orbits;
};

}  // namespace cosy
#endif  // INCLUDE_COSY_ORBITS_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
