#ifndef STRPERCPP_DICTIONARY_H
#define STRPERCPP_DICTIONARY_H
#include <string>
#include <unordered_map>
#include <vector>

namespace strpercpp {
    

class Dictionary {
    private:
        std::vector<std::string> itos;
        std::unordered_map<std::string, int> stoi;
    public:
        Dictionary();
        bool has(const std::string& key);
        void add(const std::string& key);
        std::string gets(const int id) const;
        int geti(const std::string& key) const;
        size_t size() const;
        void save(const char* filename);
        void save(FILE* fp);
        void load(const char* filename);
        void load(FILE* fp);
};

} // namespace strpercpp

#endif
