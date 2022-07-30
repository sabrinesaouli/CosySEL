// Copyright 2017 Hakan Metin

#ifndef INCLUDE_COSY_SAUCYAUTOMORPHISMFINDER_H_
#define INCLUDE_COSY_SAUCYAUTOMORPHISMFINDER_H_

#include <saucy/saucy.h>

#include <vector>
#include <numeric>
#include <memory>
#include <utility>

#include "cosy/AutomorphismFinder.h"
#include "cosy/ColoredGraph.h"
#include "cosy/Group.h"
#include "cosy/LiteralGraphNodeAdaptor.h"

namespace cosy {

class SaucyAutomorphismFinder :
        public AdjacencyColoredGraph, AutomorphismFinder {
 public:
    SaucyAutomorphismFinder();
    explicit SaucyAutomorphismFinder(unsigned int num_nodes);
    virtual ~SaucyAutomorphismFinder();

    void findAutomorphisms(unsigned int num_vars, const Adaptor& adaptor,
                           Group *group) override;
};

inline SaucyAutomorphismFinder::SaucyAutomorphismFinder() :
    AdjacencyColoredGraph(), AutomorphismFinder() {
}

inline SaucyAutomorphismFinder::SaucyAutomorphismFinder(unsigned int num_nodes) :
    AdjacencyColoredGraph(num_nodes), AutomorphismFinder() {
}

inline SaucyAutomorphismFinder::~SaucyAutomorphismFinder() {
}


static int
on_saucy_automorphism(int n, const int *aut, int k ATTRIBUTE_UNUSED,
                      int *support ATTRIBUTE_UNUSED, void *arg) {
    AutomorphismInfo *info = static_cast<AutomorphismInfo*>(arg);
    unsigned int num_vars = info->num_vars;
    LiteralGraphNodeAdaptor *adaptor = info->adaptor;
    Group *group = info->group;
    std::unique_ptr<Permutation> permutation =
        std::make_unique<Permutation>(num_vars);
    std::vector<bool> seen(n);
    LiteralIndex index;

    for (int i = 0; i < n; ++i) {
        if (i == aut[i] || seen[i])
            continue;

        index = adaptor->nodeToLiteral(i + 1);
        if (index != kNoLiteralIndex)
            permutation->addToCurrentCycle(Literal(index));

        seen[i] = true;

        for (int j = aut[i]; j != i; j = aut[j]) {
            seen[j] = true;
            index = adaptor->nodeToLiteral(j + 1);
            if (index != kNoLiteralIndex)
                permutation->addToCurrentCycle(Literal(index));
        }
        permutation->closeCurrentCycle();
    }

    group->addPermutation(std::move(permutation));
    return 1;  // Always continue to search
}

inline void
SaucyAutomorphismFinder::findAutomorphisms(unsigned int num_vars,
                                           const Adaptor& adaptor,
                                           Group *group) {
    AutomorphismInfo info(num_vars, adaptor.get(), group);

    int n = _num_nodes;
    int e = _num_edges;
    std::vector<int> adj;
    std::vector<int> edg;

    adj.push_back(0);
    unsigned int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += _adjacency[i].size();
        adj.push_back(sum);
    }

    for (int i = 0; i < n; i++)
        for (const unsigned int& x : _adjacency[i])
            edg.push_back(x);

    // Initialize saucy structure
    struct saucy *s = reinterpret_cast<struct saucy*>(saucy_alloc(n));
    struct saucy_graph  *g = reinterpret_cast<struct saucy_graph*>
        (malloc(sizeof(struct saucy_graph)));

    g->n = n;
    g->e = e;
    g->adj = adj.data();
    g->edg = edg.data();

    struct saucy_stats stats;
    saucy_search(s, g, 0, _colors.data(), on_saucy_automorphism,
                 static_cast<void*>(&info), &stats);
    free(g);
    saucy_free(s);
}

}  // namespace cosy


#endif  // INCLUDE_COSY_SAUCYAUTOMORPHISMFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
