#include <stdio.h>
#include <unistd.h>

#include "iostream"
#include "string"

#include "argparse.h"
#include "corpus.hpp"
#include "dictionary.hpp"
#include "feature_template.hpp"
#include "lattice.hpp"
#include "structured_perceptron.hpp"

using namespace strpercpp;

void eval(std::string test_file,
    StructuredPerceptron& perc,
    Dictionary& feature_dic, Dictionary& label_dic,
    std::vector<FeatureTemplate>& tmpl, bool verbose, int beam_width,
    int decode) {

  std::vector< std::vector< std::vector< std::string > > > sequences;
  std::vector< std::vector< std::string > > labels;

  Corpus corpus;

  corpus.read(test_file, &feature_dic, &label_dic, &sequences, &labels);
  
  bool train = false;
  std::vector< std::vector<node_ptr> > nodes_list(sequences.size());
  std::vector< std::vector<node_ptr> > true_path_list(sequences.size());
  corpus.build_lattices(
          &feature_dic, label_dic,
          sequences, labels,
          tmpl,
          &nodes_list,
          &true_path_list,
          train);

//  printf("labelsize:%d\n", label_dic.size());

  int n_correct_sent = 0;
  int n_sent = sequences.size();
  int n_correct_tok = 0;
  int n_tok = 0;
  for (int i=0; i < nodes_list.size(); ++i) {
    std::vector<node_ptr> nodes = nodes_list[i];

    std::vector<node_ptr> y;
    switch (decode) {
      case 0:
        y = perc.predict(nodes);
        break;
      case 1:
        std::vector< std::vector<node_ptr> > ys = perc.nbest(nodes, beam_width);
        y = ys[0];
    }
//    std::vector<node_ptr> yg = perc.predict(nodes);

//    std::vector<node_ptr> y = perc.nbest(nodes, beam_width);
//    node_ptr best = y[0];

//    int t = 0;
//    std::vector<node_ptr> b1(nodes.size()-2);
//    node_ptr n = best->prev;
//    for (int t=nodes.size()-2; t >= 1; --t) {
//      b1[t-1] = n;
//      n = n->prev;
//    }

    bool is_correct = true;
    for (int j=0; j < y.size(); ++j) {
      int ilabel = y[j]->Y;
//    for (int j=0; j < b1.size(); ++j) {
//      int ilabel = b1[j]->Y;

      n_tok += 1;

      std::string slabel = label_dic.gets(ilabel);

      if (verbose) {
        for (int k=0; k < sequences[i][j].size(); ++k) {
          std::cout << sequences[i][j][k] << "\t";
        }
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

  if (!verbose) {
    float acc_tok = float(n_correct_tok) / float(n_tok);
    float acc_sen = float(n_correct_sent) / float(n_sent);
    printf("Accuracy:\n");
    printf("Token %.2f (%d/%d)\n", acc_tok, n_correct_tok, n_tok);
    printf("Sentence %.2f (%d/%d)\n", acc_sen, n_correct_sent, n_sent);
  }

};

//int main(int argc, char const* argv[]) {
int main(int argc, char* argv[]) {
  argparse::ArgParser parser;
  parser.add_argument("-v", "0", "verbose mode");
  parser.add_argument("-b", "width of beam");
  parser.add_argument("-d", "decoding method");
  parser.add_argument("model_file", "model file");
  parser.add_argument("test_file", "test file");
  parser.parse_args(argc, argv);

  std::string model_file = parser.get<std::string>("model_file");
  StructuredPerceptron perc;
  perc.load(model_file.c_str());

  Dictionary label_dic = perc.get_label_dic();
  Dictionary feature_dic = perc.get_feature_dic();
  std::vector<FeatureTemplate> tmpl = perc.tmpl;

  std::string test_file = parser.get<std::string>("test_file");
  int verbose = parser.get<bool>("v");
  int beam_width = parser.get<int>("b");
  int decode = parser.get<int>("d");
  eval(test_file, perc, feature_dic, label_dic, tmpl, verbose, beam_width, decode);

  return 0;
}
