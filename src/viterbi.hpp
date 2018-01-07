#ifndef STRPERCPP_VITERBI_H
#define STRPERCPP_VITERBI_H
#include <memory>
#include <vector>

#include "node.hpp"

namespace strpercpp {
    

void viterbi(std::vector< std::shared_ptr<Node> >& nodes);

std::vector< std::shared_ptr<Node> > backtrack(std::vector< std::shared_ptr< Node > >& nodes);

std::vector< std::shared_ptr< Node > > true_path(
        std::vector< std::shared_ptr< Node > >& nodes,
        const std::vector<int>& true_label_ids);

} // namespace strpercpp

#endif
