// Copyright 2017 Hakan Metin
#include "cosy/CNFGraph.h"


namespace cosy {

CNFGraph::CNFGraph() :
    _num_nodes(0),
    _num_edges(0) {
}

CNFGraph::CNFGraph(const CNFModel& model) : CNFGraph() {
    assign(model);
}

CNFGraph::~CNFGraph() {
}

void CNFGraph::assign(const CNFModel& model) {
    unsigned int n = model.numberOfVariables();
    unsigned int num_clauses = 2 * n;
    unsigned int x, y, z;
    bool opt_optimized_graph = true;

    // Graph edges
    for (const std::unique_ptr<Clause>& clause : model.clauses()) {
        if (opt_optimized_graph && clause->size() == 2) {
            x = literal2Node(clause->literals()[0], n);
            y = literal2Node(clause->literals()[1], n);
            addEdge(x, y);
        } else {
            for (const Literal& literal : *clause) {
                z = literal2Node(literal, n);
                addEdge(z, num_clauses);
            }
            num_clauses++;
        }
    }
    // Boolean consistency
    for (BooleanVariable var(0); var < n; ++var) {
        x = literal2Node(Literal(var, true), n);
        y = literal2Node(Literal(var, false), n);
        addEdge(x, y);
    }
    // Node color
    int color = kClauseColor + 1;
    for (BooleanVariable var(0); var < n; ++var) {
        x = literal2Node(Literal(var, true), n);
        y = literal2Node(Literal(var, false), n);
        if (degree(x) == 1 && degree(y) == 1) {
            changeColor(x, color++);
            changeColor(y, color++);
        }
    }
    // Clause color
    for (unsigned int i=2*n; i < num_clauses; ++i)
        changeColor(i, kClauseColor);
}

void CNFGraph::addNode(unsigned int node) {
    if (_num_nodes > node)
        return;
    _num_nodes = node + 1;
    _adjacency.resize(_num_nodes);
    _colors.resize(_num_nodes);
}

void CNFGraph::addEdge(unsigned int from, unsigned int to) {
    addNode(from > to ? from : to);
    _adjacency[from].push_back(to);
    _adjacency[to].push_back(from);
    _num_edges++;
}

void CNFGraph::changeColor(unsigned int node, unsigned int color) {
    _colors[node] = color;
}

}  // namespace cosy
