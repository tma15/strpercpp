#include "early_update_perceptron.hpp"
#include "lattice.hpp"
#include "node.hpp"

namespace strpercpp {

void GreedyEarlyUpdate::_fit(std::vector<node_ptr>& nodes,
    std::vector<node_ptr>& true_path) {

  for (int i=0; i < nodes.size(); ++i) {
    for (node_ptr n = nodes[i]; n != nullptr; n = n->bnext) {
      this->fire(n);
    }
  }

  node_ptr n(new Node());

  for (int i=0; i < nodes.size(); ++i) {
    node_ptr best_n_curr = nullptr;

    /* best one-step extention based on history */
    for (node_ptr curr_n = nodes[i]; curr_n != nullptr; curr_n = curr_n->bnext) {
      node_ptr best_n_prev;
      float best_score;
      bool is_first = true;
      for (node_ptr prev_n = n; prev_n != nullptr; prev_n = prev_n->bnext) {
        float score = prev_n->path_score + curr_n->score;
        if (score > best_score || is_first) {
          best_score = score;
          best_n_prev = prev_n;
          is_first = false;
        }
      }
      curr_n->prev = best_n_prev;
      curr_n->path_score = best_score;
      if (best_n_curr == nullptr) {
        best_n_curr = curr_n;
      } else if (curr_n->path_score > best_n_curr->path_score) {
        best_n_curr = curr_n;
      }
    }

//    std::cout << "i:" << i << " best:" << best_n_curr->Y \
//              << " score:" << best_n_curr->path_score \
//              << " true:" << true_path[i]->Y << std::endl;

    if (best_n_curr->Y != true_path[i]->Y) {
      for (int j=0; j <= i; ++j) {
        node_ptr node = true_path[j];
        for (auto it = node->feature_ids.begin(); it != node->feature_ids.end(); it++) {
          int fid = *it;
          this->w(node->Y, fid) += 1.;
        }
      }

      for (node_ptr node = best_n_curr; node != nullptr; node = node->prev) {
        for (auto it = node->feature_ids.begin(); it != node->feature_ids.end(); it++) {
          int fid = *it;
          this->w(node->Y, fid) -= 1.;
        }
      }
      break;
    }
    n = nodes[i];
  }
  nodes.clear();
};


void BeamEarlyUpdate::_fit(std::vector<node_ptr>& nodes, std::vector<node_ptr>& true_path) {
  for (int i=0; i < nodes.size(); ++i) {
    for (node_ptr n = nodes[i]; n != nullptr; n = n->bnext) {
      this->fire(n);
    }
  }

  node_ptr_queue pq;
  node_ptr n(new Node()); // BOS
  pq.push(n);

  for (int t=0; t < nodes.size(); ++t) {
    node_ptr_queue next_pq_tmp;
    node_ptr_queue next_pq;
    while (!pq.empty()) {
      node_ptr node = pq.top();
      pq.pop();

      for (node_ptr n = nodes[t]; n != nullptr; n = n->bnext) {
        node_ptr n_ = std::make_shared<Node>(*n);
        n_->prev = node;
        n_->path_score = node->path_score + n_->score;
        next_pq_tmp.push(n_);
      }
    }

    bool true_exists = false;
    node_ptr n_max = next_pq_tmp.top();
    next_pq_tmp.pop();
    next_pq.push(n_max);

    std::vector< std::vector<int> > feature_ids_n_max;
    std::vector<int> ys_max;
    while (n_max->prev != nullptr) {
      ys_max.push_back(n_max->Y);
      feature_ids_n_max.push_back(n_max->feature_ids);
      n_max = n_max->prev;
    }

    bool is_true = true;
    for (int k=0; k < ys_max.size(); ++k) {
//      printf("%d/%d:%d(%s) %d/%d:%d(%s)\n", ys_max.size()-k-1,
//          ys_max.size(), ys_max[ys_max.size()-k-1],
//          label_dic.gets(ys_max[ys_max.size()-k-1]).c_str(),
//          k, true_path.size(), true_path[k]->Y,
//          label_dic.gets(true_path[k]->Y).c_str());
      if (ys_max[ys_max.size()-k-1] != true_path[k]->Y) {
        is_true = false;
      }
    }
//    printf("====\n");

    if (is_true) {
      true_exists = true;
    }

    while (next_pq.size() < beam_width_-1 && !next_pq_tmp.empty()) {
      /* if true path does not exist in beam, then update */
      node_ptr n = next_pq_tmp.top();
      next_pq_tmp.pop();
      next_pq.push(n);

      std::vector<int> ys;
      while (n->prev != nullptr) {
        ys.push_back(n->Y);
        n = n->prev;
      }

      bool is_true = true;
      for (int k=0; k < ys.size(); ++k) {
//        printf("%d/%d:%d %d/%d:%d\n", ys.size()-k-1, ys.size(), ys[ys.size()-k-1],
//            k, true_path.size(), true_path[k]->Y);
        if (ys[ys.size()-k-1] != true_path[k]->Y) {
          is_true = false;
        }
      }

      if (is_true) {
        true_exists = true;
      }
    }

    if (!true_exists) {
      for (int j=0; j <= t; ++j) {
        node_ptr n = true_path[j];
        for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
          int fid = *it;
          this->w(n->Y, fid) += 1.;
        }
      }

      for (int j=0; j < feature_ids_n_max.size(); ++j) {
        for (int k = 0; k < feature_ids_n_max[j].size(); ++k) {
          int fid = feature_ids_n_max[j][k];
          this->w(ys_max[j], fid) -= 1.;
        }
      }
      break;
    }
    pq = next_pq;
  }
};

void MaxViolationUpdate::_fit(std::vector<node_ptr>& nodes, std::vector<node_ptr>& true_path) {
  for (int i=0; i < nodes.size(); ++i) {
    for (node_ptr n = nodes[i]; n != nullptr; n = n->bnext) {
      fire(n);
    }
  }

  for (int i=0; i < true_path.size(); ++i) {
    fire(true_path[i]);
  }
  for (int i=1; i < true_path.size(); ++i) {
    true_path[i]->path_score = true_path[i-1]->path_score + true_path[i]->score;
  }

  node_ptr_queue pq;
  node_ptr n(new Node()); // BOS
  pq.push(n);

  std::vector<node_ptr> n_max_list;

  for (int t=0; t < nodes.size(); ++t) {
    node_ptr_queue next_pq_tmp;
    node_ptr_queue next_pq;
    while (!pq.empty()) {
      node_ptr node = pq.top();
      pq.pop();

      for (node_ptr n = nodes[t]; n != nullptr; n = n->bnext) {
        node_ptr n_ = std::make_shared<Node>(*n);
        n_->prev = node;
        n_->path_score = node->path_score + n_->score;
        next_pq_tmp.push(n_);
      }
    }

    bool true_exists = false;
    node_ptr n_max = next_pq_tmp.top();
    next_pq_tmp.pop();
    next_pq.push(n_max);
    n_max_list.push_back(n_max);

    std::vector< std::vector<int> > feature_ids_n_max;
    std::vector<int> ys_max;
    while (n_max->prev != nullptr) {
      ys_max.push_back(n_max->Y);
      feature_ids_n_max.push_back(n_max->feature_ids);
      n_max = n_max->prev;
    }

    bool is_true = true;
    for (int k=0; k < ys_max.size(); ++k) {
      if (ys_max[ys_max.size()-k-1] != true_path[k]->Y) {
        is_true = false;
      }
    }

    if (is_true) {
      true_exists = true;
    }

    while (next_pq.size() < beam_width_-1 && !next_pq_tmp.empty()) {
      /* if true path does not exist in beam, then update */
      node_ptr n = next_pq_tmp.top();
      next_pq_tmp.pop();
      next_pq.push(n);

      std::vector<int> ys;
      while (n->prev != nullptr) {
        ys.push_back(n->Y);
        n = n->prev;
      }

      bool is_true = true;
      for (int k=0; k < ys.size(); ++k) {
        if (ys[ys.size()-k-1] != true_path[k]->Y) {
          is_true = false;
        }
      }

      if (is_true) {
        true_exists = true;
      }
    }

//    printf("t:%d %d\n", t, n_max_list.size());

    if (!true_exists) {
      int argmin = 0;
      float min = true_path[argmin]->path_score - n_max_list[argmin]->path_score;
      for (int j=1; j < n_max_list.size(); ++j) {
        node_ptr z = n_max_list[j];
        node_ptr y = true_path[j];
        float diff = y->path_score - z->path_score;
        if (diff < min) {
          argmin = j;
          min = diff;
        }
      }

//      printf("t:%d argmin:%d\n", t, argmin);

      for (int j=0; j <= argmin; ++j) {
        node_ptr n = true_path[j];
        for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
          int fid = *it;
          this->w(n->Y, fid) += 1.;
        }
      }

      for (node_ptr n = n_max_list[argmin]; n->prev != nullptr; n = n->prev) {
        for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
          int fid = *it;
          this->w(n->Y, fid) -= 1.;
        }
      }

      break;
    }

    pq = next_pq;
  }
};



} // namespace strpercpp
