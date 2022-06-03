//
//  polynomial.cpp
//  sewup-xcode
//
//  Created by MinChunCho on 2022/5/8.
//

#include "polynomial.hpp"

std::regex Polynomial::token_exp_ = make_regexp("-?(([0-9]*[.])?[0-9]+)?\\*?((x)([1-9][0-9]*))?((x)([1-9][0-9]*))?");

Polynomial::Polynomial(size_t const& dim)
    : dim_(dim), starting_index_(dim, 0), terms_(dim*(dim+1)/2, 0)
{
    make_starting_index();
}

Polynomial::Polynomial(size_t const& dim, std::string const& func)
    : dim_(dim), starting_index_(dim, 0), terms_(dim*(dim+1)/2, 0)
{
    make_starting_index();
    *this = func;
}

Polynomial::Polynomial(Polynomial const& other)
    : dim_(other.dim_), starting_index_(other.starting_index_), terms_(other.terms_) {}

Polynomial::Polynomial(Polynomial&& other)
    : dim_(std::move(other.dim_)), starting_index_(std::move(other.starting_index_)), terms_(std::move(other.terms_)) {}

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
    
    size_t index = starting_index_[std::min(row, col)] + abs((int)(row-col));
    return terms_[index];
}

double const& Polynomial::operator()(size_t const& row, size_t const& col) const
{
    if(row >= dim_ || col >= dim_){
        throw std::out_of_range("row or col is larger than the dimension");
    }
    
    size_t index = starting_index_[std::min(row, col)] + abs((int)(row-col));
    return terms_[index];
}

void Polynomial::operator+=(Polynomial const& p)
{
    for(int i=0; i<terms_.size(); ++i){
        terms_[i] += p.terms_[i];
    }
}

void Polynomial::operator-=(Polynomial const& p)
{
    for(int i=0; i<terms_.size(); ++i){
        terms_[i] -= p.terms_[i];
    }
}

void Polynomial::operator*=(double const& c)
{
    for(int i=0; i<dim_; ++i){
        for(int j=i; j<dim_; ++j){
            (*this)(i, j) *= c;
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
            ret(i, j) += (*this)(0, i) * p(0, j);
        }
    }

    (*this) = ret;
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

void Polynomial::make_starting_index()
{
    for(int i=1; i<dim_; ++i){
        starting_index_[i] = starting_index_[i-1] + (dim_-i+1);
    }
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
    
    std::string token = str.substr(s, e-s);
    if(!std::regex_match(token, sm, token_exp_)){
        throw std::invalid_argument("wrong function format");
    }
    tokens.emplace_back(token); // the last substring
    
    return tokens;
}

void Polynomial::analyze_tokens(std::vector<std::string>& tokens)
{
    std::smatch sm;
    
    for(auto tk:tokens){
        std::vector<size_t> indices;
        
        size_t pos;
        double cof = 0;
        
        if((pos = tk.find('*')) == std::string::npos){
            if((pos = tk.find('x')) == std::string::npos){ // constant
                (*this)(0, 0) = std::stod(tk);
                continue;
            }
            else{
                cof = 1;
            }
        }
        else{
            cof  = std::stod(tk.substr(0, pos));
            tk.erase(0, pos+2);
        }
        
        std::string idx = "";
        
        while((pos = tk.find('x')) != std::string::npos){
            idx  = tk.substr(0, pos);
            indices.emplace_back(std::stoi(idx));
            tk.erase(0, pos+1);
        }
        
        if(!tk.empty()) indices.emplace_back(std::stoi(tk));
        
        size_t n = indices.size();
        if(n == 1) (*this)(0, indices[0]) += cof;
        else if(n == 2) (*this)(indices[0], indices[1]) += cof;
        else throw std::invalid_argument("wrong format of function");
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
    size_t n = target.dim_;
    Polynomial ret(n), dup(target);
    
    for(size_t i=0; i<var; ++i){
        double v = target(i, var);
        if(v != 0){
            dup(i, var) = 0;
            ret(0, i) += v;
        }
    }
    
    for(size_t i=var+1; i<n; ++i){
        double v = target(var, i);
        if(v != 0){
            dup(i, var) = 0;
            ret(0, i) += v;
        }
    }
    
    ret *= src;
    
    double v = target(var, var);
    
    if(v != 0){
        dup(var, var) = 0;
        ret += multiply(multiply(src, src), v);
    }

    ret += dup;

    return ret;
}

Polynomial substitute(size_t const& var, double const& src, Polynomial const& target)
{
    size_t n = target.dim_;
    Polynomial ret(n), dup(target);
    
    for(size_t i=0; i<var; ++i){
        double v = target(i, var);
        if(v != 0){
            dup(i, var) = 0;
            ret(0, i) += v;
        }
    }
    
    for(size_t i=var+1; i<n; ++i){
        double v = target(var, i);
        if(v != 0){
            dup(var, i) = 0;
            ret(0, i) += v;
        }
    }
    
    ret *= src;
    
    double v = target(var, var);
    
    if(v != 0){
        dup(var, var) = 0;
        ret(0, 0) = src * src * v;
    }

    ret += dup;

    return ret;
}

Polynomial multiply(Polynomial const& p, double const& c)
{
    Polynomial ret(p);
    ret *= c;
    
    return ret;
}

Polynomial multiply(Polynomial const& p1, Polynomial const& p2)
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
    ret += p2;
    
    return ret;
}

Polynomial substract(Polynomial const& p1, Polynomial const& p2)
{
    size_t n = p1.dim_;
    if(n != p2.dim_) throw std::out_of_range("function dimensions are different");
    
    Polynomial ret(p1);
    ret -= p2;
    
    return ret;
}
