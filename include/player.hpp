#include <vector>

#include "polynomial.hpp"

class Player {
public:
	Player();
	Player(Player const&);
	Player(Player &&);
	~Player();

	Player& operator=(Player const&);
	Player& operator=(Player &&);

	void substitute(std::vector<Polynomial>);
	void substitute(float);
private:
	size_t var_idx_;
	Polynomial sol_;
	Polynomial func_;
};