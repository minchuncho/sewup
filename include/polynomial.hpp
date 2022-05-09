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
#include <map>
#include <regex>

const size_t DIM_MAX = 1001;

class Polynomial{
public:
    Polynomial() = delete;
    Polynomial(size_t const& dim);
    Polynomial(Polynomial const& other);
    Polynomial(Polynomial&& other);
    Polynomial& operator=(Polynomial const& other);
    Polynomial& operator=(Polynomial&& other);
    ~Polynomial() = default;
    
    double& operator()(size_t const& row, size_t const& col);
    double const& operator()(size_t const& row, size_t const& col) const;
    void operator+=(Polynomial const& p);
    void operator*=(double const& c);
    void operator*=(Polynomial const& p);
    friend Polynomial operator*(Polynomial const& p1, double const& c);
    friend Polynomial operator*(Polynomial const& p1, Polynomial const& p2);
    friend Polynomial operator+(Polynomial const& p1, Polynomial const& p2);
    friend Polynomial operator-(Polynomial const& p1, Polynomial const& p2);
    
    void substitute(size_t const& var, Polynomial const& src, Polynomial& dest);
    void substitute(size_t const& var, double const& src, double& dest);
    Polynomial first_deriv(size_t const& var);
    
private:
    static std::regex make_regexp(std::string const& regexp);
    static std::vector<size_t> make_starting_index();
    std::vector<std::string> get_tokens(std::string str);
    void analyze_tokens(std::vector<std::string>& tokens);
    
    static std::regex token_exp_;
    static std::regex int_exp_;
    static std::regex singleton_exp_;
    static std::regex doubleton_exp_;
    static std::vector<size_t> starting_index_;
    size_t dim_;
    std::vector<double> mat_;   // different from Matrix so don't equalize 'em
};

Polynomial operator*(Polynomial const& p1, double const& c);
Polynomial operator*(Polynomial const& p1, Polynomial const& p2);
Polynomial operator+(Polynomial const& p1, Polynomial const& p2);
Polynomial operator-(Polynomial const& p1, Polynomial const& p2);

#endif /* polynomial_hpp */
