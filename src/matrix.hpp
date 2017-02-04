#ifndef STRPERCPP_MATRIX_H
#define STRPERCPP_MATRIX_H

#include <vector>

class Matrix {
    private:
        int col_;
        int row_;
        std::vector< std::vector<float> > matrix_;
    public:
        Matrix();
        Matrix(int row, int col);
        std::vector<float> operator[] (int row);
        void add(int row, int col, float val);
        void reshape(int row, int col);
        std::vector<int> shape();
        void save(FILE* fp);
        void load(FILE* fp);
//        std::vector<float>* operator[] (int row);
//        void operator+ (float val);
};

#endif
