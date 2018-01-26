#include "early_update_perceptron.hpp"
#include "node.hpp"

namespace strpercpp {

void GreedyEarlyUpdate::_fit(std::vector<node_ptr>& nodes,
    std::vector<node_ptr>& true_path_) {

  for (int i=0; i < nodes.size(); ++i) {
    for (node_ptr n = nodes[i]; n != NULL; n = n.get()->bnext) {
      this->fire(n);
    }
  }

  for (int i=1; i < nodes.size()-1; ++i) {
    node_ptr best_n_curr = nodes[i];

    /* best one-step extention based on history */
    for (node_ptr curr_n = nodes[i]; curr_n != NULL; curr_n = curr_n->bnext) {
      node_ptr best_n_prev;
      float best_score;
      bool is_new = true;
      for (node_ptr prev_n = nodes[i-1]; prev_n != NULL; prev_n = prev_n->bnext) {
        float score = prev_n->path_score + curr_n->score;
        if (score > best_score || is_new) {
          best_score = score;
          best_n_prev = prev_n;
          is_new = false;
        }
      }
      curr_n->prev = best_n_prev;
      curr_n->path_score = best_score;
      if (curr_n->path_score > best_n_curr->path_score) {
        best_n_curr = curr_n;
      }
    }

    if (best_n_curr->Y != true_path_[i-1]->Y) {
      for (int j=0; j < i; ++j) {
        node_ptr n = true_path_[j];
        for (auto it = n.get()->feature_ids.begin(); it != n->feature_ids.end(); it++) {
          int fid = *it;
          this->w(n->Y, fid) += 1.;
        }
      }

      for (node_ptr n = best_n_curr; n != NULL; n = n->prev) {
        for (auto it = n.get()->feature_ids.begin(); it != n->feature_ids.end(); it++) {
          int fid = *it;
          this->w(n->Y, fid) -= 1.;
        }
      }
      break;
    }
  }
  nodes.clear();
};

} // namespace strpercpp
