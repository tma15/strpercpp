//#include <unordered_set>

#include "node.hpp"

Node::Node() {
    this->Y = 0;
//    this->bnext = nullptr;
//    this->bnext = NULL;
//    this->enext = NULL;
    this->label = "";

//    this->fids = std::unordered_set<int>();

//    this->prev = nullptr;
//    this->prev = NULL;
//    this->next = NULL;

    this->score = 0.;
    this->path_score = 0.;
};

//void Node::add_bnext(Node& node) {
//    this->bnext = &node;
//};

