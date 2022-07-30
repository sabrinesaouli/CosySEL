// Copyright 2017 Hakan Metin - LIP6

#include "cosy/CNFModel.h"
#include "cosy/Printer.h"

namespace cosy {

CNFModel::CNFModel() :
    _num_variables(0),
    _num_clauses(0),
    _num_unary_clauses(0),
    _num_binary_clauses(0),
    _num_ternary_clauses(0),
    _num_large_clauses(0) {
}

CNFModel::~CNFModel() {
}

void CNFModel::addClause(std::vector<Literal>* literals) {
    CHECK_GT(literals->size(), static_cast<unsigned int>(0));

    /* Remove duplicate literals in clause */
    std::sort(literals->begin(), literals->end());
    auto last_element_it = std::unique(literals->begin(), literals->end());
    literals->erase(last_element_it, literals->end());

    BooleanVariable var = literals->back().variable();
    if (var > _num_variables)
        _num_variables = var.value();

    _num_clauses++;

    size_t tag = compute_tag(*literals);

    // If clause already exists do nothing
    if (!(_clauses_tag.insert(tag)).second)
        return;

    std::unique_ptr<Clause> clause(Clause::create(*literals));
    _clauses.emplace_back(clause.release());

    compute_occurences(*literals);
    compute_sizes(*literals);
}

size_t CNFModel::compute_tag(const std::vector<Literal>& literals) {
    std::string word = "";
    for (const Literal& literal : literals)
        word += std::to_string(literal.signedValue()) + " ";
    return std::hash<std::string>()(word);
}

void CNFModel::compute_occurences(const std::vector<Literal>& literals) {
    const int64 num_vars = numberOfVariables();
    for (const Literal& literal : literals) {
        const int64 index = literal.variable().value();

        if (num_vars > index) {
            _positive_occurences.resize(num_vars);
            _negative_occurences.resize(num_vars);
            _occurences.resize(num_vars);
        }

        if (literal.isPositive())
            _positive_occurences[index]++;
        else
            _negative_occurences[index]++;
        _occurences[index]++;
    }
}

void CNFModel::compute_sizes(const std::vector<Literal>& literals) {
    switch (literals.size()) {
    case 1:  _num_unary_clauses++;   break;
    case 2:  _num_binary_clauses++;  break;
    case 3:  _num_ternary_clauses++; break;
    default: _num_large_clauses++;   break;
    }
}

void CNFModel::summarize() const {
    const int64 ncl = numberOfClauses();

    Printer::printSection(" Instance Informations ");
    Printer::printStat("Number of variables", numberOfVariables());
    Printer::printStat("Number of clauses", ncl);
    Printer::printStat(" |- unary clauses", numberOfUnaryClauses(), ncl);
    Printer::printStat(" |- binary clauses", numberOfBinaryClauses(), ncl);
    Printer::printStat(" |- ternary clauses", numberOfTernaryClauses(), ncl);
    Printer::printStat(" |- large clauses", numberOfLargeClauses(), ncl);
}

}  // namespace cosy

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
