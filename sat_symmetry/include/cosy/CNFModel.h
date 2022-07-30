// Copyright 2017 Hakan Metin


#ifndef INCLUDE_COSY_CNFMODEL_H_
#define INCLUDE_COSY_CNFMODEL_H_

#include <algorithm>
#include <memory>
#include <unordered_set>
#include <vector>

#include "cosy/Clause.h"
#include "cosy/Literal.h"
#include "cosy/Logging.h"

namespace cosy {

class CNFModel {
 public:
    CNFModel();
    ~CNFModel();

    void addClause(std::vector<Literal>* literals);

    const std::vector<std::unique_ptr<Clause>>& clauses() const {
        return _clauses;
    }

    int64 numberOfVariables()      const { return _num_variables + 1; }
    int64 numberOfClauses()        const { return _num_clauses;       }

    int64 numberOfUnaryClauses()   const { return _num_unary_clauses;   }
    int64 numberOfBinaryClauses()  const { return _num_binary_clauses;  }
    int64 numberOfTernaryClauses() const { return _num_ternary_clauses; }
    int64 numberOfLargeClauses()   const { return _num_large_clauses;   }

    const std::vector<int64>& occurences() const { return _occurences; }

    void summarize() const;

 private:
    int64 _num_variables;
    int64 _num_clauses;

    int64 _num_unary_clauses;
    int64 _num_binary_clauses;
    int64 _num_ternary_clauses;
    int64 _num_large_clauses;

    std::vector<std::unique_ptr<Clause>> _clauses;
    std::unordered_set<size_t> _clauses_tag;

    std::vector<int64> _positive_occurences;
    std::vector<int64> _negative_occurences;
    std::vector<int64> _occurences;

    size_t compute_tag(const std::vector<Literal>& literals);
    void compute_occurences(const std::vector<Literal>& literals);
    void compute_sizes(const std::vector<Literal>& literals);

    DISALLOW_COPY_AND_ASSIGN(CNFModel);
};
}  // namespace cosy

#endif  // INCLUDE_COSY_CNFMODEL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
