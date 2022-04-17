#include <vector>

#include "player.hpp"
#include "polynomial.hpp"

typedef enum { utility, cost } Ftype;
typedef enum { positive, negative } Dtype;
typedef enum { leader, follower } Role;

class Solver {
public:
	Solver() = default;
	Solver(size_t nfollowers, Ftype l_ftype, Ftype f_ftype);
	Solver(Solver const& other);
	Solver(Solver && other);
	~Solver();

	Solver& operator=(Solver const& other);
	Solver& operator=(Solver && other);

	bool judge_concavity(Role role);
	std::vector<Polynomial> solve_followers();
	float solve_leader();
private:
	Ftype l_ftype_;						// leader function type
	Ftype f_ftype_;						// follower funciton type
	std::vector<Player> followers_;		// followers 
	Player leader_;						// leader
	bool l_valid_;						// if the leader function is concave/convex as expected, avoiding repeated calculation
	bool f_valid_;						// if the follower function is concave/convex as expected, avoiding repeated calculation
	VarStruct vs_;
};