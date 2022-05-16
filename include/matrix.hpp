//
//  matrix.hpp
//  sewup-xcode
//
//  Created by MinChunCho on 2022/5/9.
//

#ifndef matrix_hpp
#define matrix_hpp

#include <vector>
//#include <mkl.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <stdexcept>
//#include <pybind11/pybind11.h>
//#include <pybind11/numpy.h>
//#include <pybind11/stl.h>

typedef enum { negative, positive, illegal, none } Dtype;

class Matrix {
public:
    Matrix(size_t const& row, size_t const& col);
    Matrix(size_t const& row, size_t const& col, std::vector<double> const& vec);
    Matrix(Matrix const& mat);
    Matrix& operator=(Matrix const& mat);
    Matrix(Matrix && mat);
    Matrix& operator=(Matrix && mat);
    ~Matrix(){
        vec_.clear();
        vec_.shrink_to_fit();
    }
    
    Matrix& operator=(std::vector<double> const& vec);
    double& operator()(size_t const& i, size_t const& j);
    double operator()(size_t const& i, size_t const& j) const;
    bool operator==(Matrix const& mat) const;
    bool operator!=(Matrix const& mat) const;
    void operator*=(double const& c);
    friend std::ostream& operator<<(std::ostream& os, Matrix mat);
    friend Matrix multiply_naive(Matrix const& ma, Matrix const& mb);
    friend Matrix multiply_tile(Matrix const& mat1, Matrix const& mat2, size_t tsize);

    size_t nrow() const { return row_; }
    size_t ncol() const { return col_; }
    size_t size() const { return row_*col_; }
    double get_element(std::pair<size_t, size_t> index);
    void set_element(std::pair<size_t, size_t> index, const double& item);
    std::string get_matrix_str();
    Matrix inverse();
    bool dfness(Dtype const&);

private:
    bool bound_check(size_t const& i, size_t const& j);
    Matrix cholesky_decomposition();
    Matrix transpose();

    size_t row_;
    size_t col_;
    std::vector<double> vec_;
};

std::ostream& operator<<(std::ostream& os, Matrix mat);
Matrix multiply_naive(Matrix const& mat1, Matrix const& mat2);
Matrix multiply_tile(Matrix const& mat1, Matrix const& mat2, size_t tsize);

#endif /* matrix_hpp */
