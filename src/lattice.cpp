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
        node_ptr prev_node;
        for (node_ptr n = nodes[i+1]; n != NULL; n = n.get()->bnext) {
          prev_node = n;
        }
        prev_node->bnext = node;
      }
    }
  }

  node_ptr eos(new Node());
  eos.get()->label = corpus::EOS;
  nodes[nodes.size()-1] = eos;
  return nodes;
};

} // namespace strpercpp
