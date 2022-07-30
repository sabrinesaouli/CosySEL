// Copyright 2017 Hakan Metin


#include "cosy/CNFReader.h"

namespace cosy {

CNFReader::CNFReader() {
}

CNFReader::~CNFReader() {
}

bool CNFReader::load(const std::string &filename, CNFModel *model) {
    StreamBuffer in(filename);
    unsigned int expected_num_vars = 0, expected_num_clauses = 0;
    int read_int;

    std::vector<Literal> literals;

    while (*in != '\0') {
        if (*in == 'c') {
            in.skipLine();
        } else if (*in == 'p') {
            ++in; CHECK_EQ(*in, ' ');
            ++in; CHECK_EQ(*in, 'c');
            ++in; CHECK_EQ(*in, 'n');
            ++in; CHECK_EQ(*in, 'f');
            ++in;

            expected_num_vars = in.readInt();
            expected_num_clauses = in.readInt();
            in.skipLine();
        } else {
            literals.clear();
            do {
                read_int = in.readInt();
                if (read_int != 0) {
                    Literal lit(read_int);
                    literals.push_back(lit);
                }
            } while (read_int != 0);
            model->addClause(&literals);
            in.skipLine();
        }
        in.skipWhiteSpaces();
    }

    if (model->numberOfVariables() != expected_num_vars) {
        LOG(ERROR) << "Expected " << expected_num_vars <<
            " variables: found " <<  model->numberOfVariables();
        return false;
    }
    if (model->numberOfClauses() != expected_num_clauses) {
        LOG(ERROR) << "Expected " << expected_num_clauses <<
            " clauses: found " <<  model->numberOfClauses();
        return false;
    }
    return true;
}

}  // namespace cosy

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
