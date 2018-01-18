/// \file corpus.hpp
/// \author Takuya Makino
/// \date 2018/01/13
#ifndef STRPERCPP_CORPUS_H
#define STRPERCPP_CORPUS_H

#include <memory>
#include <unordered_set>

#include "node.hpp"
#include "dictionary.hpp"
#include "feature_template.hpp"

namespace strpercpp {

namespace corpus {

const std::string BOS = "_BOS_";
const std::string EOS = "_EOS_";

}


std::vector<std::string> split(std::string& line, char sep);

void print_vec(const std::vector< std::string >& vec);
void print_set(const std::unordered_set< int >& set);

void print_data(const std::vector< std::vector< std::vector< std::string > > > sequences,
  const std::vector< std::vector< std::string > > labels);


/*! @class Corpus
 *
 */
class Corpus {
  public:
    Corpus();

    void read(
      const std::string& filename,
      Dictionary* feature_dict, Dictionary* label_dict,
      std::vector< std::vector< std::vector< std::string > > >* sequences,
      std::vector< std::vector< std::string > >* labels);

    void build_lattices(
      Dictionary* feature_dict,
      const Dictionary& label_dict,
      const std::vector< std::vector< std::vector< std::string > > >& sequences,
      const std::vector< std::vector< std::string > >& labels,
      const std::vector< FeatureTemplate >& tmpl,
      std::vector< std::vector< node_ptr > >* nodes_list,
      std::vector< std::vector< node_ptr > >* true_path_list,
      bool train);
};

} // namespace strpercpp

#endif
