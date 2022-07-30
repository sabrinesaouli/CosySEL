// Copyright 2017 Hakan Metin

#ifndef INCLUDE_COSY_BLISSAUTOMORPHISMFINDER_H_
#define INCLUDE_COSY_BLISSAUTOMORPHISMFINDER_H_

#include <bliss/graph.hh>

#include <vector>
#include <numeric>
#include <memory>
#include <utility>

#include "cosy/AutomorphismFinder.h"
#include "cosy/ColoredGraph.h"
#include "cosy/Group.h"
#include "cosy/LiteralGraphNodeAdaptor.h"


namespace cosy {

class BlissAutomorphismFinder : public ColoredGraph, AutomorphismFinder {
 public:
    BlissAutomorphismFinder();
    explicit BlissAutomorphismFinder(unsigned int num_nodes);
    virtual ~BlissAutomorphismFinder();

    void addNode(NodeIndex node) override;
    void addEdge(NodeIndex a, NodeIndex b) override;
    void setColor(NodeIndex node, unsigned int color) override;

    void findAutomorphisms(unsigned int num_vars, const Adaptor& adaptor,
                           Group *group) override;

 private:
    std::unique_ptr<bliss::Graph> _graph;
};

inline BlissAutomorphismFinder::BlissAutomorphismFinder() :
    ColoredGraph() {
    _graph = std::make_unique<bliss::Graph>();
}

inline BlissAutomorphismFinder::BlissAutomorphismFinder(unsigned int num_nodes) :
    ColoredGraph(num_nodes) {
    _graph = std::make_unique<bliss::Graph>(num_nodes);
}

inline BlissAutomorphismFinder::~BlissAutomorphismFinder() {
}

inline void BlissAutomorphismFinder::addNode(NodeIndex node) {
    unsigned int n;
    while (_num_nodes <= node) {
        n = _graph->add_vertex();
        CHECK_EQ(n, _num_nodes);
        _num_nodes = node + 1;
    }
}

inline void BlissAutomorphismFinder::addEdge(NodeIndex a, NodeIndex b) {
    _graph->add_edge(a, b);
    _num_edges++;
}

inline void BlissAutomorphismFinder::setColor(NodeIndex node, unsigned int color) {
    CHECK_LT(node, _num_nodes);
    _graph->change_color(node, color);
}

static void
on_bliss_automorphim(void* arg, const unsigned int n, const unsigned int* aut) {
    AutomorphismInfo *info = static_cast<AutomorphismInfo*>(arg);

    unsigned int num_vars = info->num_vars;
    LiteralGraphNodeAdaptor *adaptor = info->adaptor;
    Group *group = info->group;
    std::unique_ptr<Permutation> permutation =
        std::make_unique<Permutation>(num_vars);
    std::vector<bool> seen(n);
    LiteralIndex index;

    for (unsigned int i = 0; i < n; ++i) {
        if (i == aut[i] || seen[i])
            continue;

        index = adaptor->nodeToLiteral(i + 1);
        if (index != kNoLiteralIndex)
            permutation->addToCurrentCycle(Literal(index));

        seen[i] = true;

        for (unsigned int j = aut[i]; j != i; j = aut[j]) {
            seen[j] = true;
            index = adaptor->nodeToLiteral(j + 1);
            if (index != kNoLiteralIndex)
                permutation->addToCurrentCycle(Literal(index));
        }
        permutation->closeCurrentCycle();
    }
    group->addPermutation(std::move(permutation));
}

inline void
BlissAutomorphismFinder::findAutomorphisms(unsigned int num_vars,
                                           const Adaptor& adaptor,
                                           Group *group) {
        bliss::Stats stats;
        AutomorphismInfo info(num_vars, adaptor.get(), group);

        _graph->find_automorphisms(stats, &on_bliss_automorphim, &info);
        _graph = nullptr;
}

}  // namespace cosy

#endif  // INCLUDE_COSY_BLISSAUTOMORPHISMFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
