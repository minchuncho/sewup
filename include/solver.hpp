#include <vector>

#include "Solver.hpp"

typedef enum { utility, cost } Ftype;
typedef enum { positive, negative } Dtype;
typedef enum { leader, follower } Role;

class Solver {
public:
	Solver();
	Solver(size_t nfollowers, Ftype l_ftype, Ftype f_ftype);
	Solver(Solver const& other);
	Solver(Solver && other);
	~Solver();

	Solver& operator=(Solver const& other);
	Solver& operator=(Solver && other);

	std::string judge_concavity(Role role);
	std::vector<Polynomial> solve_followers();
	float solve_leader();
private:
	size_t nplayers_;
	Ftype l_ftype_;
	Ftype f_ftype_;
	std::vector<Player> followers_;
	Player leader_;
};