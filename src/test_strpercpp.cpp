#include <stdio.h>
#include <unistd.h>

#include "iostream"
#include "string"

#include "corpus.hpp"
#include "dictionary.hpp"
#include "feature_template.hpp"
#include "lattice.hpp"
#include "structured_perceptron.hpp"

using namespace strpercpp;

void eval(
      std::string test_file,
      StructuredPerceptron& perc,
      Dictionary& feature_dic,
      Dictionary& label_dic,
      std::vector<FeatureTemplate>& tmpl,
      bool verbose,
      int beam_width) {

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
    std::vector<node_ptr> nodes = nodes_list[i];

    std::vector<node_ptr> yg = perc.predict(nodes);

    std::vector<node_ptr> y = perc.nbest(nodes, beam_width);
    node_ptr best = y[0];

    int t = 0;
    std::vector<node_ptr> b1(nodes.size()-2);
    node_ptr n = best->prev;
    for (int t=nodes.size()-2; t >= 1; --t) {
      b1[t-1] = n;
      n = n->prev;
    }

    bool is_correct = true;
//    for (int j=0; j < yg.size(); ++j) {
//      int ilabel = yg[j]->Y;
    for (int j=0; j < b1.size(); ++j) {
      int ilabel = b1[j]->Y;

      n_tok += 1;

      std::string slabel = label_dic.gets(ilabel);

      if (verbose) {
        std::cout << sequences[i][j][0] << "\t";
        std::cout << labels[i][j] << "\t";
        std::cout << slabel << std::endl;
      }

      if (slabel == labels[i][j]) {
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
  int beam_width = 1;

  while ((result=getopt(argc, argv, "vb:")) != -1) {
    switch (result) {
      case 'v':
        verbose = true;
        break;
      case 'b':
        beam_width = atoi(optarg);
        break;
    }
  }

  int i = optind;
  char* model_file = argv[i];
  i += 1;
  std::string test_file = argv[i];

  StructuredPerceptron perc;
  perc.load(model_file);

  Dictionary label_dic = perc.get_label_dic();
  Dictionary feature_dic = perc.get_feature_dic();
  std::vector<FeatureTemplate> tmpl = perc.tmpl;

  eval(test_file, perc, feature_dic, label_dic, tmpl, verbose, beam_width);

  return 0;
}
