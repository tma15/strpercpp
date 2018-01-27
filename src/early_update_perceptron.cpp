#include "early_update_perceptron.hpp"
#include "lattice.hpp"
#include "node.hpp"

namespace strpercpp {

void GreedyEarlyUpdate::_fit(std::vector<node_ptr>& nodes,
    std::vector<node_ptr>& true_path_) {

  for (int i=0; i < nodes.size(); ++i) {
    for (node_ptr n = nodes[i]; n != NULL; n = n->bnext) {
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
        for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
          int fid = *it;
          this->w(n->Y, fid) += 1.;
        }
      }

      for (node_ptr n = best_n_curr; n != NULL; n = n->prev) {
        for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
          int fid = *it;
          this->w(n->Y, fid) -= 1.;
        }
      }
      break;
    }
//    printf("t:%d/%d\n", i, nodes.size()-1);
  }
  nodes.clear();
};


void BeamEarlyUpdate::_fit(std::vector<node_ptr>& nodes, std::vector<node_ptr>& true_path_) {
  for (int i=0; i < nodes.size(); ++i) {
    for (node_ptr n = nodes[i]; n != NULL; n = n->bnext) {
      this->fire(n);
    }
  }

  node_ptr_queue pq;
  pq.push(nodes[0]);

  int len_seq = nodes.size();

  for (int t=1; t < nodes.size(); ++t) {
//    printf("t=%d\n", t);

    node_ptr_queue next_pq;
    while (!pq.empty()) {
      node_ptr node = pq.top();
      pq.pop();

      for (node_ptr n = nodes[t]; n != NULL; n = n->bnext) {
        node_ptr n_curr = std::make_shared<Node>(*node);
        node_ptr n_ = std::make_shared<Node>(*n);
        n_->prev = n_curr;
        n_->path_score = n_curr->path_score + n_->score;
        next_pq.push(n_);
      }
    }

    bool updated = false;
    while (next_pq.size() > beam_width_) {
      node_ptr n = next_pq.top();
      next_pq.pop();

      /* if falls off beam, then update paramter */
      std::vector<int> ys;
      while (n->prev != NULL) {
        ys.push_back(n->Y);
        n = n->prev;
      }

//      for (int t=0; t < ys.size(); ++t) {
//        std::cout << ys[t] << " ";
//      }
//      std::cout << std::endl;

      bool is_true = true;
      for (int t=0; t < ys.size(); ++t) {
        if (ys[ys.size()-t-1] != true_path_[t]->Y) {
          is_true = false;
        }
      }

      if (is_true) {
        updated = true;
//        printf("out of beam t=%d\n", t);
        for (int j=0; j < t; ++j) {
          node_ptr n = true_path_[j];
          for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
            int fid = *it;
            this->w(n->Y, fid) += 1.;
          }
        }

        node_ptr n_max;
        while (!next_pq.empty()) {
          n_max = next_pq.top();
          next_pq.pop();
        }

        for (node_ptr n = n_max; n != NULL; n = n->prev) {
          for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
            int fid = *it;
            this->w(n->Y, fid) -= 1.;
          }
        }
      }
    }
    if (updated) {
      break;
    }
    pq = next_pq;
  }

  nodes.clear();
};


} // namespace strpercpp
