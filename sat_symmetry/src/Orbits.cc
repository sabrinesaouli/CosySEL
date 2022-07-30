// Copyright 2017 Hakan Metin - LIP6

#include "cosy/Orbits.h"

namespace cosy {

void Orbits::assign(const std::vector<Permutation*> permutations) {
    std::unordered_set<BooleanVariable> used;
    DisjointSets disjointSets;
    int e, i;

    _symmetrics.clear();
    _orbits.clear();

    for (const Permutation* permutation : permutations) {
        for (unsigned int c = 0; c < permutation->numberOfCycles(); ++c) {
            Literal element = permutation->lastElementInCycle(c);
            e = element.variable().value();
            disjointSets.Add(e);

            for (const Literal& image : permutation->cycle(c)) {
                i = image.variable().value();

                if (used.insert(element.variable()).second)
                    _symmetrics.push_back(element.variable());

                disjointSets.Add(i);
                disjointSets.Union(e, i);

                element = image;
                e = element.variable().value();
            }
        }
    }

    std::unordered_map<int, std::vector<BooleanVariable>> m_orbits;
    for (const BooleanVariable& variable : _symmetrics) {
        int representant = disjointSets.Find(variable.value());
        m_orbits[representant].push_back(variable);
    }

    for (const auto& orbit : m_orbits)
        _orbits.emplace_back(std::move(orbit.second));
}

}  // namespace cosy
