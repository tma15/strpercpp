/// \file dictionary.hpp
/// \author Takuya Makino
/// \date 2018/01/13
#ifndef STRPERCPP_DICTIONARY_H
#define STRPERCPP_DICTIONARY_H
#include <string>
#include <unordered_map>
#include <vector>

namespace strpercpp {


/*! @class Dictionary
*   @brief Dictionary holds features for converting them into indice.
*/
class Dictionary {
  public:
    Dictionary();

    /*! checks whether key exists
     *  \param[in] key
     *  \param[out] has whether key exists or not
     */
    bool has(const std::string& key);

    /*! adds key to Dictionary
     *  \param[in] key
     */
    void add(const std::string& key);

    /*! getsstring corresponding to id
     *  \param[in] id the index of a feature
     *  \param[out] key a feature
     */
    std::string gets(const int id) const;

    /*! gets index corresponding to key
     *  \param[in] key
     *  \param[out] id
     */
    int geti(const std::string& key) const;

    /*! gets the size of Dictionary
     *  \param[out] size
     */
    size_t size() const;

    /*! saves Dictionary
     *  \param[in] filename
     */
    void save(const char* filename);
    void save(FILE* fp);

    /*! loads Dictionary
     *  \param[in] filename
     */
    void load(const char* filename);
    void load(FILE* fp);

  private:
    std::vector<std::string> itos;
    std::unordered_map<std::string, int> stoi;
};

} // namespace strpercpp

#endif
