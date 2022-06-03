//
//  solver.cpp
//  sewup-xcode
//
//  Created by MinChunCho on 2022/5/9.
//

#include "solver.hpp"

Solver::Solver(size_t nf, size_t nl, Ftype f_ftype, Ftype l_ftype)
    : nf_(nf), nl_(nl), f_ftype_(f_ftype), l_ftype_(l_ftype),
        followers_(nf, Player(nf+nl+1, nf)), leaders_(nl, Player(nf+nl+1, nl)),
        f_eq_system_(nf, nf), f_const_system_(nf, Polynomial(nf+nl+1)),
        l_eq_system_(nl, nl), l_const_system_(nl, 1)
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
    followers_(other.followers_), leaders_(other.leaders_),
        f_eq_system_(other.f_eq_system_), f_const_system_(other.f_const_system_),
        l_eq_system_(other.l_eq_system_), l_const_system_(other.l_const_system_) {}

Solver::Solver(Solver && other)
    : nf_(std::move(other.nf_)), nl_(std::move(other.nl_)), f_ftype_(std::move(other.f_ftype_)), l_ftype_(std::move(other.l_ftype_)),
        followers_(std::move(other.followers_)), leaders_(std::move(other.leaders_)),
        f_eq_system_(std::move(other.f_eq_system_)), f_const_system_(std::move(other.f_const_system_)),
        l_eq_system_(std::move(other.l_eq_system_)), l_const_system_(std::move(other.l_const_system_)) {}

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
        if(!f.is_valid(f_ftype_, 1, nf_, Role::follower)){
            throw std::runtime_error("[follower] hessain matrix is invalid");
        }
        
        Polynomial fderiv = f.first_deriv(f.var());
        for(int j=1; j<=nf_; ++j){
            f_eq_system_(i, j-1) = fderiv(0, j);
            fderiv(0, j) = 0;
        }
        f_const_system_[i] = (multiply(fderiv, -1));
    }
    
    Matrix inv = f_eq_system_.inverse();
    for(int i=0; i<nf_; ++i){
        Polynomial v(nf_+nl_+1);
        for(int j=0; j<nf_; ++j){
            v +=  multiply(f_const_system_[j], inv(i, j));
        }
        followers_[i].psol() = v;
    }
    
    substitute_to_leaders();
}

void Solver::solve_leaders()
{
    for(int i=0; i<nl_; ++i){
        Player l = leaders_[i];
        if(!l.is_valid(l_ftype_, nf_+1, nf_+nl_, Role::leader)){
            throw std::runtime_error("[leader] hessain matrix is invalid");
        }
        
        Polynomial fderiv = l.first_deriv(l.var());
        for(int j=0; j<nl_; ++j){
            size_t lj = j+nf_+1;
            l_eq_system_(i, j) = fderiv(0, lj);
            fderiv(0, lj) = 0;
        }
        l_const_system_(0, i) = ((-1)*fderiv(0, 0));
    }
    
    Matrix inv = l_eq_system_.inverse();
    Matrix ret = multiply_naive(inv, l_const_system_);
    
    for(int i=0; i<nl_; ++i){
        double val = ret(i, 0);
        leaders_[i].sol() = val;
        lsols_.push_back(val);
    }
    
    substitute_to_followers();
}

void Solver::substitute_to_followers()
{
    for(int i=0; i<nf_; ++i){
        Polynomial init = followers_[i].psol();
        
        for(int j=0; j<nl_; ++j){
            init = substitute(leaders_[j].var(), leaders_[j].sol(), init);
        }

        double val = init(0, 0);
        followers_[i].sol() = val;
        fsols_.push_back(val);
    }
}

void Solver::substitute_to_leaders()
{
    for(int i=0; i<nl_; ++i){
        Polynomial init = leaders_[i].func();
        for(int j=0; j<nf_; ++j){
            init = substitute(followers_[j].var(), followers_[j].psol(), init);
        }
        leaders_[i].psol() = init;
    }
}

void Solver::show_ans()
{
    std::cout << (*this);
}

Player& Solver::set_leader(size_t const& i, std::string const& func)
{
    leaders_[i] = func;
    return leaders_[i];
}

Player& Solver::set_follower(size_t const& i, std::string const& func)
{
    followers_[i] = func;
    return followers_[i];
}

py::array_t<double> Solver::fsols()
{
    return py::array_t(
        { nf_ },
        { sizeof(double) },
        fsols_.data(),
        py::cast(this)
    );
}

py::array_t<double> Solver::lsols()
{
    return py::array_t(
        { nl_ },
        { sizeof(double) },
        lsols_.data(),
        py::cast(this)
    );
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
