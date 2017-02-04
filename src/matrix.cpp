#include <iostream>

#include "matrix.hpp"

Matrix::Matrix() {
};

Matrix::Matrix(int row, int col) {
    this->col_ = col;
    this->row_ = row;
    this->matrix_ = std::vector< std::vector<float> >(row, std::vector<float>(col, 0.));
};

//std::vector<float>* Matrix::operator[] (int row) {
std::vector<float> Matrix::operator[] (int row) {
//    return &this->matrix_[row];
    return this->matrix_[row];
};

void Matrix::add(int row, int col, float val) {
    this->matrix_[row][col] += val;
}

std::vector<int> Matrix::shape() {
    int r = this->matrix_.size();
    int c = this->matrix_[0].size();
    std::vector<int> ret(2);
    ret[0] = r;
    ret[1] = c;
    return ret;
}

void Matrix::reshape(int row, int col) {
    for (int i=this->matrix_.size(); i <= row; ++i) {
        std::vector<float> vec;
        this->matrix_.push_back(vec);
    }

    for (int i=0; i <= row; ++i) {
        for (int j=this->matrix_[i].size(); j <= col; ++j) {
            this->matrix_[i].push_back(0.);
        }
    }
}

void Matrix::save(FILE* fp) {
    fwrite(&this->row_, sizeof(int), 1, fp);
    fwrite(&this->col_, sizeof(int), 1, fp);

    for (size_t i=0; i < this->row_; ++i) {
        for (size_t j=0; j < this->col_; ++j) {
            fwrite(&this->matrix_[i][j], sizeof(float), 1, fp);
        }
    }
}

void Matrix::load(FILE* fp) {
    fread(&this->row_, sizeof(int), 1, fp);
    fread(&this->col_, sizeof(int), 1, fp);

    this->matrix_ = std::vector< std::vector<float> >(this->row_, std::vector<float>(this->col_, 0.));

    for (size_t i=0; i < this->row_; ++i) {
        for (size_t j=0; j < this->col_; ++j) {
            fread(&this->matrix_[i][j], sizeof(float), 1, fp);
        }
    }
}
