#include "../src/argparse.h"
#include "../src/corpus.hpp"
#include "../src/dictionary.hpp"
#include "../src/feature_template.hpp"
#include "../src/lattice.hpp"
#include "../src/node.hpp"
#include "../src/structured_perceptron.hpp"
#include "../src/viterbi.hpp"

#include <gtest/gtest.h>


namespace {

using namespace strpercpp;

TEST(TestArgParse, ParseArgs) {
  using namespace argparse;
  ArgParser parser = ArgParser();


  int argc = 6;
  char* argv[] = {(char*)"train_strpercpp", (char*)"-e",
    (char*)"10", (char*)"train.txt", (char*)"template", (char*)"model"};

  parser.add_argument("-e", "10", "number of epoch");
  parser.add_argument("--update", "0", "updating rule");
  parser.add_argument("train_file", "a");
  parser.add_argument("template_file", "b");
  parser.add_argument("model_file", "c");
  parser.parse_args(argc, argv);

  int expected1 = 10;
  int epoch = parser.get<int>("e");
  EXPECT_EQ(expected1, epoch);

  int expected2 = 0;
  int got2 = parser.get<int>("update");
  EXPECT_EQ(expected2, got2);

  std::string expected3 = "train.txt";
  std::string got3 = parser.get<std::string>("train_file");
  EXPECT_EQ(expected3, got3);

  std::string expected4 = "template";
  std::string got4 = parser.get<std::string>("template_file");
  EXPECT_EQ(expected4, got4);
}


void test_lattice() {
  Node node;

  int len_seq = 4;
  std::vector<node_ptr> nodes(len_seq);

  int num_label = 4;

  // BOS
  node_ptr bos(new Node());
  bos->Y = 0;
  nodes[0] = bos;

  // internal nodes
  for (int i=1; i < len_seq-1; ++i) {

    // j = 0
    node_ptr node(new Node());
    node->score = 0;
    node->Y = 0;
    nodes[i] = node;

    for (int j=1; j < num_label; ++j) {
      node_ptr node(new Node());
      node->score = j;
      node->Y = j;

      node_ptr next_node;
      for (node_ptr n = nodes[i]; n != nullptr; n = n->bnext) {
        next_node = n;
      }
      next_node->bnext = node;
    }
  }

  // EOS
  node_ptr eos(new Node());
  eos->Y = num_label;
  nodes[nodes.size()-1] = eos;

//  viterbi(nodes);

  int beam_width = 2;
  std::vector<node_ptr> nbest = beamsearch(nodes, beam_width);
  for (int i=0; i < beam_width; ++i) {
    node_ptr n = nbest[i];
    printf("%d path score:%f\n", i, n->path_score);
    print_label_seq(n);
  }

}

} // namespace

