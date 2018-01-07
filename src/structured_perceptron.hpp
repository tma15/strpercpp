#ifndef STRPERCPP_STRUCTUREDPERCEPTRON_H
#define STRPERCPP_STRUCTUREDPERCEPTRON_H
#include <memory>

#include "feature_template.hpp"
#include "dictionary.hpp"
#include "matrix.hpp"
#include "node.hpp"

namespace strpercpp {
    

class StructuredPerceptron {
    private:
        std::string BOS;
        std::string EOS;
        Dictionary label_dic;

        Dictionary feature_dic;

        // (label_size, feature_size) matrix
        Matrix w;

        Matrix w_a;

        void fire(std::shared_ptr<Node> node);

    public:
        std::vector<FeatureTemplate> tmpl;

        StructuredPerceptron();
        ~StructuredPerceptron(){};
        StructuredPerceptron(Dictionary& feature_dic, Dictionary& label_dic);

        Dictionary get_feature_dic() { return this->feature_dic; };
        Dictionary get_label_dic() { return this->label_dic; };

        void read_template(const char* filename);
        void set_template(const std::vector<FeatureTemplate>& tmpl);

        void fit(std::vector< std::shared_ptr< Node > >& nodes,
                std::vector< std::shared_ptr< Node > >& true_path_);

        void fit(std::vector< std::shared_ptr< Node > >& nodes,
                const std::vector< std::string >& labels);

        void fit( const std::vector< std::vector< int > >& feature_ids_list,
                const std::vector< std::string >& labels);

        void update(const std::vector< std::shared_ptr<Node> >& true_path,
                const std::vector< std::shared_ptr< Node > >& pred_path);

        std::vector< std::shared_ptr<Node> > predict(const std::vector< std::vector< int > >& feature_ids);

        std::vector< std::shared_ptr<Node> > predict(std::vector< std::shared_ptr<Node> >& nodes);

        std::vector< std::shared_ptr<Node> > predict(const std::vector< std::vector<std::string> >& sequence);
        void save(const char* filename);
        void load(const char* filename);

        void print_w();
};

} // namespace strpercpp
#endif
