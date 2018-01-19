#include <vector>

#include "lattice.hpp"
#include "corpus.hpp"

namespace strpercpp {
    

std::vector<node_ptr> build_lattice(
        int label_size,
        const std::vector< std::vector< int > >& feature_ids) {

  std::vector<node_ptr> nodes(feature_ids.size()+2);

  node_ptr bos(new Node());
  bos.get()->label = corpus::BOS;
  nodes[0] = bos;

  for (int i=0; i < feature_ids.size(); ++i) {
    for (int j=0; j < label_size; ++j) {
      node_ptr node(new Node());

      node.get()->feature_ids = feature_ids[i];
      node.get()->Y = j;

      if (j==0) {
        nodes[i+1] = node;
      } else {
        node_ptr next_node;
        for (node_ptr n = nodes[i+1]; n != NULL; n = n->bnext) {
          next_node = n;
        }
        next_node->bnext = node;
      }
    }
  }

  node_ptr eos(new Node());
  eos.get()->label = corpus::EOS;
  nodes[nodes.size()-1] = eos;
  return nodes;
};

void print_label_seq(node_ptr n) {
  printf("path score:%.2f, ", n->path_score);
  while (n != NULL) {
    printf("%d ", n->Y);
    n = n->prev;
  }
  printf("\n");
};

// add BOS to curr_queue
// for t=0; t < T; ++t
//   initialize next_queue
//   while !curr_queue.empty()
//     pop element from curr_queue
//     for y=0; y < Y; ++y
//       generate new hypothesis
//       add new hypothesis to next_queue
//   while next_queue.size() > beam_width
//     next_queue.pop()
//   curr_queue = next_queue
std::vector<node_ptr> beamsearch(std::vector<node_ptr>& nodes, int beam_width) {
  node_ptr_queue pq;
  pq.push(nodes[0]);

  int len_seq = nodes.size();

  for (int t=1; t < nodes.size(); ++t) {

    node_ptr_queue next_pq;
    while (!pq.empty()) {
      node_ptr node = pq.top();
      pq.pop();

//      printf("base: ");
//      print_label_seq(node);

      for (node_ptr n = nodes[t]; n != NULL; n = n->bnext) {
        node_ptr n_curr = std::make_shared<Node>(*node);
        node_ptr n_ = std::make_shared<Node>(*n);
        n_->prev = n_curr;
        n_->path_score = n_curr->path_score + n_->score;
        next_pq.push(n_);

//        printf("size:%d add: ", next_pq.size());
//        print_label_seq(n_);
      }
    }

    while (next_pq.size() > beam_width) {
      node_ptr n = next_pq.top();
//      printf("size:%d remove: ", next_pq.size());
//      print_label_seq(n);
      next_pq.pop();
    }
    pq = next_pq;
  }

//  printf("last pq size:%d\n", pq.size());
  std::vector<node_ptr> nbest(beam_width);
  int i = 0;
  while (!pq.empty()) {
    node_ptr n = pq.top();
    nbest[beam_width - 1 - i] = n;
    i += 1;
//    printf("%d pop path score:%f\n", pq.size(), n->path_score);
//    print_label_seq(n);
    pq.pop();
  }
  return nbest;

}

} // namespace strpercpp
