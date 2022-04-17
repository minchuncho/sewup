#include "polynomial.hpp"

Polynomial::Polynomial(VarsStrust const * const vs_ptr)
{
	vs_ptr_ = vs_ptr;
	if(!terms_.empty){
		terms_.clear();
		terms_.shrink_to_fit();
	}
	terms_.resize(vs_ptr_->nterms_, 0);
}

Polynomial::Polynomial(Polynomial const& other)
{
	terms_ = other.terms_;
	vs_ptr_ = other.vs_ptr_;
}

Polynomial::Polynomial(Polynomial && other)
{
	terms_ = std::move(other.terms_);
	std::swap(vs_ptr_, other.vs_ptr_);
}

Polynomial::~Polynomial()
{
	terms_.clear();
	terms_.shrink_to_fit();
}

Polynomial& Polynomial::operator=(Polynomial const& other)
{
	if(this != &other){
		terms_ = other.terms_;
		vs_ptr_ = other.vs_ptr_;
	}

	return *this;
}

Polynomial& Polynomial::operator=(Polynomial && other)
{
	if(this != &other){
		terms_ = std::move(other.terms_);
		std::swap(vs_ptr_, other.vs_ptr_);
	}

	return *this;
}

Polynomial& Polynomial::operator=(std::string func_str)
{
	// example: 1 + x_3 + 2*x_5 + x_2*x_3 + p*x_7
	std::vector<std::string> tokens;
	str::string token = "";
	for(int i=0; i<func_str.length(); ++i){
		char c = str[i];
		if(c == '+' || c == '-'){
			for(int j=0; j<token.length(); ++j){

			}
			token = "";
		}
		else{
			token += c;
		}
	}
}

Polynomial Polynomial::operator+(Polynomial const& pa, Polynomial const& pb)
{
	check_vars_match(pa, pb);

	Polynomial new_poly(pa.vs_ptr_);
	for(int i=0; i<terms_.size(); ++i){
		new_poly.terms_[i] = pa.terms_[i]+pb.terms_[i];
	}
}

Polynomial Polynomial::operator-(Polynomial const& pa, Polynomial const& pb)
{
	check_vars_match(pa, pb);

	Polynomial new_poly(pa.vs_ptr_);
	for(int i=0; i<terms_.size(); ++i){
		new_poly.terms_[i] = pa.terms_[i]-pb.terms_[i];
	}
}

Polynomial Polynomial::operator*(Polynomial const& pa, Polynomial const& pb)
{
	check_vars_match(pa, pb);

	Polynomial new_poly(pa.vs_ptr_);
	for(int i=0; i<=nvars; ++i){
		for(int j=0; j<=nvars; ++j){
			if(i>j){
				term(i, j) = pa.terms_[i]*pb.terms_[j];
			}
			else{
				term(j, i) = pa.terms_[i]*pb.terms_[j];
			}
		}
	}

	return new_poly;
}

Polynomial& operator=(std::string func_str)
{

}

size_t const& Polynomial::term(size_t var1_idx, size_t var2_idx) const
{

}

size_t& Polynomial::term(size_t var1_idx, size_t var2_idx)
{

}

Polynomial Polynomial::first_derivative(size_t var_idx)
{

}

float Polynomial::second_derivative(size_t var1_idx, size_t var2_idx)
{

}

void check_vars_match(Polynomial const& pa, Polynomial const& pb)
{
	if(pa.vs_ptr_ != pb.vs_ptr_){
		throw std::exception("cannot operate on functions with different sets of vars.");
	}
}