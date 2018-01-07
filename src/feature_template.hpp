#ifndef STRPERCPP_FEATURETEMPLATE_H
#define STRPERCPP_FEATURETEMPLATE_H

#include <string>
#include <vector>

namespace strpercpp {

class FeatureTemplate {
    public:
        std::string prefix;
        std::vector<int> rel_pos;
        std::vector<int> feature_type;

        FeatureTemplate();

        size_t size();

        void read(std::string& line);

        void save(FILE* fp);

        void load(FILE* fp);
};

std::vector<FeatureTemplate> read_template_file(const char* filename);

std::vector<int> extract_features(
        const std::vector<FeatureTemplate>& tmpl,
        Dictionary* feature_dic,
        const std::vector< std::vector< std::string > >& x,
        const int pos,
        bool train);

} // namespace strpercpp

#endif
