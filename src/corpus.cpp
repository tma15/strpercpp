#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "corpus.hpp"
#include "feature_template.hpp"
#include "lattice.hpp"
#include "viterbi.hpp"

namespace strpercpp {
    

std::vector<std::string> split(std::string& line, char sep) {
  std::vector<std::string> ret;
  std::stringstream ss(line);
  std::string buffer("");
  while (std::getline(ss, buffer, sep)) {
    ret.push_back(buffer);
  }
  return ret;
}

void print_vec(const std::vector< std::string >& vec) {
  if (vec.size() > 0) {
    for (int i=0; i < vec.size()-1; ++i) {
      std::cout << vec[i] << " ";
    }
    std::cout << vec[vec.size()-1] << std::endl;
  } else {
    std::cout << std::endl;
  }
}

void print_set(const std::unordered_set< int >& set) {
  for (auto it = set.begin(); it != set.end(); it++) {
    std::cout << *it << std::endl;
  }
}

void print_data(const std::vector< std::vector< std::vector< std::string > > > sequences,
    const std::vector< std::vector< std::string > > labels) {

  for (int i=0; i < sequences.size(); ++i) {
    for (int j=0; j < sequences[i].size(); ++j) {
      for (int k=0; k < sequences[i][j].size(); ++k) {
        std::cout << sequences[i][j][k] << " ";
      }
      std::cout << labels[i][j] << std::endl;
    }
  }

}


Corpus::Corpus() {
};


void Corpus::read(
        const std::string& filename,
        Dictionary* feature_dict, Dictionary* label_dict,
        std::vector< std::vector< std::vector< std::string > > >* sequences,
        std::vector< std::vector< std::string > >* labels) {
  std::ifstream ifs(filename);

  if (ifs.fail()) {
    std::cerr << "failed to open " << filename << std::endl;
  }

  if (!label_dict->has(corpus::BOS)) {
    label_dict->add(corpus::BOS);
  }
  if (!label_dict->has(corpus::BOS)) {
    label_dict->add(corpus::EOS);
  }

  std::vector< std::vector< std::string > > x;
  std::vector< std::string > y;

  std::string line;
  while (ifs && getline(ifs, line)) {
    std::vector<std::string> elems = split(line, ' ');

    int size_elem = elems.size();

    if (size_elem == 0) {
      if (x.size() > 0 && y.size() > 0) {
        sequences->push_back(x);
        labels->push_back(y);
      }
      std::vector< std::vector< std::string > >().swap(x);
      std::vector< std::string >().swap(y);
    } else {
      std::vector<std::string> features(size_elem - 1);
      for (size_t i = 0; i < size_elem - 1; ++i) {
        std::string elem = elems[i];
        features[i] = elem;
      }
      x.push_back(features);

      std::string label = elems[size_elem - 1];
      y.push_back(label);

      if (!label_dict->has(label)) {
        label_dict->add(label);
      }
    }
  }
};

void Corpus::build_lattices(
    Dictionary* feature_dict,
    const Dictionary& label_dict,
    const std::vector< std::vector< std::vector< std::string > > >& sequences,
    const std::vector< std::vector< std::string > >& labels,
    const std::vector< FeatureTemplate >& tmpl,
    std::vector< std::vector< node_ptr > >* nodes_list,
    std::vector< std::vector< node_ptr > >* true_path_list,
    bool train) {

  int label_size = label_dict.size();
  for (int i=0; i < sequences.size(); ++i) {
    std::vector< std::vector< int > > fids;
    std::vector<int> yids;
    for (int j=0; j < sequences[i].size(); ++j) {
      yids.push_back(label_dict.geti(labels[i][j]));

      std::vector<int> features = extract_features(
              tmpl,
              feature_dict,
              sequences[i],
              j,
              train);
      fids.push_back(features);
    }

    std::vector<node_ptr> nodes = build_lattice(label_size, fids);
    std::vector<node_ptr> true_path_ = true_path(nodes, yids);

    nodes_list->push_back(nodes);
    true_path_list->push_back(true_path_);
  }

};

} // namespace strpercpp
