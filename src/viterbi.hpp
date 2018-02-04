#ifndef STRPERCPP_VITERBI_H
#define STRPERCPP_VITERBI_H
#include <memory>
#include <vector>

#include "node.hpp"

namespace strpercpp {
    

std::vector<node_ptr> viterbi(std::vector<node_ptr>& nodes);

std::vector<node_ptr> backtrack(std::vector<node_ptr>& nodes);

std::vector<node_ptr>
true_path(std::vector<node_ptr>& nodes, const std::vector<int>& true_label_ids);


} // namespace strpercpp

#endif
