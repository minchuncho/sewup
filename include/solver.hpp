#include <vector>

#include "player.hpp"
#include "polynomial.hpp"

typedef enum { utility, cost } Ftype;
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

	void solve();
private:
	void solve_followers();
	void solve_leaders();
	void substitute_leaders();
	void substitute_followers();

	Ftype l_ftype_;						// leader function type
	Ftype f_ftype_;						// follower funciton type
	std::vector<Player> followers_;		// followers 
	std::vector<Player> leaders_;		// leaders, so far only one is allowed
	bool l_valid_;						// if the leader function is concave/convex as expected, avoiding repeated calculation
	bool f_valid_;						// if the follower function is concave/convex as expected, avoiding repeated calculation
	size_t nplayers_;					// number of players: followers and one leader
	size_t nf_;							// number of followers
	size_t nl_;							// number of leaders

	std::vector<float><float> f_eq_system_;		// a ((nf_+1)*(nf_+1)) matrix
	std::vector<Polynomial> f_const_system_;	// an amount of nf_ ((nl_+1)*(nl_+1)) matrix

	std::vector<float><float> l_eq_system_;		// a ((nf_+1)*(nf_+1)) matrix
	std::vector<float> l_const_system_;	// an amount of nf_ ((nl_+1)*(nl_+1)) matrix
};