// Copyright 2017 Hakan Metin

#ifndef INCLUDE_COSY_CNFGRAPH_H_
#define INCLUDE_COSY_CNFGRAPH_H_

#include <vector>

#include "cosy/CNFModel.h"
#include "cosy/Literal.h"
#include "cosy/Macros.h"
#include "cosy/IntegralTypes.h"

namespace cosy {

static const int kLiteralColor = 0;
static const int kClauseColor  = 1;

static inline
unsigned int literal2Node(const Literal& literal, unsigned int n) {
    // return literal.index().value();

    unsigned int v = literal.variable().value();
    if (literal.isPositive())
        return v;
    else
        return n + v;
}

static inline LiteralIndex node2Literal(unsigned int node, unsigned int n) {
    // if (node < 2*n)
    //     return LiteralIndex(node);
    // else
    //     return kNoLiteralIndex;

    if (node < n)
        return Literal(node+1).index();
    else if (node < 2*n)
        return Literal(-(node - n + 1)).index();
    else
        return kNoLiteralIndex;
}


class CNFGraph {
 public:
    CNFGraph();
    explicit CNFGraph(const CNFModel& model);
    ~CNFGraph();

    void assign(const CNFModel& model);

    unsigned int numberOfNodes() const { return _num_nodes; }
    unsigned int numberOfEdges() const { return _num_edges; }

    const std::vector<unsigned int> neighbour(unsigned int node) const {
        return _adjacency[node];
    }

    unsigned int degree(unsigned int node) const {
        return _adjacency[node].size();
    }

    unsigned int color(unsigned int node) const {
        return _colors[node];
    }

 private:
    int64 _num_nodes;
    int64 _num_edges;

    std::vector< std::vector<unsigned int> > _adjacency;
    std::vector<unsigned int> _colors;

    void addNode(unsigned int node);
    void addEdge(unsigned int from, unsigned int to);
    void changeColor(unsigned int node, unsigned int color);

    DISALLOW_COPY_AND_ASSIGN(CNFGraph);
};
}  // namespace cosy

#endif  // INCLUDE_COSY_CNFGRAPH_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
