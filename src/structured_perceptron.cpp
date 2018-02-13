#include <iostream>
#include <fstream>
#include <memory>

#include "corpus.hpp"
#include "lattice.hpp"
#include "matrix.hpp"
#include "node.hpp"
#include "structured_perceptron.hpp"
#include "viterbi.hpp"

namespace strpercpp {

StructuredPerceptron::StructuredPerceptron(Dictionary& _feature_dic, Dictionary& _label_dic):
  label_dic(_label_dic), feature_dic(_feature_dic), n_update_(0) {

  this->w = Matrix(label_dic.size(), feature_dic.size());
  w_trans_ = Matrix(label_dic.size(), label_dic.size());
  this->w_a_ = Matrix(label_dic.size(), feature_dic.size());
  w_trans_a_ = Matrix(label_dic.size(), label_dic.size());
};

void StructuredPerceptron::set_template(const std::vector<FeatureTemplate>& tmpl) {
  this->tmpl = tmpl;
}

void StructuredPerceptron::read_template(const char* filename) {
  this->tmpl = read_template_file(filename);
};

void StructuredPerceptron::fire(node_ptr node) {
  node->score = 0.;
  int num_features = node->feature_ids.size();
  int yid = node->Y;

  if (yid < label_dic.size()) {
    const std::vector<float>& w_y = this->w[yid];
    for (int i=0; i < num_features; ++i) {
      int fid = node->feature_ids[i];
      float w_f = w_y[fid];
      node->score += w_f;
    }
  }
};



void StructuredPerceptron::fit(const std::vector< std::vector<int> >& feature_ids,
    const std::vector<std::string>& labels) {

  std::vector<node_ptr> nodes(feature_ids.size());
  build_lattice(this->label_dic.size(), feature_ids, &nodes);
  this->fit(nodes, labels);
};

void StructuredPerceptron::fit(std::vector<node_ptr>& nodes,
    const std::vector<std::string>& labels) {

  std::vector<node_ptr> path = this->predict(nodes);

  std::vector<int> true_label_ids(labels.size());
  for (int i=0; i < labels.size(); ++i) {
    true_label_ids[i] = this->label_dic.geti(labels[i]);
  }
  std::vector<node_ptr> true_path_ = true_path(nodes, true_label_ids);
  this->_fit(nodes, true_path_);
};

void StructuredPerceptron::_fit(std::vector<node_ptr>& nodes,
    std::vector<node_ptr>& true_path_) {

  std::vector<node_ptr> path = this->predict(nodes);

  bool is_correct = true;
  for (int i = 0; i < path.size(); ++i) {
    if (path[i]->Y != true_path_[i]->Y) {
      is_correct = false;
      break;
    }
  }

  if (!is_correct) {
    n_update_ += 1;
    this->update(true_path_, path);
  }
  nodes.clear();
};


void StructuredPerceptron::update(const std::vector<node_ptr>& true_path,
    const std::vector<node_ptr>& pred_path) {

  for (int i=0; i < true_path.size(); ++i) {
    node_ptr n = true_path[i];
    for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
      int fid = *it;
      this->w(n->Y, fid) += 1.;
      this->w_a_(n->Y, fid) += n_update_;
    }
    if (i >= 1) {
      w_trans_(true_path[i-1]->Y, true_path[i]->Y) += 1.;
      w_trans_a_(true_path[i-1]->Y, true_path[i]->Y) += n_update_;
    }
  }

  for (int i=0; i < pred_path.size(); ++i) {
    node_ptr n = pred_path[i];
    for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
      int fid = *it;
      this->w(n->Y, fid) -= 1.;
      this->w_a_(n->Y, fid) -= n_update_;
    }
    if (i >= 1) {
      w_trans_(pred_path[i-1]->Y, pred_path[i]->Y) -= 1.;
      w_trans_a_(pred_path[i-1]->Y, pred_path[i]->Y) -= n_update_;
    }
  }
};

std::vector<node_ptr>
StructuredPerceptron::predict(const std::vector< std::vector<std::string> >& sequence) {

  bool train = false;
  std::vector< std::vector<int> > fids(sequence.size());
  for (int j=0; j < sequence.size(); ++j) {

    std::vector<int> features;
    extract_features(tmpl, &this->feature_dic, sequence, j, train, &features);
    fids[j] = features;
  }

  std::vector<node_ptr> path = this->predict(fids);
  return path;
};

std::vector<node_ptr>
StructuredPerceptron::predict(const std::vector< std::vector<int> >& feature_ids) {

  std::vector<node_ptr> nodes(feature_ids.size());
  build_lattice(this->label_dic.size(), feature_ids, &nodes);
  std::vector<node_ptr> path = this->predict(nodes);
  return path;
};

std::vector<node_ptr>
StructuredPerceptron::predict(std::vector<node_ptr>& nodes) {
  for (int i=0; i < nodes.size(); ++i) {
    for (node_ptr n = nodes[i]; n != nullptr; n = n->bnext) {
      this->fire(n);
    }
  }

//  std::vector<node_ptr> path = viterbi(nodes);
  node_ptr n(new Node());

  for (int i=0; i < nodes.size(); ++i) {
    for (node_ptr curr_n = nodes[i]; curr_n != nullptr; curr_n = curr_n->bnext) {
      node_ptr best_node;
      float best_score;
      bool is_new = true;
      for (node_ptr prev_n = n; prev_n != nullptr; prev_n = prev_n->bnext) {
        float score = prev_n->path_score + curr_n->score;
        if (i > 0) {
          if (prev_n->Y < label_dic.size() && curr_n->Y < label_dic.size()) {
            score += w_trans_[prev_n->Y][curr_n->Y];
          }
        }

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


std::vector< std::vector<node_ptr> >
StructuredPerceptron::nbest(std::vector<node_ptr>& nodes, int beam_width) {
  for (int i=0; i < nodes.size(); ++i) {
    for (node_ptr n = nodes[i]; n != nullptr; n = n->bnext) {
      this->fire(n);
    }
  }

  std::vector<node_ptr> nbest = beamsearch(nodes, beam_width);

  std::vector< std::vector<node_ptr> > ret(beam_width);
  for (int i=0; i < beam_width; ++i) {
    std::vector<node_ptr> b(nodes.size());
    node_ptr n = nbest[i];
    int t = 0;
    for (int t=nodes.size()-1; t >= 0; --t) {
      b[t] = n;
      n = n->prev;
    }
    ret[i] = b;
  }

  return ret;
};


void StructuredPerceptron::save(const char* filename) {
    FILE* fp = fopen(filename, "wb");

    this->label_dic.save(fp);
    this->feature_dic.save(fp);

    int s_tmpl = this->tmpl.size();
    fwrite(&s_tmpl, sizeof(int), 1, fp);
    for (int i=0; i < this->tmpl.size(); ++i) {
        this->tmpl[i].save(fp);
    }

    for (int i=0; i < w_a_.row(); ++i) {
      for (int j=0; j < w_a_.col(); ++j) {
        w_a_(i, j) = w(i, j) - w_a_(i, j) / n_update_;
      }
    }
    w_a_.save(fp);

    for (int i=0; i < w_trans_a_.row(); ++i) {
      for (int j=0; j < w_trans_a_.col(); ++j) {
        w_trans_a_(i, j) = w_trans_(i, j) - w_trans_a_(i, j) / n_update_;
      }
    }

//    w_trans_.save(fp);
    w_trans_a_.save(fp);

//    this->w.save(fp);
    fclose(fp);
}

void StructuredPerceptron::load(const char* filename) {
    FILE* fp = fopen(filename, "rb");

    this->label_dic.load(fp);
    this->feature_dic.load(fp);

    int s_tmpl = 0;
    fread(&s_tmpl, sizeof(int), 1, fp);
    for (int i=0; i < s_tmpl; ++i) {
        FeatureTemplate ft;
        ft.load(fp);
        this->tmpl.push_back(ft);
    }

    this->w.load(fp);
    w_trans_.load(fp);
    fclose(fp);
}

void StructuredPerceptron::print_w() {
    for (int i=0; i < this->feature_dic.size(); ++i) {
        for (int j=0; j < this->label_dic.size(); ++j) {
        }
    }
};

} // namespace strpercpp
