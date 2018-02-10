#include "dictionary.hpp"
#include "util.hpp"

namespace strpercpp {
    

Dictionary::Dictionary() {
};

bool Dictionary::has(const std::string& key) const {
    return this->stoi.count(key);
};

void Dictionary::add(const std::string& key) {
    int i = this->itos.size();
    this->itos.push_back(key);
    this->stoi[key] = i;
};

size_t Dictionary::size() const {
    return this->itos.size();
};

std::string Dictionary::gets(const int value) const {
    return this->itos[value];
};

int Dictionary::geti(const std::string& key) const {
    std::unordered_map<std::string, int>::const_iterator it = this->stoi.find(key);
    return it->second;
};

void Dictionary::save(const char* filename) {
    FILE* fp = fopen(filename, "wb");
    this->save(fp);
}

void Dictionary::save(FILE* fp) {
    size_t num_tok = this->size();
    fwrite(&num_tok, sizeof(size_t), 1, fp);

    for (size_t i=0; i < this->itos.size(); i++) {
        std::string tok = this->itos[i];
        save_string(fp, tok);
    }
}

void Dictionary::load(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    this->load(fp);
}

void Dictionary::load(FILE* fp) {
    size_t num_tok;
    fread(&num_tok, sizeof(size_t), 1, fp);

    for (int i=0; i < num_tok; ++i) {

        std::string tok = "";
        read_string(fp, &tok);

        this->add(tok);
    }

}

} // namespace strpercpp
