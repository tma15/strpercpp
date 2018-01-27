#include <memory>
#include <vector>

#include "corpus.hpp"
#include "node.hpp"
#include "viterbi.hpp"

namespace strpercpp {

void viterbi(std::vector<node_ptr>& nodes) {
  for (int i=1; i < nodes.size(); ++i) {
    for (node_ptr curr_n = nodes[i]; curr_n != NULL; curr_n = curr_n->bnext) {
      node_ptr best_node;
      float best_score;
      bool is_new = true;
      for (node_ptr prev_n = nodes[i-1]; prev_n != NULL; prev_n = prev_n->bnext) {
        float score = prev_n->path_score + curr_n->score;
        if (score > best_score || is_new) {
          best_score = score;
          best_node = prev_n;
          is_new = false;
        }
      }

      if (is_new) {
        std::cerr << "best node is not selected." << std::endl;
        exit(1);
      }

      curr_n->prev = best_node;
      curr_n->path_score = best_score;
    }
//    printf("%d/%d\n", i, nodes.size());
  }
};


std::vector<node_ptr> backtrack(std::vector<node_ptr>& nodes) {
  if (nodes.size() == 0) {
    std::cerr << "nodes.size()==0" << std::endl;
    exit(1);
  }

  int size_path = nodes.size() - 2;
  std::vector< node_ptr > path(size_path);

  node_ptr node = nodes[nodes.size()-1];
  for (int i = size_path - 1; i >= 0; --i) {
    node = node->prev;
    path[i] = node;
  }
  return path;
};

std::vector<node_ptr>
true_path(std::vector<node_ptr>& nodes,
    const std::vector<int>& true_label_ids) {

  int size_path = nodes.size() - 2;
  std::vector< node_ptr > path(size_path);

  for (int k=0; k < true_label_ids.size(); ++k) {
    int i = k + 1;
    bool found = false;
    for (node_ptr n = nodes[i]; n != NULL; n = n->bnext) {
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
  }
  return path;
};

} // namespace strpercpp
