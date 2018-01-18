#include "strpercpp/corpus.hpp"
#include "strpercpp/dictionary.hpp"
#include "strpercpp/feature_template.hpp"
#include "strpercpp/node.hpp"
#include "strpercpp/structured_perceptron.hpp"
#include "strpercpp/viterbi.hpp"

using namespace strpercpp;

void test_lattice() {
  Node node;

  int len_seq = 3;
  std::vector<node_ptr> nodes(len_seq);

  int num_label = 4;
  for (int i=0; i < len_seq; ++i) {

    node_ptr node(new Node());
    node.get()->score = i;
    nodes[i] = node;

    for (int j=1; j < num_label; ++j) {
      node_ptr node(new Node());
      node.get()->score = j;

      node_ptr next_node;
      for (node_ptr n = nodes[i]; n != NULL; n = n.get()->bnext) {
        next_node = n;
      }
      next_node->bnext = node;
    }
  }

  viterbi(nodes);

}

int main(int argc, char const* argv[]) {
  Dictionary dict;
  FeatureTemplate tmpl;
  StructuredPerceptron model;

  test_lattice();
  
  return 0;
}
