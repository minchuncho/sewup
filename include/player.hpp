#include <vector>

#include "polynomial.hpp"

typedef enum { negative, positive, illegal, none } Dtype;

class Player {
public:
	Player() = delete;
	Player(size_t var_idx);
	Player(Player const& other);
	Player(Player && other);
	~Player() = default;
	Player& operator=(Player const& other);
	Player& operator=(Player && other);

	Player& operator=(std::string const& func);

	Polynomial const& func() const { return func_; }
	Polynomial& func() { return func_; }
	Polynomial const& sol() const { return sol_; }
	Polynomial& sol() { return sol_; }
	Polynomial const& var_idx() const { return var_idx_; }

	Dtype& hessian_dfness();
	friend void judge_hessian_dfness(Player player);
private:
	void make_hessian();

	size_t var_idx_;
	Polynomial sol_;			// ((nl_+1)*(nl_+1)) matrix
	Polynomial func_;			// ((nl_+nf_+1)*(nl_+nf_+1)) matrix
	Dtype dfness_;
	std::vector<float><float> hessian_;
	std::map<int><Polynomial> first_derivs_;		// keep this function's first deriv regarding each variable
};

void judge_hessian_dfness(Player player);