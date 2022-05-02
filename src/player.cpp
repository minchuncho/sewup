#include "player.hpp"

Player::Player(size_t var_idx)
	:var_idx_(var_idx)
{
	
}

Player::Player(Player const& other)
{
	func_ = other.func_;
	sol_ = other.sol_;
}

Player::Player(Player && other)
{
	std::swap(func_, other.func_);
	std::swap(sol_, other.sol_);
}

Player& Player::operator=(Player const& other)
{
	if(this != &other){
		func_ = other.func_;
		sol_ = other.sol_;
	}

	return *this;
}

Player& Player::operator=(Player && other)
{
	if(this != &other){
		std::swap(func_, other.func_);
		std::swap(sol_, other.sol_);
	}

	return *this;
}

void Player::make_hessian()
{
	size_t nvars = this->func().nvars();
	for(int i=1; i<nvars; ++i){
		for(int j=1; j<nvars; ++j){
			this->hessian_[i][j] = this->func().second_derivative(i, j);
		}
	}
}

Dtype& Player::hessian_dfness()
{
	make_hessian();
	return judge_hessian(this->hessian_);
}

// free function
void judge_hessian_dfness(std::vector<float><float> hessian)
{
	// set dfness_
}
