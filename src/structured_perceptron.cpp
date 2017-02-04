#include <iostream>
#include <fstream>
#include <memory>

#include "corpus.hpp"
#include "lattice.hpp"
#include "matrix.hpp"
#include "node.hpp"
#include "structured_perceptron.hpp"
#include "viterbi.hpp"

StructuredPerceptron::StructuredPerceptron() {
};

StructuredPerceptron::StructuredPerceptron(Dictionary& feature_dic, Dictionary& label_dic) {
    this->label_dic = label_dic;
    this->feature_dic = feature_dic;
    this->BOS = corpus::BOS;
    this->EOS = corpus::EOS;
    
    this->w = Matrix(label_dic.size(), feature_dic.size());

};

void StructuredPerceptron::set_template(const std::vector<FeatureTemplate>& tmpl) {
    this->tmpl = tmpl;
}

void StructuredPerceptron::read_template(const char* filename) {

//    std::ifstream ifs(filename);

//    if (ifs.fail()) {
//        std::cerr << "failed to open " << filename << std::endl;
//    }

//    std::string line;
//    while (ifs && getline(ifs, line)) {
//        FeatureTemplate t;
//        t.read(line);
//        this->tmpl.push_back(t);
//    }
    this->tmpl = read_template_file(filename);
};

void StructuredPerceptron::fire(std::shared_ptr<Node> node) {
    node.get()->score = 0.;
    int num_features = node.get()->feature_ids.size();
    int yid = node.get()->Y;
    const std::vector<float> w_y = this->w[yid];
    for (int i=0; i < num_features; ++i) {
        int fid = node->feature_ids[i];

        float w_f = w_y[fid];
        node.get()->score += w_f;
    }
};



void StructuredPerceptron::fit(
        const std::vector< std::vector< int > >& feature_ids,
        const std::vector< std::string >& labels) {

    std::vector< std::shared_ptr< Node> > nodes = build_lattice(this->label_dic.size(), feature_ids);
    this->fit(nodes, labels);
};

void StructuredPerceptron::fit(
        std::vector< std::shared_ptr< Node > >& nodes,
        const std::vector< std::string >& labels) {

    std::vector< std::shared_ptr< Node > > path = this->predict(nodes);

    std::vector<int> true_label_ids(labels.size());
    for (int i=0; i < labels.size(); ++i) {
        true_label_ids[i] = this->label_dic.geti(labels[i]);
    }
    std::vector< std::shared_ptr< Node > > true_path_ = true_path(nodes, true_label_ids);
    this->fit(nodes, true_path_);

};

void StructuredPerceptron::fit(
        std::vector< std::shared_ptr< Node > >& nodes,
        std::vector< std::shared_ptr< Node > >& true_path_) {

    std::vector< std::shared_ptr< Node > > path = this->predict(nodes);

    bool is_correct = true;
    for (int i = 0; i < path.size(); ++i) {
        if (path[i]->Y != true_path_[i]->Y) {
            is_correct = false;
            break;
        }
    }

    if (!is_correct) {
        this->update(true_path_, path);
    } else {
    }
    nodes.clear();
};

void StructuredPerceptron::update(const std::vector< std::shared_ptr< Node > >& true_path,
        const std::vector< std::shared_ptr< Node > >& pred_path) {

    for (int i=0; i < true_path.size(); ++i) {
        std::shared_ptr<Node> n = true_path[i];
        for (auto it = n.get()->feature_ids.begin(); it != n->feature_ids.end(); it++) {
            int fid = *it;
            w.add(n->Y, fid, 1.);
        }
    }

    for (int i=0; i < pred_path.size(); ++i) {
        std::shared_ptr<Node> n = pred_path[i];
        for (auto it = n.get()->feature_ids.begin(); it != n->feature_ids.end(); it++) {
            int fid = *it;
            w.add(n->Y, fid, -1.);
        }
    }
};

std::vector< std::shared_ptr< Node > > StructuredPerceptron::predict(
        const std::vector< std::vector< std::string > >& sequence) {

    bool train = false;
    std::vector< std::vector< int > > fids;
    for (int j=0; j < sequence.size(); ++j) {

        std::vector<int> features = extract_features(
                tmpl,
                &this->feature_dic,
                sequence,
                j,
                train);
        fids.push_back(features);
    }

    std::vector< std::shared_ptr< Node > > path = this->predict(fids);
    return path;
};

std::vector< std::shared_ptr< Node > > StructuredPerceptron::predict(
        const std::vector< std::vector< int > >& feature_ids) {

    std::vector< std::shared_ptr< Node > > nodes = build_lattice(this->label_dic.size(), feature_ids);
    std::vector< std::shared_ptr< Node > > path = this->predict(nodes);
    return path;
};

std::vector< std::shared_ptr< Node > > StructuredPerceptron::predict(
        std::vector< std::shared_ptr< Node > >& nodes) {

    for (int i=0; i < nodes.size(); ++i) {
        for (std::shared_ptr<Node> n = nodes[i]; n != NULL; n = n.get()->bnext) {
            this->fire(n);
        }
    }

    viterbi(nodes);
    std::vector< std::shared_ptr< Node > > path = backtrack(nodes);
    return path;
};

void StructuredPerceptron::save(const char* filename) {
    FILE* fp = fopen(filename, "wb");

    this->label_dic.save(fp);
    this->feature_dic.save(fp);

    int s_tmpl = this->tmpl.size();
    fwrite(&s_tmpl, sizeof(int), 1, fp);
    for (int i=0; i < this->tmpl.size(); ++i) {
        this->tmpl[i].save(fp);
    }

    this->w.save(fp);
    fclose(fp);
}

void StructuredPerceptron::load(const char* filename) {
    FILE* fp = fopen(filename, "rb");

    this->label_dic.load(fp);
    this->feature_dic.load(fp);

    int s_tmpl = 0;
    fread(&s_tmpl, sizeof(int), 1, fp);
    for (int i=0; i < s_tmpl; ++i) {
        FeatureTemplate ft;
        ft.load(fp);
        this->tmpl.push_back(ft);
    }

    this->w.load(fp);
    fclose(fp);
}

void StructuredPerceptron::print_w() {
    for (int i=0; i < this->feature_dic.size(); ++i) {
        for (int j=0; j < this->label_dic.size(); ++j) {
        }
    }
};
