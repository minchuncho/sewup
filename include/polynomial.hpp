//
//  polynomial.hpp
//  sewup-xcode
//
//  Created by MinChunCho on 2022/5/8.
//

#ifndef polynomial_hpp
#define polynomial_hpp

#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <map>
#include <regex>
#include <cstdlib>

const size_t DIM_MAX = 1001;

class Polynomial{
public:
    Polynomial() = delete;
    Polynomial(size_t const& dim);
    Polynomial(size_t const& dim, std::string const& func);
    Polynomial(Polynomial const& other);
    Polynomial(Polynomial&& other);
    Polynomial& operator=(Polynomial const& other);
    Polynomial& operator=(Polynomial&& other);
    ~Polynomial() = default;
    
    void operator=(std::string const& func);
    double& operator()(size_t const& row, size_t const& col);
    double const& operator()(size_t const& row, size_t const& col) const;
    void operator+=(Polynomial const& p);
    void operator-=(Polynomial const& p);
    void operator*=(double const& c);
    void operator*=(Polynomial const& p);
    bool operator==(Polynomial const& p) const;
    
    friend Polynomial multiply_const(Polynomial const& p1, double const& c);
    friend Polynomial multiply_poly(Polynomial const& p1, Polynomial const& p2);
    friend Polynomial add(Polynomial const& p1, Polynomial const& p2);
    friend Polynomial substract(Polynomial const& p1, Polynomial const& p2);
    friend Polynomial substitute(size_t const& var, Polynomial const& src, Polynomial const& target);
    friend double substitute(size_t const& var, double const& src, Polynomial const& target);
    
    Polynomial first_deriv(size_t const& var);
    double get_element(std::pair<size_t, size_t> index);
    void set_element(std::pair<size_t, size_t> index, const double& item);
    size_t const& dim() const { return dim_; }
    std::string get_sympy_str();
    
private:
    static std::regex make_regexp(std::string const& regexp);
    void make_starting_index();
    std::vector<std::string> get_tokens(std::string str);
    void analyze_tokens(std::vector<std::string>& tokens);
    
    static std::regex token_exp_;
    size_t dim_;
    std::vector<size_t> starting_index_;
    std::vector<double> terms_;   // works differently from Matrix so don't equalize 'em
};

Polynomial multiply_const(Polynomial const& p1, double const& c);
Polynomial multiply_poly(Polynomial const& p1, Polynomial const& p2);
Polynomial add(Polynomial const& p1, Polynomial const& p2);
Polynomial substract(Polynomial const& p1, Polynomial const& p2);
Polynomial substitute(size_t const& var, Polynomial const& src, Polynomial const& target);
double substitute(size_t const& var, double const& src, Polynomial const& target);

#endif /* polynomial_hpp */
