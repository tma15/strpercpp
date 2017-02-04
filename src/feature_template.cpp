#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include "corpus.hpp"
#include "dictionary.hpp"
#include "feature_template.hpp"
#include "util.hpp"


FeatureTemplate::FeatureTemplate() {
    
}

size_t FeatureTemplate::size() {
    return this->rel_pos.size();
}

// U00:%x[-2,0]
// U05:%x[-1,0]/%x[0,0]
void FeatureTemplate::read(std::string& line) {
    const char* c = line.c_str();
    int num_c = strlen(c);

    std::string prefix = "";
    std::string relpos_str = "";
    std::string feature_type_str = "";

    std::string mode = "prefix";
    for (int i=0; i < num_c; ++i) {
//        std::cout << "i:" << c[i] << std::endl;

        if (c[i] == ':') {
            mode = "-";
            continue;
        } else if (c[i] == '[') {
            mode = "relpos";
            continue;
        } else if (c[i] == ',') {
            mode = "feature_type";
            continue;
        } else if (c[i] == ']') {
            mode = "-";
            int relpos = std::stoi(relpos_str);
            int feature_type_id = std::stoi(feature_type_str);
//            std::cout << prefix << std::endl;
//            std::cout << relpos_str << relpos << std::endl;
//            std::cout << feature_type_str << feature_type_id << std::endl;

            this->rel_pos.push_back(relpos);
            this->feature_type.push_back(feature_type_id);
            continue;
        } else if (c[i] == '/') {
            mode = "-";

            std::string relpos_str = "";
            std::string feature_type_str = "";
        }

        if (mode == "prefix") {
            prefix += c[i];
        } else if (mode == "relpos") {
            relpos_str += c[i];
        } else if (mode == "feature_type") {
            feature_type_str += c[i];
        }
    }
    this->prefix = prefix;
}

void FeatureTemplate::save(FILE* fp) {
    save_string(fp, this->prefix);

    int s_rp = this->rel_pos.size();
    fwrite(&s_rp, sizeof(int), 1, fp);
    for (size_t j=0; j < this->rel_pos.size(); ++j) {
        fwrite(&this->rel_pos[j], sizeof(float), 1, fp);
    }

    int s_ft = this->feature_type.size();
    fwrite(&s_ft, sizeof(int), 1, fp);
    for (size_t j=0; j < this->feature_type.size(); ++j) {
        fwrite(&this->feature_type[j], sizeof(float), 1, fp);
    }
}

void FeatureTemplate::load(FILE* fp) {
    read_string(fp, &this->prefix);

    int s_rp = 0;
    fread(&s_rp, sizeof(int), 1, fp);
    for (size_t j=0; j < s_rp; ++j) {
        int i = 0;
        fread(&i, sizeof(int), 1, fp);
        this->rel_pos.push_back(i);
    }

    int s_ft = 0;
    fread(&s_ft, sizeof(int), 1, fp);
    for (size_t j=0; j < s_ft; ++j) {
        int i = 0;
        fread(&i, sizeof(int), 1, fp);
        this->feature_type.push_back(i);
    }
}


std::vector<FeatureTemplate> read_template_file(const char* filename) {
    std::ifstream ifs(filename);

    if (ifs.fail()) {
        std::cerr << "failed to open " << filename << std::endl;
    }

    std::vector<FeatureTemplate> tmpl;

    std::string line;
    while (ifs && getline(ifs, line)) {
        FeatureTemplate t;
        t.read(line);
        tmpl.push_back(t);
    }
    return tmpl;
}

std::vector<int> extract_features(
        const std::vector<FeatureTemplate>& tmpl,
        Dictionary* feature_dic,
        const std::vector< std::vector< std::string > >& sequence,
        const int pos,
        bool train) {

    std::vector<int> features;
    for (int _u=0; _u < tmpl.size(); ++_u) {
        FeatureTemplate u = tmpl[_u];
        std::string prefix = u.prefix;
        std::string f = "";
        for (int k=0; k < u.size(); ++k) {
            int rel_pos = u.rel_pos[k];
            int feature_type_id = u.feature_type[k];
            std::string s("");
            if (pos + rel_pos >= sequence.size()) {
                s = corpus::EOS;
                f += s;
            } else if (pos + rel_pos < 0) {
                s = corpus::BOS;
                f += s;
            } else {
                if (feature_type_id >= sequence[pos+rel_pos].size()) {
                    continue;
                }

                s = sequence[pos+rel_pos][feature_type_id];
                f += s;
            }
        }
        if (f != "") {
            std::string feature = prefix + ":" + f;

            if (train) {
                if (!feature_dic->has(feature)) {
                    feature_dic->add(feature);
                }
            }

            if (feature_dic->has(feature)) {
                int fid = feature_dic->geti(feature);
                features.push_back(fid);
            }
        }
    }
    return features;
};

