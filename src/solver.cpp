#include "solver.hpp"

Solver::Solver(size_t nfollowers, Ftype l_ftype, Ftype f_ftype)
	:l_ftype_(l_ftype), f_ftype_(f_ftype)
{
	size_t n = nfollowers+1;
	vs_.nvars_ = n;
	vs_.nterms_ = (n+1)*(n+2)/2;
	vs_.prefix_sums_.resize(n+1, 0);
	for(int i=1; i<n+1; i++){
		vs_.prefix_sums_[i] = vs_.prefix_sums_[i-1]+(n-i+2); // starting index of each row
	}

	if(!followers_.empty()){
		followers_.clear();
		followers_.shrink_to_fit();
	}
	followers_.resize(nfollowers, new Player(&vs));
}

Solver::Solver(Solver const& other)
{
	nplayers_ = other.nplayers_;
	l_ftype_ = other.l_ftype_;
	f_ftype_ = other.f_ftype_;
	followers_ = other.followers_;
	leader_ = other.leader_;
}

Solver::Solver(Solver && other)
{
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
		std::swap(nplayers_, other.nplayers_);
		std::swap(l_ftype_, other.l_ftype_);
		std::swap(f_ftype_, other.f_ftype_);
		std::swap(leader_, other.leader_);
		followers_ = std::move(other.followers_);
	}

	return *this;
}

bool Solver::judge_concavity(Role role)
{
	bool flag = false;

	switch(role){
		case leader:
			break;
		case follower:
			break;
		default:
			break;
	}

	return flag;
}

std::vector<Polynomial> Solver::solve_followers()
{

}

float Solver::solve_leader()
{

}
