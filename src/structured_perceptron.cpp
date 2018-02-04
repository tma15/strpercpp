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
    

StructuredPerceptron::StructuredPerceptron() {
};

StructuredPerceptron::StructuredPerceptron(Dictionary& _feature_dic, Dictionary& _label_dic):
  label_dic(_label_dic), feature_dic(_feature_dic) {
  this->w = Matrix(label_dic.size(), feature_dic.size());
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



void StructuredPerceptron::fit(const std::vector< std::vector< int > >& feature_ids,
    const std::vector<std::string>& labels) {

  std::vector<node_ptr> nodes = build_lattice(this->label_dic.size(), feature_ids);
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
    this->update(true_path_, path);
  }
  nodes.clear();
};


void StructuredPerceptron::update(const std::vector< node_ptr >& true_path,
        const std::vector< node_ptr >& pred_path) {

  for (int i=0; i < true_path.size(); ++i) {
    node_ptr n = true_path[i];
    for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
      int fid = *it;
      this->w(n->Y, fid) += 1.;
    }
  }

  for (int i=0; i < pred_path.size(); ++i) {
    node_ptr n = pred_path[i];
    for (auto it = n->feature_ids.begin(); it != n->feature_ids.end(); it++) {
      int fid = *it;
      this->w(n->Y, fid) -= 1.;
    }
  }
};

std::vector<node_ptr> StructuredPerceptron::predict(
        const std::vector< std::vector<std::string> >& sequence) {

  bool train = false;
  std::vector< std::vector<int> > fids;
  for (int j=0; j < sequence.size(); ++j) {

    std::vector<int> features = extract_features(
          tmpl,
          &this->feature_dic,
          sequence,
          j,
          train);
    fids.push_back(features);
  }

  std::vector<node_ptr> path = this->predict(fids);
  return path;
};

std::vector<node_ptr> StructuredPerceptron::predict(
        const std::vector< std::vector<int> >& feature_ids) {

  std::vector<node_ptr> nodes = build_lattice(this->label_dic.size(), feature_ids);
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

  std::vector<node_ptr> path = viterbi(nodes);
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

    this->w.save(fp);
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
    fclose(fp);
}

void StructuredPerceptron::print_w() {
    for (int i=0; i < this->feature_dic.size(); ++i) {
        for (int j=0; j < this->label_dic.size(); ++j) {
        }
    }
};

} // namespace strpercpp
