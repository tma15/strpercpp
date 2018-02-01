#ifndef STRPERCPP_LATTICE_H
#define STRPERCPP_LATTICE_H

#include <memory>

#include "node.hpp"

namespace strpercpp {

//std::vector<node_ptr>
//build_lattice(int label_size, const std::vector< std::vector<int> >& feature_ids);
void build_lattice(int label_size,
    const std::vector< std::vector<int> >& feature_ids,
    std::vector<node_ptr>* nodes);

void print_label_seq(node_ptr n);

std::vector<node_ptr> beamsearch(std::vector<node_ptr>& nodes, int beam_width);

} // namespace strpercpp

#endif
