#include "solver.hpp"

Solver::Solver(size_t nfollowers, Ftype l_ftype, Ftype f_ftype)
	:l_ftype_(l_ftype), f_ftype_(f_ftype), nf_(nfollowers), nl_(1), nplayers_(nfollowers+1)
{
	if(!followers_.empty()){
		followers_.clear();
		followers_.shrink_to_fit();
	}
	followers_.resize(nfollowers, new Player(&vs));
}

Solver::Solver(Solver const& other)
{
	nf_ = other.nf_;
	nl_ = other.nl_;
	nplayers_ = other.nplayers_;
	l_ftype_ = other.l_ftype_;
	f_ftype_ = other.f_ftype_;
	followers_ = other.followers_;
	leader_ = other.leader_;
}

Solver::Solver(Solver && other)
{
	std::swap(nf_, other.nf_);
	std::swap(nl_, other.nl_);
	std::swap(nplayers_, other.nplayers_);
	std::swap(l_ftype_, other.l_ftype_);
	std::swap(f_ftype_, other.f_ftype_);
	std::swap(leader_, other.leader_);
	followers_ = std::move(other.followers_);
}

Solver::~Solver()
{
	followers_.clear();
	followers_.shrink_to_fit();
}

Solver& Solver::operator=(Solver const& other)
{
	if(this != &other){
		nf_ = other.nf_;
		nl_ = other.nl_;
		nplayers_ = other.nplayers_;
		l_ftype_ = other.l_ftype_;
		f_ftype_ = other.f_ftype_;
		followers_ = other.followers_;
		leader_ = other.leader_;
	}

	return *this;
}

Solver& Solver::operator=(Solver && other)
{
	if(this != &other){
		std::swap(nf_, other.nf_);
		std::swap(nl_, other.nl_);
		std::swap(nplayers_, other.nplayers_);
		std::swap(l_ftype_, other.l_ftype_);
		std::swap(f_ftype_, other.f_ftype_);
		std::swap(leader_, other.leader_);
		followers_ = std::move(other.followers_);
	}

	return *this;
}

void Solver::solve()
{
	for(int i=0; i<nf_; ++i){
		if(followers_[i].hessian_dfness() != f_ftype_) throw std::exception("follower: function type doesn't match definiteness.");
	}

	solve_followers();

	for(int i=0; i<nl_; ++i){
		if(leaders_[i].hessian_dfness() != l_ftype_) throw std::exception("leader: function type doesn't match definiteness.");
	}

	solve_leader();
}

void Solver::solve_followers()
{
	for(int i=0; i<nf_; ++i){
		Player cur = followers_[i];
		f_eq_system_.push_back(std::vector<float>(nf_, 0));
		Polynomial fderiv = cur.first_derivs_[cur.var_idx_], fconst(nl_);
		
		// const
		fconst[0][0]=fderiv[0][0];
		
		// followers' vars
		size_t j=1;
		for(; j<=nf_; ++j){
			f_eq_system_[i][j] = fderiv[0][j];
		}
		
		// leaders' vars
		for(; j<=nplayers_; ++j){
			fconst[0][j-nf_] = fderiv[0][j-nf_];
		}

		f_const_system_.push_back(fconst);
	}

	// todos:
	// inverse matrix
	// constant * polynomial = polynomial -> follower's solution
	// substitute solution to the leaders' functions
}

void Solver::solve_leaders()
{
	for(int i=0; i<nl_; ++i){
		Player cur = leaders_[i];
		l_eq_system_.push_back(std::vector<float>(nl_, 0));
		Polynomial fderiv = cur.first_derivs_[cur.var_idx_], fconst(nl_);
		
		// const
		l_const_system_.push_back(fderiv[0][0]);
		
		// leaders' vars
		size_t j=1;
		for(; j<=nl_; ++j){
			l_eq_system_[i][j] = fderiv[0][j];
		}

		// todos:
		// inverse matrix
		// constant * polynomial = polynomial -> follower's solution
		// substitute solution to the followers' functions
	}


	float sderiv = leader_.sol().second_derivative(leader_.var_idx(), leader_.var_idx());
	if((sderiv > 0 && l_ftype_ == cost)
		|| (sderiv < 0 && l_ftype_ == utility))
	{
		// solve first derivative
		Polynomial fderiv = leader_.sol().first_derivative(leader_.var_idx());
		leader_.sol()[0][0] = fderiv[0][0]/fderiv[0][1];
	}
	else throw std::exception("no extremum exists");
}

void Solver::substitute_followers()
{
	
}

void Solver::substitute_leaders()
{
	for(int j=1; i<=nl_; ++j){
		for(int i=1; i<=nf_; ++i){
			Polynomial res(nl_);
			res[0][j] = leader_.func()[i][j+nf_];
			leader_.sol() += res * followers_[i].sol();
		}
	}
}
