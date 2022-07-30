// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_PERMUTATION_H_
#define INCLUDE_COSY_PERMUTATION_H_

#include <unordered_map>
#include <vector>
#include <string>

#include "cosy/Literal.h"
#include "cosy/Logging.h"

namespace cosy {

class Permutation {
 public:
    explicit Permutation(unsigned int size) : _size(size) {}
    ~Permutation() {}

    void addToCurrentCycle(Literal x);
    void closeCurrentCycle();

    unsigned int size()           const { return _size; }
    unsigned int numberOfCycles() const { return _cycles_lim.size(); }
    bool isIdentity()             const { return _cycles.empty(); }

    const std::vector<Literal>& support() const { return _cycles; }
    struct Iterator;
    Iterator cycle(unsigned int i) const;
    Literal lastElementInCycle(unsigned int i) const;

    const Literal imageOf(const Literal& element) const;
    const Literal inverseOf(const Literal& element) const;

    bool isTrivialImage(const Literal& element) const;
    bool isTrivialInverse(const Literal& element) const;

    void debugPrint() const;

 private:
    const int _size;
    std::vector<Literal> _cycles;
    std::vector<int> _cycles_lim;

    std::unordered_map<Literal, Literal> _image;
    std::unordered_map<Literal, Literal> _inverse;

    DISALLOW_COPY_AND_ASSIGN(Permutation);
};

struct Permutation::Iterator {
    typedef Literal value_type;
    typedef std::vector<Literal>::const_iterator const_iterator;

    Iterator() {}
    Iterator(const std::vector<Literal>::const_iterator& b,
             const std::vector<Literal>::const_iterator& e) :
        _begin(b), _end(e) {}

    std::vector<Literal>::const_iterator begin() const { return _begin; }
    std::vector<Literal>::const_iterator end() const { return _end; }
    const std::vector<Literal>::const_iterator _begin;
    const std::vector<Literal>::const_iterator _end;

    int size() const { return _end - _begin; }
};

}  // namespace cosy

#endif  // INCLUDE_COSY_PERMUTATION_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
