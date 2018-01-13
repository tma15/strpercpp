/// \file node.hpp
/// \author Takuya Makino
/// \date 2018/01/13
#ifndef STRPERCPP_NODE_H
#define STRPERCPP_NODE_H

#include <iostream>
#include <unordered_set>
#include <memory>
#include <vector>

namespace strpercpp {
  
/*! @class Node
 *  @brief Node represents a node in lattice.
 */
class Node {
  public:
    /*! feature indice on the node */
    std::vector<int> feature_ids;

    /*! label index */
    int Y;

    /*! label */
    std::string label;

    /*! the pointer to previous node */
    std::shared_ptr<Node> prev;

    /*! the pointer to next node */
    std::shared_ptr<Node> bnext;

    /*! the score of the node */
    float score;

    /*! the cumulative score of the node from the start of lattice */
    float path_score;

    Node();
    ~Node(){};
};

} // namespace strpercpp

#endif
