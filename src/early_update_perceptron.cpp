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

  for (int i=1; i < nodes.size()-1; ++i) {
    node_ptr best_n_curr = nodes[i];

    /* best one-step extention based on history */
    for (node_ptr curr_n = nodes[i]; curr_n != nullptr; curr_n = curr_n->bnext) {
      node_ptr best_n_prev;
      float best_score;
      bool is_first = true;
      for (node_ptr prev_n = nodes[i-1]; prev_n != nullptr; prev_n = prev_n->bnext) {
        float score = prev_n->path_score + curr_n->score;
        if (score > best_score || is_first) {
          best_score = score;
          best_n_prev = prev_n;
          is_first = false;
        }
      }
      curr_n->prev = best_n_prev;
      curr_n->path_score = best_score;
      if (curr_n->path_score > best_n_curr->path_score) {
        best_n_curr = curr_n;
      }
    }

    if (best_n_curr->Y != true_path[i-1]->Y) {
      for (int j=0; j < i; ++j) {
        node_ptr n = true_path[j];
        for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
          int fid = *it;
          this->w(n->Y, fid) += 1.;
        }
      }

      for (node_ptr n = best_n_curr; n != nullptr; n = n->prev) {
        for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
          int fid = *it;
          this->w(n->Y, fid) -= 1.;
        }
      }
      break;
    }
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
  pq.push(nodes[0]);

  int len_seq = nodes.size();

  if (true_path.size()==1) { return; }

  for (int t=1; t < nodes.size(); ++t) {
    node_ptr_queue next_pq_tmp;
    node_ptr_queue next_pq;
    while (!pq.empty()) {
      node_ptr node = pq.top();
      pq.pop();

      for (node_ptr n = nodes[t]; n != nullptr; n = n->bnext) {
        node_ptr n_curr = std::make_shared<Node>(*node);
        node_ptr n_ = std::make_shared<Node>(*n);
        n_->prev = n_curr;
        n_->path_score = n_curr->path_score + n_->score;
        next_pq_tmp.push(n_);
      }
    }

    bool true_exists = false;
    node_ptr n_max = next_pq_tmp.top();
    next_pq_tmp.pop();
    next_pq.push(n_max);

//    std::vector< std::vector<int> > feature_ids_n_max;
    std::vector<int> ys_max;
//    int offset = 0;
    node_ptr n_max_ = std::make_shared<Node>(*n_max);
    if (t == nodes.size()-1) { // EOS
//      ys_max.push_back(n_max->Y);
//      feature_ids_n_max.push_back(n_max->feature_ids);
      n_max_ = n_max_->prev;
//      n_max = n_max->prev;
//      offset += 1;
    }
    while (n_max_->prev != nullptr) {
      ys_max.push_back(n_max_->Y);
      n_max_ = n_max_->prev;
//    while (n_max->prev != nullptr) {
//      ys_max.push_back(n_max->Y);
//      feature_ids_n_max.push_back(n_max->feature_ids);
//      n_max = n_max->prev;
    }
//    std::reverse(ys_max.begin(), ys_max.end());

    bool is_true = true;
//    for (int k=0; k < ys_max.size()+offset; ++k) {
    for (int k=0; k < ys_max.size(); ++k) {
//      printf("%d/%d:%d(%s) %d/%d:%d(%s)\n", ys_max.size()-k-1,
//          ys_max.size(), ys_max[ys_max.size()-k-1],
//          label_dic.gets(ys_max[ys_max.size()-k-1]).c_str(),
//          k, true_path.size(), true_path[k]->Y,
//          label_dic.gets(true_path[k]->Y).c_str());
      if (ys_max[ys_max.size()-k-1] != true_path[k]->Y) {
//      if (ys_max[ys_max.size()-1-k-1] != true_path[k]->Y) {
//      if (ys_max[k] != true_path[k]->Y) {
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
      if (t == nodes.size()-1) { // EOS
        n = n->prev;
      }
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
//      printf("is true[b=%d]:%d\n", is_true);
    }
//    printf("--\n");

    if (!true_exists) {
//      printf("nodes:%d true_path:%d\n", nodes.size(), true_path.size());
      for (int j=0; j < t; ++j) {
//        printf("j:%d/%d\n", j, true_path.size());
        node_ptr n = true_path[j];
        for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
          int fid = *it;
          this->w(n->Y, fid) += 1.;
//          printf("true: %d %d %f\n", n->Y, fid, w(n->Y, fid));
        }
      }

      for (node_ptr n = n_max; n != nullptr; n = n->prev) {
        for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
//      for (int j=0; j < feature_ids_n_max.size(); ++j) {
//        for (int k = 0; k < feature_ids_n_max[j].size(); ++k) {
          int fid = *it;
          this->w(n->Y, fid) -= 1.;
//          int fid = feature_ids_n_max[j][k];
//          this->w(ys_max[j], fid) += 1.;
//          printf("max: %d %d %f\n", n->Y, fid, w(n->Y, fid));
        }
      }
      break;
    }
//    printf("%d\n", next_pq.size());
    pq = next_pq;
  }


};

void MaxViolationUpdate::_fit(std::vector<node_ptr>& nodes, std::vector<node_ptr>& true_path) {
  node_ptr_queue pq;
  pq.push(nodes[0]);

  int len_seq = nodes.size();

  for (int t=1; t < nodes.size(); ++t) {

    node_ptr_queue next_pq;
    while (!pq.empty()) {
      node_ptr node = pq.top();
      pq.pop();

      for (node_ptr n = nodes[t]; n != nullptr; n = n->bnext) {
        node_ptr n_curr = std::make_shared<Node>(*node);
        node_ptr n_ = std::make_shared<Node>(*n);
        n_->prev = n_curr;
        n_->path_score = n_curr->path_score + n_->score;
        next_pq.push(n_);
      }
    }

    while (next_pq.size() > beam_width_) {
      node_ptr n = next_pq.top();
      next_pq.pop();

      /* if falls off beam, then update paramter */
      std::vector<int> ys;
      while (n->prev != nullptr) {
        ys.push_back(n->Y);
        n = n->prev;
      }

      bool is_true = true;
      for (int t=0; t < ys.size(); ++t) {
        if (ys[ys.size()-t-1] != true_path[t]->Y) {
          is_true = false;
        }
      }

      if (is_true) {
        for (int j=0; j < t; ++j) {
          node_ptr n = true_path[j];
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

        for (node_ptr n = n_max; n != nullptr; n = n->prev) {
          for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
            int fid = *it;
            this->w(n->Y, fid) -= 1.;
          }
        }
        break;
      }
    }
    pq = next_pq;
  }
};



} // namespace strpercpp
