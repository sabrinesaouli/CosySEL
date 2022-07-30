// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_SYMMETRYFINDER_H_
#define INCLUDE_COSY_SYMMETRYFINDER_H_

#include <memory>
#include <string>

#include "cosy/CNFModel.h"
#include "cosy/CNFGraph.h"
#include "cosy/Group.h"
#include "cosy/Stats.h"
#include "cosy/AutomorphismBuilder.h"
#include "cosy/AutomorphismFinder.h"
#include "cosy/BlissAutomorphismFinder.h"
#include "cosy/SaucyAutomorphismFinder.h"


namespace cosy {


class SymmetryFinder {
 public:
    enum Automorphism {
        BLISS,
        SAUCY
    };

    SymmetryFinder() {}
    virtual ~SymmetryFinder() {}

    void findAutomorphism(const CNFModel& model, SymmetryFinder::Automorphism tool,
                          Group *group);

    void printStats() const {
        Printer::printStat("Automorhism tool", _tool_name);
        _stats.print();
    }

 private:
    std::string _tool_name;

    struct Stats : public StatsGroup {
        Stats() : StatsGroup("Symmetry Finder"),
                  find_time("Automorphism time", this) {}
        TimeDistribution find_time;
    };
    Stats _stats;
};

}  // namespace cosy

#endif  // INCLUDE_COSY_SYMMETRYFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
