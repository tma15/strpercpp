///
/// \file early_update_perceptron.hpp
/// \author Takuya Makino
/// \date 2018/01/25
#ifndef STRPERCPP_EARLY_UPDATE_HPP
#define STRPERCPP_EARLY_UPDATE_HPP

#include "structured_perceptron.hpp"

namespace strpercpp {

class GreedyEarlyUpdate: public StructuredPerceptron {
  public:
    GreedyEarlyUpdate(){};

    ~GreedyEarlyUpdate(){};

    GreedyEarlyUpdate(Dictionary& _feature_dic, Dictionary& _label_dic):
      StructuredPerceptron(_feature_dic, _label_dic) {
    };
    
    void fit(std::vector<node_ptr>& nodes, std::vector<node_ptr>& true_path_) {
      _fit(nodes, true_path_);
    };

  private:
    void _fit(std::vector<node_ptr>& nodes, std::vector<node_ptr>& true_path_);
};

class BeamEarlyUpdate: public StructuredPerceptron {
  public:
    BeamEarlyUpdate(): beam_width_(5) {};

    BeamEarlyUpdate(int beam_width): beam_width_(beam_width) {};

    ~BeamEarlyUpdate(){};

    BeamEarlyUpdate(Dictionary& _feature_dic, Dictionary& _label_dic):
      StructuredPerceptron(_feature_dic, _label_dic), beam_width_(5) {
    };

    BeamEarlyUpdate(Dictionary& _feature_dic, Dictionary& _label_dic, int beam_width):
      StructuredPerceptron(_feature_dic, _label_dic), beam_width_(beam_width) {
    };
   
    void fit(std::vector<node_ptr>& nodes, std::vector<node_ptr>& true_path_) {
      _fit(nodes, true_path_);
    };

  private:
    int beam_width_;
    void _fit(std::vector<node_ptr>& nodes, std::vector<node_ptr>& true_path_);
};

class MaxViolationUpdate: public StructuredPerceptron {
  public:
    MaxViolationUpdate(): beam_width_(5) {};

    MaxViolationUpdate(int beam_width): beam_width_(beam_width) {};

    ~MaxViolationUpdate(){};

    MaxViolationUpdate(Dictionary& _feature_dic, Dictionary& _label_dic):
      StructuredPerceptron(_feature_dic, _label_dic), beam_width_(5) {
    };

    MaxViolationUpdate(Dictionary& _feature_dic, Dictionary& _label_dic, int beam_width):
      StructuredPerceptron(_feature_dic, _label_dic), beam_width_(beam_width) {
    };
   
    void fit(std::vector<node_ptr>& nodes, std::vector<node_ptr>& true_path_) {
      _fit(nodes, true_path_);
    };

  private:
    int beam_width_;
    void _fit(std::vector<node_ptr>& nodes, std::vector<node_ptr>& true_path_);

};



} // namespace strpercpp

#endif // STRPERCPP_EARLY_UPDATE_HPP
