// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_COSY_SYMMETRYCONTROLLER_H_
#define INCLUDE_COSY_SYMMETRYCONTROLLER_H_

#include <memory>
#include <utility>
#include <vector>
#include <string>

#include "cosy/CosyManager.h"
#include "cosy/ClauseInjector.h"
#include "cosy/CNFModel.h"
#include "cosy/CNFReader.h"
#include "cosy/Group.h"
#include "cosy/LiteralAdapter.h"
#include "cosy/Logging.h"
#include "cosy/OrderFactory.h"
#include "cosy/Printer.h"
#include "cosy/SaucyReader.h"
#include "cosy/BreakIDReader.h"
#include "cosy/SymmetryFinder.h"


namespace cosy {

enum SymmetryReader {
    SAUCY_SYM,
    BREAKID_SYM,
};

template<class T>
class SymmetryController {
 public:
    SymmetryController(const std::string& cnf_filename,
                       const std::string& symmetry_filename,
                       const SymmetryReader reader,
                       const std::unique_ptr<LiteralAdapter<T>>& adapter);

    SymmetryController(const std::string& cnf_filename,
                       SymmetryFinder::Automorphism tool,
                       const std::unique_ptr<LiteralAdapter<T>>& adapter);

    virtual ~SymmetryController() {}

    void enableCosy(OrderMode vars, ValueMode value);

    void updateNotify(T literal_s, unsigned int level, bool isDecision);
    void updateCancel(T literal_s);

    bool hasClauseToInject(ClauseInjector::Type type, T literal_s) const;
    std::vector<T> clauseToInject(ClauseInjector::Type type, T literal_s);

    bool hasClauseToInject(ClauseInjector::Type type) const;
    std::vector<T> clauseToInject(ClauseInjector::Type type);

    void printInfo() const;
    void printStats() const;

 private:
    unsigned int _num_vars;
    const std::unique_ptr<LiteralAdapter<T>>& _literal_adapter;
    Group _group;
    CNFModel _cnf_model;
    Assignment _assignment;
    ClauseInjector _injector;
    SymmetryFinder _symmetry_finder;

    std::unique_ptr<CosyManager> _cosy_manager;

    bool loadCNFProblem(const std::string cnf_filename);
    std::vector<T> adaptVector(const std::vector<Literal>& literals);
};

// Implementation

template<class T> inline
bool SymmetryController<T>::loadCNFProblem(const std::string cnf_filename) {
    CNFReader cnf_reader;
    bool success;

    success = cnf_reader.load(cnf_filename, &_cnf_model);
    if (!success) {
        LOG(ERROR) << "CNF file " << cnf_filename << " is not well formed.";
        return false;
    }
    _num_vars = _cnf_model.numberOfVariables();
    _assignment.resize(_num_vars);

    return true;
}


template<class T>
inline SymmetryController<T>::SymmetryController(
                           const std::string& cnf_filename,
                           const std::string& sym_filename,
                           const SymmetryReader reader,
                           const std::unique_ptr<LiteralAdapter<T>>& adapter) :
    _literal_adapter(adapter),
    _cosy_manager(nullptr) {
    bool success;

    if (!loadCNFProblem(cnf_filename))
        return;

    if (reader == SAUCY_SYM) {
        SaucyReader sym_reader;
        success = sym_reader.load(sym_filename, _num_vars, &_group);
    } else if (reader == BREAKID_SYM) {
        BreakIDReader sym_reader;
        success = sym_reader.load(sym_filename, _num_vars, &_group);
        // _group.debugPrint();
    } else {
        success = false;
        assert(false);
    }
    if (!success) {
        LOG(ERROR) << "Saucy file " << sym_filename << " is not well formed.";
    }
}

template<class T>
inline SymmetryController<T>::SymmetryController(
                            const std::string& cnf_filename,
                            SymmetryFinder::Automorphism tool,
                            const std::unique_ptr<LiteralAdapter<T>>& adapter) :
    _literal_adapter(adapter),
    _cosy_manager(nullptr) {
    if (!loadCNFProblem(cnf_filename))
        return;

    _symmetry_finder.findAutomorphism(_cnf_model, tool, &_group);
}

template<class T>
inline void SymmetryController<T>::enableCosy(OrderMode vars, ValueMode value) {
    if (_group.numberOfPermutations() == 0)
        return;

    std::unique_ptr<Order> order
        (OrderFactory::create(vars, value, _cnf_model, _group));
    CHECK_NOTNULL(order);

    _cosy_manager = std::unique_ptr<CosyManager>
        (new CosyManager(_group, _assignment));

    _cosy_manager->defineOrder(std::move(order));
    _cosy_manager->generateUnits(&_injector);
}

template<class T>
inline void SymmetryController<T>::updateNotify(T literal_s,
                                                unsigned int level,
                                                bool isDecision) {
    cosy::Literal literal_c = _literal_adapter->convertTo(literal_s);
    _assignment.assignFromTrueLiteral(literal_c);
    if (_cosy_manager)
        _cosy_manager->updateNotify(literal_c, &_injector);
}

template<class T>
inline void SymmetryController<T>::updateCancel(T literal_s) {
    cosy::Literal literal_c = _literal_adapter->convertTo(literal_s);

    if (!_assignment.literalIsAssigned(literal_c))
        return;

    _assignment.unassignLiteral(literal_c);

    if (_cosy_manager)
        _cosy_manager->updateCancel(literal_c);
}

template<class T> inline bool
SymmetryController<T>::hasClauseToInject(ClauseInjector::Type type,
                                         T literal_s) const {
    cosy::Literal literal_c = _literal_adapter->convertTo(literal_s);
    return _injector.hasClause(type, literal_c.variable());
}


template<class T> inline std::vector<T>
SymmetryController<T>::clauseToInject(ClauseInjector::Type type, T literal_s) {
    cosy::Literal literal_c =  _literal_adapter->convertTo(literal_s);
    std::vector<cosy::Literal> literals_c =
        std::move(_injector.getClause(type, literal_c.variable()));
    std::vector<T> literals_s = adaptVector(literals_c);
    return literals_s;
}

template<class T> inline bool
SymmetryController<T>::hasClauseToInject(ClauseInjector::Type type) const {
    return _injector.hasClause(type, kNoBooleanVariable);
}

template<class T> inline std::vector<T>
SymmetryController<T>::clauseToInject(ClauseInjector::Type type) {
    std::vector<cosy::Literal> literals_c =
        std::move(_injector.getClause(type, kNoBooleanVariable));
    std::vector<T> literals_s = adaptVector(literals_c);
    return literals_s;
}


template<class T> inline std::vector<T>
SymmetryController<T>::adaptVector(const std::vector<Literal>& literals) {
    std::vector<T> adapted;
    for (const Literal& literal : literals)
        adapted.push_back(_literal_adapter->convertFrom(literal));

    return std::move(adapted);
}

template<class T> inline void
SymmetryController<T>::printStats() const {
    Printer::printSection(" Symmetry Stats ");
    _injector.printStats();
    if (_cosy_manager) {
        IF_STATS_ENABLED(_cosy_manager->printStats());
    }
}

template<class T> inline void
SymmetryController<T>::printInfo() const {
    _cnf_model.summarize();
    Printer::printSection(" Symmetry Information ");
    _symmetry_finder.printStats();
    _group.summarize(_num_vars);
    if (_cosy_manager)
        _cosy_manager->summarize();
}



}  // namespace cosy

#endif  // INCLUDE_COSY_SYMMETRYCONTROLLER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
