#include <memory>
#include <vector>

#include "corpus.hpp"
#include "node.hpp"
#include "viterbi.hpp"

void viterbi(std::vector< std::shared_ptr<Node> >& nodes) {
    for (int i=1; i < nodes.size(); ++i) {
        for (std::shared_ptr<Node> curr_node = nodes[i]; curr_node != NULL; curr_node = curr_node->bnext) {
            std::shared_ptr<Node> best_node;
            float best_score;
            bool is_new = true;
            for (std::shared_ptr<Node> prev_node = nodes[i-1]; prev_node != NULL; prev_node = prev_node->bnext) {
                float score = prev_node->path_score + curr_node->score;
                if (score > best_score || is_new) {
                    best_score = score;
                    best_node = prev_node;
                    is_new = false;
                }
            }

            if (is_new) {
                std::cerr << "best node is not selected." << std::endl;
                exit(1);
            }

            curr_node->prev = best_node;
//            curr_node->path_score += best_score;
            curr_node->path_score = best_score;
        }
    }
};


std::vector< std::shared_ptr< Node > > backtrack(std::vector< std::shared_ptr< Node > >& nodes) {
    if (nodes.size() == 0) {
        std::cerr << "nodes.size()==0" << std::endl;
        exit(1);
    }

    int size_path = nodes.size() - 2;
//    std::cout << size_path << std::endl;
    std::vector< std::shared_ptr<Node> > path(size_path);

    std::shared_ptr<Node> node = nodes[nodes.size()-1];
    for (int i = size_path - 1; i >= 0; --i) {
        node = node.get()->prev;
        path[i] = node;
    }
    return path;
};

std::vector< std::shared_ptr< Node > > true_path(
        std::vector< std::shared_ptr< Node > >& nodes,
        const std::vector<int>& true_label_ids) {

    int size_path = nodes.size() - 2;
    std::vector< std::shared_ptr< Node > > path(size_path);

    for (int k=0; k < true_label_ids.size(); ++k) {
        int i = k + 1;
        bool found = false;
        for (std::shared_ptr<Node> n = nodes[i]; n != NULL; n = n->bnext) {
            if (n.get()->Y == true_label_ids[i-1]) {
                path[i-1] = n;
                found = true;
            }
        }
        if (!found) {
            std::cerr << "not found in candidates: " << true_label_ids[i-1] << std::endl;
            std::cout << size_path << " " << true_label_ids.size() << std::endl;
            exit(1);
        }
//        std::cout << std::endl;
    }
    return path;
};

