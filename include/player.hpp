#include <vector>

#include "polynomial.hpp"

class Player {
public:
	Player() = delete;
	Player(VarsStrust const * const vs);
	Player(Player const& other);
	Player(Player && other);
	~Player() = default;
	Player& operator=(Player const& other);
	Player& operator=(Player && other);

	Player& operator=(std::string const& func);
	void substitute(std::vector<Polynomial>);
	void substitute(float);
private:
	Polynomial sol_;
	Polynomial func_;
	Polynomial first_deriv_;
	float second_deriv_;
};