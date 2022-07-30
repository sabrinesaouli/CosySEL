// Copyright 2017 Hakan Metin - LIP6

#include "cosy/Group.h"
#include "cosy/Printer.h"

namespace cosy {

Group::Group() {
}

Group::~Group() {
}

void Group::addPermutation(std::unique_ptr<Permutation>&& permutation) {
    CHECK_NOTNULL(permutation);

    const unsigned int permutation_index = _permutations.size();
    const unsigned int num_cycles = permutation->numberOfCycles();

    if (num_cycles == 0)
        return;

    if (isPermutationSpurious(permutation))
        return;


    if (permutation->size() > _watchers.size())
        _watchers.resize(permutation->size());

    for (unsigned int c = 0; c < num_cycles; ++c) {
        Literal element = permutation->lastElementInCycle(c);

        for (const Literal& image : permutation->cycle(c)) {
            const int index = image.variable().value();
            if (image.isPositive())
                _watchers[index].push_back(permutation_index);

            const BooleanVariable variable = image.variable();
            _symmetric.insert(variable);

            if (element == image.negated())
                _inverting.insert(variable);

            element = image;
        }
    }

    _permutations.emplace_back(permutation.release());
}

Group::Iterator Group::watch(BooleanVariable variable) const {
    const int index = variable.value();
    return Iterator(_watchers[index].begin(), _watchers[index].end());
}

bool Group::isPermutationSpurious(const std::unique_ptr<Permutation>& p) const {
    const unsigned int num_cycles = p->numberOfCycles();
    for (unsigned int c = 0; c < num_cycles; ++c) {
        for (const Literal& element : p->cycle(c)) {
            if (p->isTrivialImage(element))
                return true;
            if (p->isTrivialImage(element.negated()))
                return true;
            if (p->imageOf(element).negated() != p->imageOf(element.negated()))
                return true;
        }
    }

    return false;
}

void Group::summarize(unsigned int num_vars) const {
    Printer::printStat("Number of generators", numberOfPermutations());
    Printer::printStat("Number of vars in generators",
                       numberOfSymmetricVariables(),
                       static_cast<int64>(num_vars));
    Printer::printStat("Number of inverting", numberOfInverting());
}

void Group::debugPrint() const {
    unsigned int i = 1;
    for (const std::unique_ptr<Permutation>& permutation : _permutations) {
        std::cout << "[" << i++ << "]: ";
        permutation->debugPrint();
    }
}

}  // namespace cosy


/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
