#include "player.hpp"

Player::Player(VarsStrust const * const vs)
	:sol_(vs), func_(vs), first_deriv_(vs)
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

void Player::substitute(std::vector<Polynomial> polys)
{

}

void Player::substitute(float)
{

}
