//
//  polynomial.cpp
//  sewup-xcode
//
//  Created by MinChunCho on 2022/5/8.
//

#include "polynomial.hpp"

std::regex Polynomial::token_exp_ = make_regexp("-?(([0-9]*[.])?[0-9]+)?((x)_([1-9][0-9]*))?((x)_([1-9][0-9]*))?");
std::regex Polynomial::int_exp_ = make_regexp("[1-9][0-9]*");
std::regex Polynomial::singleton_exp_ = make_regexp("(x)_([1-9][0-9]*)");
std::regex Polynomial::doubleton_exp_ = make_regexp("(x)_([1-9][0-9]*)(x)_([1-9][0-9]*)?");
std::vector<size_t> Polynomial::starting_index_ = make_starting_index();

Polynomial::Polynomial(size_t const& dim)
    : dim_(dim), terms_(dim, 0) {}

Polynomial::Polynomial(size_t const& dim, std::string const& func)
    : dim_(dim), terms_(dim, 0)
{
    *this = func;
}

Polynomial::Polynomial(Polynomial const& other)
    : dim_(other.dim_), terms_(other.terms_) {}

Polynomial::Polynomial(Polynomial&& other)
    : dim_(std::move(other.dim_)), terms_(std::move(other.terms_)) {}

Polynomial& Polynomial::operator=(Polynomial const& other)
{
    if(&other != this){
        dim_ = other.dim_;
        terms_ = other.terms_;
    }
    
    return *this;
}

Polynomial& Polynomial::operator=(Polynomial&& other)
{
    if(&other != this){
        dim_ = std::move(other.dim_);
        terms_ = std::move(other.terms_);
    }
    
    return *this;
}

void Polynomial::operator=(std::string const& func)
{
    std::vector<std::string> tokens = get_tokens(func);
    analyze_tokens(tokens);
}

double& Polynomial::operator()(size_t const& row, size_t const& col)
{
    if(row >= dim_ || col >= dim_){
        throw std::out_of_range("row or col is larger than the dimension");
    }
    
    if(row>col) return terms_[starting_index_[col]+row];
    else return terms_[starting_index_[row]+col];
}

double const& Polynomial::operator()(size_t const& row, size_t const& col) const
{
    if(row >= dim_ || col >= dim_){
        throw std::out_of_range("row or col is larger than the dimension");
    }
    
    if(row>col) return terms_[starting_index_[col]+row];
    else return terms_[starting_index_[row]+col];
}

void Polynomial::operator+=(Polynomial const& p)
{
    for(int i=0; i<dim_; ++i){
        for(int j=0; j<dim_; ++j){
            (*this)(i, j) += p(i, j);
        }
    }
}

void Polynomial::operator*=(double const& c)
{
    for(int i=0; i<dim_; ++i){
        for(int j=0; j<dim_; ++j){
            (*this)(i, j) = (*this)(i, j) * c;
        }
    }
}

void Polynomial::operator*=(Polynomial const& p)
{
    size_t n = dim_;
    if(n != p.dim_) throw std::out_of_range("function dimensions are different");
    
    Polynomial ret(n);
    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            (*this)(i, j) += (*this)(0, i) * p(0, j);
        }
    }
}

bool Polynomial::operator==(Polynomial const& p) const
{
    size_t n = dim_;
    if(n != p.dim_) return false;
    
    for(int i=0; i<n; ++i){
        for(int j=i; j<n; ++j){
            if((*this)(i, j) != p(i, j)) return false;
        }
    }

    return true;
}

std::regex Polynomial::make_regexp(std::string const& regexp)
{
    std::regex ret(regexp);
    return ret;
}

std::vector<size_t> Polynomial::make_starting_index()
{
    std::vector<size_t> ret(DIM_MAX, 0);
    for(int i=1; i<=DIM_MAX; ++i){
        ret[i] = ret[i-1] + (DIM_MAX-i+1);
    }
    
    return ret;
}

std::vector<std::string> Polynomial::get_tokens(std::string str)
{
    size_t s=0, e=s, n=str.length();
    std::vector<std::string> tokens;
    std::smatch sm;
    while(e < n && s < n){
        if(str[e] == '+'){
            std::string token = str.substr(s, e-s);
            if(!std::regex_match(token, sm, token_exp_)){
                throw std::invalid_argument("wrong function format");
            }
            tokens.emplace_back(token);
            s=e+1;
        }
        else if(str[e] == '-'){
            std::string token = str.substr(s, e-s);
            if(!std::regex_match(token, sm, token_exp_)){
                throw std::invalid_argument("wrong function format");
            }
            tokens.emplace_back(token); // include the substraction sign
            s=e;
        }
        ++e;
    }
    
    tokens.emplace_back(str.substr(s, e-s)); // the last substring
    
    return tokens;
}

void Polynomial::analyze_tokens(std::vector<std::string>& tokens)
{
    std::smatch sm;
    
    for(auto tk:tokens){
        std::vector<int> nums;
        if(std::regex_search(tk, sm, doubleton_exp_)){
            std::smatch sm2;
            std::string term = sm.str();
            size_t cnt = 0, index[2];
            while(std::regex_search(term, sm2, int_exp_)){
                index[cnt]=std::stoi(sm2.str());
                term = sm2.suffix();
                ++cnt;
            }
            (*this)(index[0], index[1]) = std::stod(sm.prefix().str());
        }
        else if(std::regex_search(tk, sm, singleton_exp_)){
            std::smatch sm2;
            std::string term = sm.str();
            if(std::regex_search(term, sm2, int_exp_)){
                (*this)(0, std::stoi(sm2.str())) = std::stod(sm.prefix().str());
            }
        }
        else if(std::regex_search(tk, sm, int_exp_)){
            (*this)(0, 0) = std::stod(sm.str());
        }
    }
}

Polynomial Polynomial::first_deriv(size_t const& var)
{
    Polynomial p(dim_);
    for(size_t i=0; i<var; ++i){
        p(0, i) += (*this)(var, i);
    }
    for(size_t i=var+1; i<dim_; ++i){
        p(0, i) += (*this)(i, var);
    }
    p(0, var) = 2*(*this)(var, var);
    
    return p;
}

double Polynomial::get_element(std::pair<size_t, size_t> index)
{
    return (*this)(index.first, index.second);
}

void Polynomial::set_element(std::pair<size_t, size_t> index, const double& item)
{
    (*this)(index.first, index.second) = item;
}

std::string Polynomial::get_sympy_str()
{
    std::string str = "";

    for(size_t i=0; i<dim_; ++i){
        for(size_t j=i; j<dim_; ++j){
            double v = (*this)(i, j);
            
            if(v == 0) continue;
            
            if(v > 0 && !str.empty()) str += '+';

            if(i == 0){
                if(j == 0){
                    str += std::to_string(v);
                }
                else{
                    std::string term_str = "x"+std::to_string(j);
                    str += std::to_string(v) + '*' + term_str;
                }
            }
            else{
                std::string term_str = "x"+std::to_string(i)+"*x"+std::to_string(j);
                str += std::to_string(v) + '*' + term_str;
            }
        }
    }

    return str;
}

/* free functions */
Polynomial substitute(size_t const& var, Polynomial const& src, Polynomial const& target)
{
    size_t dim = target.dim_;
    std::vector<Polynomial> polys;
    Polynomial ret(dim);
    
    for(size_t i=0; i<var-1; ++i){
        double v = target(i, var);
        if(v != 0){
            Polynomial p(dim);
            p(i, 0) = v;
            p *= src;
            polys.emplace_back(p);
        }
    }
    
    for(size_t i=var+1; i<dim; ++i){
        double v = target(var, i);
        if(v != 0){
            Polynomial p(dim);
            p(0, i) = v;
            p *= src;
            polys.emplace_back(p);
        }
    }
    
    double v = target(var, var);
    
    if(v != 0){
        Polynomial p(dim);
        p = multiply_const(multiply_poly(src, src), v);
        polys.emplace_back(p);
    }
    
    for(auto const& p: polys){
        ret += p;
    }
    
    return ret;
}

double substitute(size_t const& var, double const& src, Polynomial const& target)
{
    double ret = 0;
    
    for(size_t i=0; i<var-1; ++i){
        double v = target(i, var);
        if(v != 0){
            ret += v*src;
        }
    }
    
    for(size_t i=var+1; i<target.dim_; ++i){
        double v = target(var, i);
        if(v != 0){
            ret += v*src;
        }
    }
    
    double v = target(var, var);
    
    if(v != 0){
        ret += v*src*src;
    }
    
    return ret;
}

Polynomial multiply_const(Polynomial const& p1, double const& c)
{
    Polynomial p(p1);
    for(int i=0; i<p.dim_; ++i){
        for(int j=0; j<p.dim_; ++j){
            p(i, j) = p(i, j)*c;
        }
    }
    
    return p;
}

Polynomial multiply_poly(Polynomial const& p1, Polynomial const& p2)
{
    size_t n = p1.dim_;
    if(n != p2.dim_) throw std::out_of_range("function dimensions are different");
    
    Polynomial ret(n);
    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            ret(i, j) += p1(0, i) * p2(0, j);
        }
    }
    
    return ret;
}

Polynomial add(Polynomial const& p1, Polynomial const& p2)
{
    size_t n = p1.dim_;
    if(n != p2.dim_) throw std::out_of_range("function dimensions are different");
    
    Polynomial ret(p1);
    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            ret(i, j) += p2(i, j);
        }
    }
    
    return ret;
}

Polynomial substract(Polynomial const& p1, Polynomial const& p2)
{
    size_t n = p1.dim_;
    if(n != p2.dim_) throw std::out_of_range("function dimensions are different");
    
    Polynomial ret(p1);
    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            ret(i, j) -= p2(i, j);
        }
    }
    
    return ret;
}
