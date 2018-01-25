#include <stdio.h>
#include <unistd.h>

#include "iostream"
#include "string"

#include "argparse.h"
#include "corpus.hpp"
#include "dictionary.hpp"
#include "feature_template.hpp"
#include "structured_perceptron.hpp"

using namespace strpercpp;


void usage() {
  std::cout << "train_strpercpp -e <epoch> -u <update_rule> <train_data> ";
  std::cout << "<template_file> <model_file>";
  std::cout << std::endl;

  std::cout << "  -e\tnumber of epoch" << std::endl;
  std::cout << "  -u\tupdate rule" << std::endl;
  std::cout << "\t0: full update" << std::endl;
  std::cout << "\t1: early update" << std::endl;
  std::cout << std::endl;
}

int main(int argc, char const* argv[]) {
  argparse::ArgParser parser;
  parser.add_argument("-e", "3", "number of epoch");
  parser.add_argument("--update", "0", "updating rule");
  parser.add_argument("train_file", "training file");
  parser.add_argument("template_file", "template file");
  parser.add_argument("model_file", "model file");
  parser.parse_args(argc, argv);

  int epoch = parser.get<int>("e");
  int update_rule = parser.get<int>("update");
  std::string train_file = parser.get<std::string>("train_file");
  std::string template_file = parser.get<std::string>("template_file");
  std::string model_file = parser.get<std::string>("model_file");

//  printf("epoch:%d\n", epoch);
//  printf("update_rule:%d\n", update_rule);
//  printf("train_file:%s\n", train_file.c_str());
//  printf("template_file:%s\n", template_file.c_str());
//  printf("model_file:%s\n", model_file.c_str());
//  exit(1);

  Dictionary feature_dic;
  Dictionary label_dic;

  std::vector< std::vector< std::vector< std::string > > > sequences;
  std::vector< std::vector< std::string > > labels;
  std::vector< std::vector< node_ptr > > nodes_list;
  std::vector< std::vector< node_ptr > > true_path_list;

  std::vector<FeatureTemplate> tmpl = read_template_file(template_file.c_str());
  Corpus corpus;
  corpus.read(train_file, &feature_dic, &label_dic, &sequences, &labels);

  bool train = true;
  corpus.build_lattices(&feature_dic, label_dic,
      sequences, labels, tmpl, &nodes_list, &true_path_list, train);

  StructuredPerceptron perc(feature_dic, label_dic);
  perc.set_template(tmpl);

  std::cout << "#samples: " << sequences.size() << std::endl;
  std::cout << "#labels: " << label_dic.size() << std::endl;
  std::cout << "#features: " << feature_dic.size() << std::endl;

  for (int e=0; e < epoch; ++e) {
    std::cout << "epoch:" << e+1 << "/" << epoch << std::endl;
    for (int i=0; i < nodes_list.size(); ++i) {
      std::vector< node_ptr > nodes = nodes_list[i];
      std::vector< node_ptr > true_path_ = true_path_list[i];

      switch (update_rule) {
        case 0:
         perc.fit(nodes, true_path_);
         break;
        case 1:
         perc.early_update(nodes, true_path_);
         break;
      }
    }
  }

  std::cout << "finish training" << std::endl;

  perc.save(model_file.c_str());

//    StructuredPerceptron perc2;
//    perc2.load(model_file);

//    Dictionary label_dic2 = perc2.get_label_dic();
//    Dictionary feature_dic2 = perc2.get_feature_dic();
//    std::vector<FeatureTemplate> tmpl2 = perc2.tmpl;

//    eval(test_file, perc2, feature_dic2, label_dic2, tmpl2);

    return 0;
}
