#include <vector>

#include "lattice.hpp"
#include "corpus.hpp"

std::vector< std::shared_ptr< Node > > build_lattice(
        int label_size,
        const std::vector< std::vector< int > >& feature_ids) {

    std::vector< std::shared_ptr<Node> > nodes(feature_ids.size()+2);

    std::shared_ptr<Node> bos(new Node());
    bos.get()->label = corpus::BOS;
    nodes[0] = bos;

    for (int i=0; i < feature_ids.size(); ++i) {
        for (int j=0; j < label_size; ++j) {
            std::shared_ptr<Node> node(new Node());

            node.get()->feature_ids = feature_ids[i];
            node.get()->Y = j;

            if (j==0) {
                nodes[i+1] = node;
            } else {
                std::shared_ptr<Node> prev_node;
                for (std::shared_ptr<Node> n = nodes[i+1]; n != NULL; n = n.get()->bnext) {
                    prev_node = n;
                }
                prev_node->bnext = node;
            }
        }
    }

    std::shared_ptr<Node> eos(new Node());
    eos.get()->label = corpus::EOS;
    nodes[nodes.size()-1] = eos;
    return nodes;
};

