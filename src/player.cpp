//
//  player.cpp
//  sewup-xcode
//
//  Created by MinChunCho on 2022/5/9.
//

#include "player.hpp"

Player::Player(size_t dim, size_t nvar)
    : func_(dim), psol_(dim), hessian_(nvar, nvar) {}

Player::Player(Player const& other)
    : var_(other.var_), sol_(other.sol_), dfness_(other.dfness_), func_(other.func_), psol_(other.psol_), hessian_(other.hessian_), first_derivs_(other.first_derivs_) {}

Player::Player(Player && other)
    : var_(std::move(other.var_)), sol_(std::move(other.sol_)), dfness_(std::move(other.dfness_)), func_(std::move(other.func_)), psol_(std::move(other.psol_)), hessian_(std::move(other.hessian_)), first_derivs_(std::move(other.first_derivs_)) {}

Player& Player::operator=(Player const& other)
{
    if(this != &other){
        var_ = other.var_;
        func_ = other.func_;
        psol_ = other.psol_;
        hessian_ = other.hessian_;
        first_derivs_ = other.first_derivs_;
        dfness_ = other.dfness_;
        sol_ = other.sol_;
    }
    
    return *this;
}

Player& Player::operator=(Player && other)
{
    if(this != &other){
        std::swap(var_, other.var_);
        std::swap(func_, other.func_);
        std::swap(psol_, other.psol_);
        std::swap(hessian_, other.hessian_);
        std::swap(first_derivs_, other.first_derivs_);
        std::swap(dfness_, other.dfness_);
        std::swap(sol_, other.sol_);
    }
    
    return *this;
}

Player& Player::operator=(std::string const& func)
{
    func_ = func;
    return *this;
}

bool Player::is_valid(Ftype const& ftype, size_t var_s, size_t var_e, Role role)
{
    calc_hessian_mat(var_s, var_e, role);
    dfness_ = (Dtype)ftype;
    return hessian_.dfness(dfness_);
}

void Player::calc_hessian_mat(size_t var_s, size_t var_e, Role role)
{
    Polynomial* ptr;
    if(role == leader) ptr = &psol();
    else ptr = &func();
    
    for(size_t i=var_s; i<=var_e; ++i){
        if(first_derivs_.find(i) == first_derivs_.end()){
            first_derivs_.at(i) = ptr->first_deriv(i);
        }
        for(size_t j=var_s; j<=var_e; ++j){
            hessian_(i-var_s, j-var_s) += first_derivs_.at(i).first_deriv(j)(0, 0);
        }
    }
}
