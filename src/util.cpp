
#include "util.hpp"

void save_string(FILE* fp, const std::string& line) {
    size_t s = line.size();
    fwrite(&s, sizeof(size_t), 1, fp);
    fwrite(line.data(), sizeof(char), s, fp);
};

void read_string(FILE* fp, std::string* line) {
    size_t line_len;
    fread(&line_len, sizeof(size_t), 1, fp);

    for (size_t k=0; k < line_len; ++k) {
        char ch;
        fread(&ch, sizeof(char), 1, fp);
        line->push_back(ch);
    }

};
