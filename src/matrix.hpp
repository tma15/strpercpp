///
/// \file matrix.hpp
/// \author Takuya Makino
/// \date 2018/01/13
///
#ifndef STRPERCPP_MATRIX_H
#define STRPERCPP_MATRIX_H

#include <vector>

namespace strpercpp {

/*! @class Matrix
 *  @brief Matrix class is used as weight vectors of features.
 */
class Matrix {
  public:
    Matrix();
    Matrix(int row, int col);

    /*! returns weight of a feature
     * \param[in] i index of row
     * \param[in] j index of column
     * \param[out] weight weight of feature
     */
    float& operator() (int i, int j);

    /*! returns the weight vector associated with the index of a feature.
     *  \param[out] vec weight vetctor associated with a feature
     */
    std::vector<float> operator[] (int row);

    /*! reshapes matrix
     * \param[in] row index of row
     * \param[in] col index of column
     */
    void reshape(int row, int col);

    /*! returns shape of matrix
     *  \param[out] shape shape of matrix
     */
    std::vector<int> shape();

    /*! saves matrix
     * \param[in] fp file object
     */
    void save(FILE* fp);

    /*! loads matrix
     * \param[in] fp file object
     */
    void load(FILE* fp);

  private:
    int col_;
    int row_;
    std::vector< std::vector<float> > matrix_;

};

} // namespace strpercpp

#endif
