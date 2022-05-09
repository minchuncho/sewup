//
//  solver.cpp
//  sewup-xcode
//
//  Created by MinChunCho on 2022/5/9.
//

#include "solver.hpp"

Solver::Solver(size_t nf, size_t nl, Ftype f_ftype, Ftype l_ftype)
    : nf_(nf), nl_(nl), f_ftype_(f_ftype), l_ftype_(l_ftype),
        f_eq_system_(nf, nf), f_const_system_(nf, Polynomial(nf+nl+1)),
        l_eq_system_(nl, nl), l_const_system_(nl, 0),
        followers_(nf, Player(nf+nl+1, nf)), leaders_(nl, Player(nf+nl+1, nl))
{
    size_t cnt = 1;
    for(int i=0; i<nf_; ++i){
        followers_[i].var() = cnt;
        ++cnt;
    }
    for(int i=0; i<nl_; ++i){
        leaders_[i].var() = cnt;
        ++cnt;
    }
}

Solver::Solver(Solver const& other)
    : nf_(other.nf_), nl_(other.nl_), f_ftype_(other.f_ftype_), l_ftype_(other.l_ftype_),
        f_eq_system_(other.f_eq_system_), f_const_system_(other.f_const_system_),
        l_eq_system_(other.l_eq_system_), l_const_system_(other.l_const_system_),
        followers_(other.followers_), leaders_(other.leaders_) {}

Solver::Solver(Solver && other)
    : nf_(std::move(other.nf_)), nl_(std::move(other.nl_)), f_ftype_(std::move(other.f_ftype_)), l_ftype_(std::move(other.l_ftype_)),
        f_eq_system_(std::move(other.f_eq_system_)), f_const_system_(std::move(other.f_const_system_)),
        l_eq_system_(std::move(other.l_eq_system_)), l_const_system_(std::move(other.l_const_system_)),
        followers_(std::move(other.followers_)), leaders_(std::move(other.leaders_)) {}

Solver& Solver::operator=(Solver const& other)
{
    if(this != &other){
        nf_ = other.nf_;
        nl_ = other.nl_;
        f_ftype_ = other.f_ftype_;
        l_ftype_ = other.l_ftype_;
        f_eq_system_ = other.f_eq_system_;
        f_const_system_ = other.f_const_system_;
        l_eq_system_ = other.l_eq_system_;
        l_const_system_ = other.l_const_system_;
        followers_ = other.followers_;
        leaders_ = other.leaders_;
    }
    
    return *this;
}

Solver& Solver::operator=(Solver && other)
{
    if(this != &other){
        std::swap(nf_, other.nf_);
        std::swap(nl_, other.nl_);
        std::swap(f_ftype_, other.f_ftype_);
        std::swap(l_ftype_, other.l_ftype_);
        std::swap(f_eq_system_, other.f_eq_system_);
        std::swap(f_const_system_, other.f_const_system_);
        std::swap(l_eq_system_, other.l_eq_system_);
        std::swap(l_const_system_, other.l_const_system_);
        std::swap(followers_, other.followers_);
        std::swap(leaders_, other.leaders_);
    }
    
    return *this;
}

void Solver::solve_followers()
{
    for(int i=0; i<nf_; ++i){
        Player f = followers_[i];
        if(!f.is_valid(f_ftype_, 1, nf_, follower)){
            throw std::runtime_error("[follower] hessain matrix is invalid");
        }
        
        Polynomial fderiv = f.first_deriv(f.var());
        for(int j=1; j<=nf_; ++j){
            f_eq_system_(i, j-1) = fderiv(0, j);
            f_eq_system_(i, j-1) = 0;
        }
        f_const_system_.emplace_back(fderiv*(-1.0));
    }
    
    Matrix inv = f_eq_system_.inverse();
    for(int i=0; i<nf_; ++i){
        Polynomial v(nf_+nl_+1);
        for(int j=0; j<nf_; ++j){
            v +=  f_const_system_[j] * inv(i, j);
        }
        followers_[i].psol() = v;
    }
    
    substitute_to_leaders();
}

void Solver::solve_leaders()
{
    for(int i=0; i<nl_; ++i){
        Player l = leaders_[i];
        if(!l.is_valid(l_ftype_, nf_+1, nf_+nl_, leader)){
            throw std::runtime_error("[leader] hessain matrix is invalid");
        }
        
        Polynomial fderiv = l.first_deriv(l.var());
        for(int j=1; j<=nf_; ++j){
            l_eq_system_(i, j-1) = fderiv(0, j);
            l_eq_system_(i, j-1) = 0;
        }
        l_const_system_(0, i) = ((-1)*fderiv(0, 0));
    }
    
    Matrix inv = l_eq_system_.inverse();
    Matrix res = multiply_tile(inv, l_const_system_, 32);
    
    for(int i=0; i<nl_; ++i){
        leaders_[i].sol() *= res(i, 0);
    }
    
    substitute_to_followers();
}

void Solver::substitute_to_followers()
{
    for(int i=0; i<nf_; ++i){
        double dest=0;
        for(int j=0; j<nl_; ++j){
            double src = leaders_[j].sol();
            size_t var = followers_[j].var();
            followers_[i].psol().substitute(var, src, dest);
        }
        followers_[i].sol() = dest;
    }
}

void Solver::substitute_to_leaders()
{
    size_t dim = nf_+nl_+1;
    for(int i=0; i<nl_; ++i){
        Polynomial dest(dim);
        for(int j=0; j<nf_; ++j){
            Polynomial src = followers_[j].psol();
            size_t var = followers_[j].var();
            leaders_[i].psol().substitute(var, src, dest);
        }
        leaders_[i].psol() = dest;
    }
}

void Solver::show_ans()
{
    std::cout << (*this);
}

// stand-alone function
std::ostream& operator<<(std::ostream& os, Solver solver)
{
    std::cout << "followers' answers:\n";
    for(auto const& f:solver.followers_){
        std::cout << f.sol() << ", ";
    }
    std::cout << std::endl;
    std::cout << "leaders' answers:\n";
    for(auto const& l:solver.leaders_){
        std::cout << l.sol() << ", ";
    }
    return os;
}
