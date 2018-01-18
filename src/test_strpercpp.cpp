#include <stdio.h>
#include <unistd.h>

#include "iostream"
#include "string"

#include "corpus.hpp"
#include "dictionary.hpp"
#include "feature_template.hpp"
#include "structured_perceptron.hpp"

using namespace strpercpp;

void eval(
      std::string test_file,
      StructuredPerceptron& perc,
      Dictionary& feature_dic,
      Dictionary& label_dic,
      std::vector<FeatureTemplate>& tmpl,
      bool verbose) {

  std::vector< std::vector< std::vector< std::string > > > sequences;
  std::vector< std::vector< std::string > > labels;
  std::vector< std::vector< node_ptr > > nodes_list;
  std::vector< std::vector< node_ptr > > true_path_list;

  Corpus corpus;
  bool train = false;

  corpus.read(test_file,
          &feature_dic, &label_dic,
          &sequences, &labels);
  
  corpus.build_lattices(
          &feature_dic, label_dic,
          sequences, labels,
          tmpl,
          &nodes_list,
          &true_path_list,
          train);

  int n_correct_sent = 0;
  int n_sent = sequences.size();
  int n_correct_tok = 0;
  int n_tok = 0;
  for (int i=0; i < nodes_list.size(); ++i) {
    std::vector< node_ptr > nodes = nodes_list[i];
    std::vector< node_ptr > y = perc.predict(nodes);

    bool is_correct = true;
    for (int j=0; j < y.size(); ++j) {
      n_tok += 1;

      std::string labe = label_dic.gets(y[j]->Y);

      if (verbose) {
        std::cout << sequences[i][j][0] << "\t";
        std::cout << labels[i][j] << "\t";
        std::cout << labe << std::endl;
      }

      if (labe == labels[i][j]) {
        n_correct_tok += 1;
      } else {
        is_correct = false;
      }

    }

    if (is_correct) {
      n_correct_sent += 1;
    }
    if (verbose) {
      std::cout << std::endl;
    }
  }
  std::cout << n_correct_tok << "/" << n_tok << std::endl;
  std::cout << n_correct_sent << "/" << n_sent << std::endl;

};

int main(int argc, char* argv[]) {
  int result;
  bool verbose = false;

  while ((result=getopt(argc, argv, "v")) != -1) {
    switch (result) {
      case 'v':
        verbose = true;
        break;
    }
  }

  int i = optind;
  char* model_file = argv[i];
  i += 1;
  std::string test_file = argv[i];

//  char* model_file = argv[1];
//  std::string test_file = argv[2];

  StructuredPerceptron perc;
  perc.load(model_file);

  Dictionary label_dic = perc.get_label_dic();
  Dictionary feature_dic = perc.get_feature_dic();
  std::vector<FeatureTemplate> tmpl = perc.tmpl;

  eval(test_file, perc, feature_dic, label_dic, tmpl, verbose);

  return 0;
}
