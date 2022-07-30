// Copyright 2017 Hakan Metin - LIP6

#include "cosy/Permutation.h"

namespace cosy {

void Permutation::addToCurrentCycle(Literal x) {
    const int cs = _cycles.size();
    const int back = _cycles_lim.empty() ? 0 : _cycles_lim.back();
    _cycles.push_back(x);

    // Add image and inverse to access in lookup unordered_map
    if (_cycles.size() > 0 && cs != back) {
        const Literal e = _cycles[cs - 1];
        const Literal i = _cycles[cs];
        _image[e] = i;
        _inverse[i] = e;
    }
}

void Permutation::closeCurrentCycle() {
    const int sz = _cycles.size();
    int last = _cycles_lim.empty() ? 0 : _cycles_lim.back();

    if (last == sz)
        return;

    DCHECK_GE(sz - last, 2);
    _cycles_lim.push_back(sz);

    // Add image and inverse to access in lookup unordered_map
    const int num_cycle = _cycles_lim.size() - 1;
    const Literal e = lastElementInCycle(num_cycle);
    const Literal i = *(cycle(num_cycle).begin());
    _image[e] = i;
    _inverse[i] = e;
}

Permutation::Iterator Permutation::cycle(unsigned int i) const {
    //    DCHECK_GE(i, static_cast<unsigned int>(0));
    DCHECK_LT(i, numberOfCycles());

    return Iterator(_cycles.begin() + (i == 0 ? 0 : _cycles_lim[i - 1]),
        _cycles.begin() + _cycles_lim[i]);
}

Literal Permutation::lastElementInCycle(unsigned int i) const {
    // DCHECK_GE(i, static_cast<unsigned int>(0));
    DCHECK_LT(i, numberOfCycles());

    return _cycles[_cycles_lim[i] - 1];
}

const Literal Permutation::imageOf(const Literal& element) const {
    return _image.at(element);
}
const Literal Permutation::inverseOf(const Literal& element) const {
    return _inverse.at(element);
}

bool Permutation::isTrivialImage(const Literal& element) const {
    return _image.find(element) == _image.end();
}
bool Permutation::isTrivialInverse(const Literal& element) const {
    return _inverse.find(element) == _inverse.end();
}


void Permutation::debugPrint() const {
    CHECK_NE(numberOfCycles(), 0);

    for (unsigned int c = 0; c < numberOfCycles(); ++c) {
        std::cout << "(";
        for (const Literal& element : cycle(c)) {
            if (element == lastElementInCycle(c))
                std::cout << element.signedValue();
            else
                std::cout << element.signedValue() << " ";
        }
        std::cout << ")";
    }
    std::cout << std::endl;
}


}  // namespace cosy


/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
