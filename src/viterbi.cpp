#include <memory>
#include <vector>

#include "corpus.hpp"
#include "node.hpp"
#include "viterbi.hpp"

namespace strpercpp {

std::vector<node_ptr> viterbi(std::vector<node_ptr>& nodes) {
  node_ptr n(new Node());

  for (int i=0; i < nodes.size(); ++i) {
    for (node_ptr curr_n = nodes[i]; curr_n != nullptr; curr_n = curr_n->bnext) {
      node_ptr best_node;
      float best_score;
      bool is_new = true;
      for (node_ptr prev_n = n; prev_n != nullptr; prev_n = prev_n->bnext) {
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
    n = nodes[i];
  }

  node_ptr eos(new Node());

  node_ptr best_node;
  float best_score;
  bool is_new = true;
  for (node_ptr prev_n = n; prev_n != nullptr; prev_n = prev_n->bnext) {
    float score = prev_n->path_score + eos->score;
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

  eos->prev = best_node;
  eos->path_score = best_score;

  // backtrack
  int size_path = nodes.size();
  std::vector<node_ptr> path(size_path);

  node_ptr node = eos;
  node = node->prev;
  for (int i = size_path - 1; i >= 0; --i) {
    path[i] = node;
    node = node->prev;
  }
  return path;
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

//  int size_path = nodes.size() - 2;
  int size_path = nodes.size();
  std::vector<node_ptr> path(size_path);

  for (int k=0; k < true_label_ids.size(); ++k) {
//    int i = k + 1;
    bool found = false;
//    for (node_ptr n = nodes[i]; n != nullptr; n = n->bnext) {
    for (node_ptr n = nodes[k]; n != nullptr; n = n->bnext) {
//      if (n->Y == true_label_ids[i-1]) {
      if (n->Y == true_label_ids[k]) {
//        path[i-1] = n;
        path[k] = n;
        found = true;
      }
    }
    if (!found) {
//      std::cerr << "not found in candidates: " << true_label_ids[i-1] << std::endl;
      std::cerr << "not found in candidates: " << true_label_ids[k] << std::endl;
      std::cout << size_path << " " << true_label_ids.size() << std::endl;
      exit(1);
    }
  }
  return path;
};

} // namespace strpercpp
