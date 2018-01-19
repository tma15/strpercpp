#include "strpercpp/corpus.hpp"
#include "strpercpp/dictionary.hpp"
#include "strpercpp/feature_template.hpp"
#include "strpercpp/lattice.hpp"
#include "strpercpp/node.hpp"
#include "strpercpp/structured_perceptron.hpp"
#include "strpercpp/viterbi.hpp"

using namespace strpercpp;

void test_pq() {
  node_ptr_queue pq;

  node_ptr n1(new Node());
  n1->path_score = 5;
  pq.push(n1);

  node_ptr n2(new Node());
  n2->path_score = 4;
  pq.push(n2);

  node_ptr n3(new Node());
  n3->path_score = 2;
  pq.push(n3);

  node_ptr n4(new Node());
  n4->path_score = 7;
  pq.push(n4);

  while (!pq.empty()) {
    node_ptr n = pq.top();
//    printf("n score:%f\n", n->path_score);
    pq.pop();
  }

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
      for (node_ptr n = nodes[i]; n != NULL; n = n->bnext) {
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

int main(int argc, char const* argv[]) {
  Dictionary dict;
  FeatureTemplate tmpl;
  StructuredPerceptron model;

//  test_pq();
  test_lattice();
  
  return 0;
}
