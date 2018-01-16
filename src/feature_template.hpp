/// \file feature_template.hpp
/// \author Takuya Makino
/// \date 2018/01/13
#ifndef STRPERCPP_FEATURETEMPLATE_H
#define STRPERCPP_FEATURETEMPLATE_H

#include <string>
#include <vector>

#include "dictionary.hpp"

namespace strpercpp {

/*! @class FeatureTemplate
 *  @brief FeatureTemplate extracts feature information from template file.
 */
class FeatureTemplate {
  public:
    /*! prefix of feature such as U */
    std::string prefix;

    /*! relative positions of features */
    std::vector<int> rel_pos;

    /*! types of feature */
    std::vector<int> feature_type;

    FeatureTemplate();

    size_t size();

    void read(std::string& line);

    /*! saves feature tempalte from a file
     * \param[in] fp file object
     */
    void save(FILE* fp);

    /*! loads feature tempalte from a file
     * \param[in] fp file object
     */
    void load(FILE* fp);
};

/*! read all feature templates from a file */
std::vector<FeatureTemplate> read_template_file(const char* filename);

std::vector<int> extract_features(
  const std::vector<FeatureTemplate>& tmpl,
  Dictionary* feature_dic,
  const std::vector< std::vector< std::string > >& x,
  const int pos,
  bool train);

} // namespace strpercpp

#endif
