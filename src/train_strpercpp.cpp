#include <stdio.h>
#include <unistd.h>

#include "iostream"
#include "string"

#include "corpus.hpp"
#include "dictionary.hpp"
#include "feature_template.hpp"
#include "structured_perceptron.hpp"

using namespace strpercpp;


void usage() {
  std::cout << "train_strpercpp -e <epoch> <train_data> <template_file> <model_file>";
  std::cout << std::endl;

  std::cout << "  -e\tnumber of epoch";
  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  int result;
  int epoch = 3;

  while ((result=getopt(argc, argv, "he:")) != -1) {
    switch (result) {
      case 'h':
        usage();
        exit(0);
      case 'e':
        epoch = atoi(optarg);
        break;
    }
  }

  int i = optind;
  std::string train_file = std::string(argv[i]);
  i += 1;
  char* template_file = argv[i];
  i += 1;
  char* model_file = argv[i];


  Dictionary feature_dic;
  Dictionary label_dic;

  std::vector< std::vector< std::vector< std::string > > > sequences;
  std::vector< std::vector< std::string > > labels;
  std::vector< std::vector< node_ptr > > nodes_list;
  std::vector< std::vector< node_ptr > > true_path_list;

  std::vector<FeatureTemplate> tmpl = read_template_file(template_file);
  Corpus corpus;
  bool train = true;
  corpus.read(train_file,
          &feature_dic, &label_dic,
          &sequences, &labels);

  corpus.build_lattices(
          &feature_dic, label_dic,
          sequences, labels,
          tmpl,
          &nodes_list,
          &true_path_list,
          train);
//  exit(0);

  StructuredPerceptron perc(feature_dic, label_dic);
  perc.set_template(tmpl);

  std::cout << "#samples: " << sequences.size() << std::endl;
  std::cout << "#labels: " << label_dic.size() << std::endl;
  std::cout << "#featres: " << feature_dic.size() << std::endl;
//  exit(0);
  for (int e=0; e < epoch; ++e) {
    std::cout << "epoch:" << e+1 << "/" << epoch << std::endl;
    for (int i=0; i < nodes_list.size(); ++i) {
      std::vector< node_ptr > nodes = nodes_list[i];
      std::vector< node_ptr > true_path_ = true_path_list[i];
      perc.fit(nodes, true_path_);
//      perc.early_update(nodes, true_path_);
    }
  }

  std::cout << "finish training" << std::endl;

  perc.save(model_file);

//    StructuredPerceptron perc2;
//    perc2.load(model_file);

//    Dictionary label_dic2 = perc2.get_label_dic();
//    Dictionary feature_dic2 = perc2.get_feature_dic();
//    std::vector<FeatureTemplate> tmpl2 = perc2.tmpl;

//    eval(test_file, perc2, feature_dic2, label_dic2, tmpl2);

    return 0;
}
