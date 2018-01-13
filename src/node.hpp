#ifndef STRPERCPP_NODE_H
#define STRPERCPP_NODE_H

#include <iostream>
#include <unordered_set>
#include <memory>
#include <vector>

namespace strpercpp {
  
class Node {
  public:
    std::vector<int> feature_ids;
    int Y;
    std::string label;

    std::shared_ptr<Node> prev;
    std::shared_ptr<Node> bnext;

    float score;
    float path_score;

    Node();
    ~Node(){};
};

} // namespace strpercpp

#endif
