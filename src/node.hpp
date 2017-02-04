#ifndef STRPERCPP_NODE_H
#define STRPERCPP_NODE_H

#include <iostream>
#include <unordered_set>
#include <memory>
#include <vector>

class Node {
    public:

//        std::vector<std::string> features;
        std::vector<int> feature_ids;
//        std::unordered_set<int> fids;
        int Y;
        std::string label;

//        Node* prev;
        std::shared_ptr<Node> prev;
//        Node* next;

//        Node* bnext;
        std::shared_ptr<Node> bnext;
//        Node* enext;

        float score;
        float path_score;

        Node();
        ~Node(){
//            std::cout << "Free " << Y << std::endl;
        };

//        void add_bnext(Node& node);
};


#endif
