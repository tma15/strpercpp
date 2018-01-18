#ifndef STRPERCPP_LATTICE_H
#define STRPERCPP_LATTICE_H

#include <memory>

#include "node.hpp"

namespace strpercpp {

std::vector<node_ptr>
build_lattice(int label_size, const std::vector< std::vector< int > >& feature_ids);

} // namespace strpercpp

#endif
