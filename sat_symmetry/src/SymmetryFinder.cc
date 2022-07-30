// Copyright 2017 Hakan Metin - LIP6

#include "cosy/SymmetryFinder.h"

namespace cosy {


void SymmetryFinder::findAutomorphism(const CNFModel& model,
                                      SymmetryFinder::Automorphism tool,
                                      Group *group) {
    switch (tool) {
    case BLISS:
        {
            AutomorphismBuilder<BlissAutomorphismFinder,
                                DoubleLiteralGraphNodeAdaptor> finder;
            finder.findAutomorphisms(model, group);
            _tool_name = "Bliss";
        }
        break;
    case SAUCY:
        {
            AutomorphismBuilder<SaucyAutomorphismFinder,
                                DoubleLiteralGraphNodeAdaptor> finder;
            finder.findAutomorphisms(model, group);
            _tool_name = "Saucy";
        }
        break;

    default:
        _tool_name = "null";
    }
}


}  // namespace cosy
