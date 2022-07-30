#include <gtest/gtest.h>

#include "cosy/SymmetryController.h"

namespace cosy {

TEST(SymmetryController, ConstructorFile)  {
    const std::string cnf_filename("tests/resources/simple.cnf");
    const std::string sym_filename("tests/resources/simple.cnf.sym");

    std::unique_ptr<LiteralAdapter<Literal>> adapter
        (new LiteralAdapter<Literal>());

    SymmetryController<Literal> symmetry(cnf_filename, sym_filename, adapter);
}

TEST(SymmetryController, ConstructorBliss)  {
    const std::string cnf_filename("tests/resources/simple.cnf");

    std::unique_ptr<LiteralAdapter<Literal>> adapter
        (new LiteralAdapter<Literal>());

    SymmetryFinder::Automorphism tool = SymmetryFinder::Automorphism::BLISS;
    SymmetryController<Literal> symmetry(cnf_filename, tool, adapter);
}

TEST(SymmetryController, ConstructorSaucy)  {
    const std::string cnf_filename("tests/resources/simple.cnf");

    std::unique_ptr<LiteralAdapter<Literal>> adapter
        (new LiteralAdapter<Literal>());

    SymmetryFinder::Automorphism tool = SymmetryFinder::Automorphism::SAUCY;
    SymmetryController<Literal> symmetry(cnf_filename, tool, adapter);
}

}  // namespace cosy
