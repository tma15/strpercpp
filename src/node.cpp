//#include <unordered_set>

#include "node.hpp"

namespace strpercpp {
Node::Node() {
//  this->Y = 0;
  this->Y = -1;
  this->label = "";
  this->score = 0.;
  this->path_score = 0.;
};

} // namespace strpercpp
