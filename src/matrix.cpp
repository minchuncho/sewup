//
//  matrix.cpp
//  sewup-xcode
//
//  Created by MinChunCho on 2022/5/9.
//

#include "matrix.hpp"

Matrix::Matrix(size_t const& row, size_t const& col)
    : row_(row), col_(col)
{
    if(!this->vec_.empty()){
        this->vec_.clear();
    }
    
    this->vec_.resize(row*col);
}

Matrix::Matrix(size_t const& row, size_t const& col, std::vector<double> const& vec)
    :row_(row), col_(col)
{
    if(vec.size() != row*col) throw std::out_of_range("size mismatches");
    this->vec_ = vec;
}

// copy constructor
Matrix::Matrix(Matrix const& mat)
{
    this->row_ = mat.row_;
    this->col_ = mat.col_;
    this->vec_ = mat.vec_;
}

// copy assignment
Matrix& Matrix::operator=(Matrix const& mat)
{
    if(this == &mat) return *this;
    
    this->row_ = mat.row_;
    this->col_ = mat.col_;
    this->vec_ = mat.vec_;
    
    return *this;
}

// move constructor
Matrix::Matrix(Matrix && mat)
{
    std::swap(this->row_, mat.row_);
    std::swap(this->col_, mat.col_);
    this->vec_ = std::move(mat.vec_);
}

// move assignment
Matrix& Matrix::operator=(Matrix && mat)
{
    if(this == &mat) return *this;
    
    std::swap(this->row_, mat.row_);
    std::swap(this->col_, mat.col_);
    this->vec_ = std::move(mat.vec_);
    
    return *this;
}

Matrix& Matrix::operator=(std::vector<double> const& vec)
{
    this->vec_ = vec;
    return *this;
}

double& Matrix::operator()(size_t const& i, size_t const& j)
{
    return this->vec_[i*this->col_ + j];
}

double Matrix::operator()(size_t const& i, size_t const& j) const
{
    return this->vec_[i*this->col_ + j];
}

bool Matrix::operator==(Matrix const& mat) const
{
    if(this->col_ != mat.col_ || this->row_ != mat.row_) return false;
    
    size_t total = this->size();
    for(size_t i=0; i<total; ++i){
        // a floating-point precision issue
        // if(this->vec_[i] != mat.vec_[i]) return false;
        if(fabs(this->vec_[i]-mat.vec_[i]) > 0.000001f) return false;
    }
    
    return true;
}

bool Matrix::operator!=(Matrix const& mat) const
{
    return !((*this) == mat);
}

bool Matrix::bound_check(size_t const& i, size_t const& j)
{
    return i<this->row_ && i>=0 && j<this->col_ && j>=0;
}

double Matrix::get_element(std::pair<size_t, size_t> index)
{
    return (*this)(index.first, index.second);
}

void Matrix::set_element(std::pair<size_t, size_t> index, const double& item)
{
    (*this)(index.first, index.second) = item;
}

void Matrix::operator*=(double const& c)
{
    for(int i=0; i<row_; ++i){
        for(int j=0; j<col_; ++j){
            (*this)(i, j) *= -1;
        }
    }
}

std::string Matrix::get_matrix_str()
{
    std::string s;
    for(size_t i=0; i<row_; i++){
        for(size_t j=0; j<col_; j++){
            s += std::to_string((*this)(i, j)) + ' ';
        }
        s += '\n';
    }
    return s;
}

Matrix Matrix::inverse()
{
    if(this->row_ != this->col_){
        throw std::out_of_range("only a square matrix can possibly have an inverse");
    }
    
    double det = determinant(*this);
    if(det == 0){
        throw std::runtime_error("this is a singular matrix");
    }
    
    size_t n = this->row_;
    Matrix adj = this->adjoint(), inv(n, n);
    for(size_t i=0; i<n; ++i){
        for(size_t j=0; j<n; ++j){
            inv(i, j) = adj(i, j)/det;
        }
    }
    
    return inv;
}

Matrix Matrix::cofactor(size_t p, size_t q) const
{
    size_t n = row_, m = n-1;
    Matrix ret(m, m);
    
    size_t k=0;
    for(size_t i=0; i<n; ++i){
        for(size_t j=0; j<n; ++j){
            if(i!=p && j!=q){
                ret.vec_[k] = (*this)(i, j);
                ++k;
            }
        }
    }
    
    return ret;
}

Matrix Matrix::cholesky_decomposition()
{
    Matrix lower(row_, row_);
    
    for(int i=0; i<row_; ++i){
        for(int j=0; j<=i; ++j){
            double sum=0;
            if(j == i){
                for(int k=0; k<j; ++k){
                    sum += pow(lower(j, k), 2);
                }
                    
                lower(j, j) = sqrt((*this)(j, j)-sum);
            } else {
 
                // Evaluating L(i, j) using L(j, j)
                for(int k=0; k<j; ++k){
                    sum += lower(i, k) * lower(j, k);
                }
                lower(i, j) = ((*this)(i, j) - sum)/lower(j, j);
            }
        }
    }
    
    return lower;
}

Matrix Matrix::transpose()
{
    Matrix ret(col_, row_);
    
    for(int i=0; i<row_; ++i){
        for(int j=0; j<col_; ++j){
            ret(j, i) = (*this)(i, j);
        }
    }
    
    return ret;
}

bool Matrix::dfness(Dtype const& dtype)
{
    if(row_ != col_){
        throw std::out_of_range("cholesky_decomposition is only available for square matrices");
    }
    
    // LU decomposition only works for positive-defeniteness
    if(dtype == negative) (*this) *= (-1);
    
    // diagoal elements
    bool diag = true;
    Matrix lower = cholesky_decomposition();
    
    for(int i=0; i<row_; ++i){
        diag &= lower(i, i) > 0;
    }
    
    if(!diag) return false;
    
    // LU
    Matrix upper = lower.transpose();
    Matrix ret = multiply_tile(lower, upper, 32);
    
    if((*this) != ret) return false;
    
    if(dtype == negative) (*this) *= (-1);
    
    return true;
}

Matrix Matrix::adjoint()
{
    size_t n = this->row_;
    Matrix ret(n ,n);
    int sign = 1;
    
    for(size_t i=0; i<n; ++i){
        for(size_t j=0; j<n; ++j){
            sign = ((i+j)%2 == 0)? 1: -1;
            Matrix cf = this->cofactor(i, j);
            ret(j, i) = sign * determinant(cf); // put as transpose
        }
    }
    
    return ret;
}

std::ostream& operator<<(std::ostream& os, Matrix mat)
{
    for(size_t i=0; i<mat.row_; ++i){
        for(size_t j=0; j<mat.col_; ++j){
            std::cout << mat(i, j) << ", ";
        }
        std::cout << std::endl;
    }
    
    return os;
}

Matrix multiply_naive(Matrix const& mat1, Matrix const& mat2)
{
    if(mat1.ncol() != mat2.nrow()){
        throw std::out_of_range("mat1 col is different from mat2 row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for(size_t i=0; i<mat1.nrow(); i++){
        for(size_t j=0; j<mat2.ncol(); j++){
            double v = 0;
            for(size_t k=0; k<mat1.ncol(); k++){
                v += mat1(i, k)*mat2(k, j);
            }
            ret(i,j) = v;
        }
    }

    return ret;
}

Matrix multiply_tile(Matrix const& mat1, Matrix const& mat2, size_t tsize)
{
    size_t m1_col = mat1.ncol(), m1_row = mat1.nrow(), m2_row = mat2.nrow(), m2_col = mat2.ncol();

    if(m1_col != m2_row){
        throw std::out_of_range("mat1 col is different from mat2 row");
    }

    Matrix ret(m1_row, m2_col);
    size_t i_size, j_size, k_size;
    double v=0;

    for(size_t ti=0; ti<m1_row; ti+=tsize){
        i_size = std::min(m1_row, ti+tsize);
        for(size_t tj=0; tj<m2_col; tj+=tsize){
            j_size = std::min(m2_col, tj+tsize);
            for(size_t tk=0; tk<m1_col; tk+=tsize){
                k_size = std::min(m1_col, tk+tsize);
                for(size_t i=ti; i<i_size; ++i){
                    size_t i_col = i * mat1.col_;
                    for(size_t j=tj; j<j_size; ++j){
                        v = 0;
                        for(size_t k=tk; k<k_size; ++k){
                            v += mat1.vec_[i_col+k] * mat2.vec_[k*mat2.col_+j];
                        }
                        ret.vec_[i_col+j] += v;
                    }
                }
            }
        }
    }
    
    return ret;
}

double determinant(Matrix const& mat)
{
    size_t n = mat.row_;
    
    if(n == 1){
        return mat(0, 0);
    }
    
    double ret = 0;
    int sign = 1;
    
    for(size_t i=0; i<n; ++i){
        Matrix cf = mat.cofactor(0, i);
        ret += (sign * mat(0, i) * determinant(cf));
        sign *= (-1);
    }
    
    return ret;
}
