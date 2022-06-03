//
//  player.hpp
//  sewup-xcode
//
//  Created by MinChunCho on 2022/5/9.
//

#ifndef player_hpp
#define player_hpp

#include <vector>
#include <map>

#include "polynomial.hpp"
#include "matrix.hpp"

typedef enum { utility, cost } Ftype;
enum class Role { follower, leader };

class Player{
public:
    Player() = delete;
    Player(size_t dim, size_t nvar);
    Player(Player const& other);
    Player(Player && other);
    Player& operator=(Player const& other);
    Player& operator=(Player && other);
    ~Player() = default;

    Player& operator=(std::string const& func);

    size_t const& var() const { return var_; }
    size_t& var() { return var_; }
    Polynomial const& func() const { return func_; }
    Polynomial& func() { return func_; }
    Polynomial const& psol() const { return psol_; }
    Polynomial& psol() { return psol_; }
    double const& sol() const { return sol_; }
    double& sol() { return sol_; }
    Polynomial const& first_deriv(size_t var) const { return first_derivs_.at(var); }
    Polynomial& first_deriv(size_t var) { return first_derivs_.at(var); }
    bool is_valid(Ftype const& ftype, size_t var_s, size_t var_e, Role role);

private:
    void calc_hessian_mat(size_t var_s, size_t var_e, Role role);
    void hessian_mat_dfness();
    
    size_t var_;
    double sol_;
    Dtype dfness_;
    Polynomial func_;
    Polynomial psol_;
    Matrix hessian_;
    std::map<size_t, Polynomial> first_derivs_;
};

#endif /* player_hpp */
