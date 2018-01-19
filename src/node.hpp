/// \file node.hpp
/// \author Takuya Makino
/// \date 2018/01/13
#ifndef STRPERCPP_NODE_H
#define STRPERCPP_NODE_H

#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>
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

    /*! the pointer to previous node on the previous time step */
    std::shared_ptr<Node> prev;

    /*! the pointer to next node in the same time step */
    std::shared_ptr<Node> bnext;

    /*! the score of the node */
    float score;

    /*! the cumulative score of the node from the start of lattice */
    float path_score;

    Node();
    ~Node(){};
};

typedef std::shared_ptr<Node> node_ptr;

class NodeCompare {
  public:
    bool operator() (node_ptr a, node_ptr b) {
//      printf("   a:%f b:%f\n", a->path_score, b->path_score);
      return a->path_score > b->path_score;
//      return a->path_score < b->path_score;
    }
};

typedef std::priority_queue<node_ptr, std::vector<node_ptr>, NodeCompare> node_ptr_queue;

} // namespace strpercpp

#endif
