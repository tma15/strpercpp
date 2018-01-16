///
/// \file structured_perceptron.hpp
/// \author Takuya Makino
/// \date 2018/01/08
///
#ifndef STRPERCPP_STRUCTUREDPERCEPTRON_H
#define STRPERCPP_STRUCTUREDPERCEPTRON_H
#include <memory>

#include "feature_template.hpp"
#include "dictionary.hpp"
#include "matrix.hpp"
#include "node.hpp"

namespace strpercpp {


/*! @class StructuredPerceptron
 *  @brief StructuredPerceptron supports APIs for training a model and predicting of a
 *  given input.
 */
class StructuredPerceptron {
    public:
      /*! templates of features */
      std::vector<FeatureTemplate> tmpl;

      StructuredPerceptron();
      ~StructuredPerceptron(){};
      StructuredPerceptron(Dictionary& feature_dic, Dictionary& label_dic);

      /*! gets the dictionary of features
       * \param[out] feature_dic Dictionary of features
       */
      Dictionary get_feature_dic() { return this->feature_dic; };

      /*! gets the dictionary of features 
       * \param[out] label_dic Dictionary of labels
       */
      Dictionary get_label_dic() { return this->label_dic; };

      /*! reads the template file of features
       * \param[in] filename the filename of a feature template
       */
      void read_template(const char* filename);

      /*! sets feature templates
       * \param[in] tmpls list of feature template
       */
      void set_template(const std::vector<FeatureTemplate>& tmpls);

      /*! learns a model
       * \param[in] nodes lattice of Node s
       * \param[in] true_path_ true path of Node s
       */
      void fit(std::vector< std::shared_ptr< Node > >& nodes,
              std::vector< std::shared_ptr< Node > >& true_path_);
      void early_update(std::vector< std::shared_ptr< Node > >& nodes,
              std::vector< std::shared_ptr< Node > >& true_path_);

      /*! learns a model
       * \param[in] nodes lattice of Node s
       * \param[in] labels the sequence of true labels
       */
      void fit(std::vector< std::shared_ptr< Node > >& nodes,
              const std::vector< std::string >& labels);

      /*! learns a model
       * \param[in] feature_ids_list list of feature indice
       * \param[in] labels the sequence of true labels
       */
      void fit( const std::vector< std::vector< int > >& feature_ids_list,
              const std::vector< std::string >& labels);

      /*! updates weight vector
       * \param[in] true_path the sequence of true labels
       * \param[in] pred_path the sequence of predicted labels
       */
      void update(const std::vector< std::shared_ptr<Node> >& true_path,
              const std::vector< std::shared_ptr< Node > >& pred_path);

      /*! updates weight vector
       * \param[in] feature_ids the sequence of feature indice
       * \param[out] nodes the sequence of nodes that hold labels
       */
      std::vector< std::shared_ptr<Node> >
      predict(const std::vector< std::vector< int > >& feature_ids);

      /*! updates weight vector
       * \param[in] nodes the lattice of nodes
       * \param[out] nodes the sequence of nodes that hold labels
       */
      std::vector< std::shared_ptr<Node> >
      predict(std::vector< std::shared_ptr<Node> >& nodes);

      /*! updates weight vector
       * \param[in] sequence the sequence of features
       * \param[out] nodes the sequence of nodes that hold labels
       */
      std::vector< std::shared_ptr<Node> >
      predict(const std::vector< std::vector<std::string> >& sequence);

      /*! save learned model
       * \param[in] filename the file name of output
       */
      void save(const char* filename);

      /*! load learned model
       * \param[in] filename the file name of learned model
       */
      void load(const char* filename);

      /*! print weight vector
       */
      void print_w();

    private:
      /*! the dictionary of labels */
      Dictionary label_dic;

      /*! the dictionary of features */
      Dictionary feature_dic;

      /*! weight vector */
      Matrix w;

      /*! calculates the score of a given node.
       * \param[in] node a node of which score is calculated.
      */
      void fire(std::shared_ptr<Node> node);
};

} // namespace strpercpp
#endif
