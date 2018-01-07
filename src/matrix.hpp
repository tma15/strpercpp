#ifndef STRPERCPP_MATRIX_H
#define STRPERCPP_MATRIX_H

#include <vector>

namespace strpercpp {

class Matrix {
    private:
        int col_;
        int row_;
        std::vector< std::vector<float> > matrix_;
    public:
        Matrix();
        Matrix(int row, int col);
        float& operator() (int i, int j);

        std::vector<float> operator[] (int row);
        void reshape(int row, int col);
        std::vector<int> shape();
        void save(FILE* fp);
        void load(FILE* fp);
};

}

#endif
