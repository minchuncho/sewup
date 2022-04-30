#include "polynomial.hpp"

Polynomial::regs_ = make_reg_vector();

std::vector<std::regex> Polynomial::make_reg_vector()
{
	/* regular expression
		1. 2*x_1, -2*x_1, p*x_1
		2. 2*x_1*x_2, -2*x_1*x_2
		3. 2*x_1^2, -2*x_1^2
		4. 2, -2
		5. 2*p, -2*p
	*/

	std::regex reg_singleton("((\\-)?[1-9][0-9]*\\*)?(p\\*)?(x_[1-9][0-9]*)"),
		reg_doubleton("(\\-)?[1-9][0-9]*\\*)?(x_[1-9][0-9]*)\\*(x_[1-9][0-9]*)"),
		reg_square("(\\-)?[1-9][0-9]*\\*)?(x_[1-9][0-9]*)\\^2"),
		reg_const("((\\-)?[1-9][0-9]*)?"),
		reg_p("((\\-)?[1-9][0-9]*\\*)?p");

	std::vector<std::regex> regs = {reg_singleton, reg_doubleton, reg_square, reg_const, reg_p};

	return regs;
}

void Polynomial::check_reg_exp(std::string token)
{
	bool valid = false;
	for(auto const& reg:regs_){
		valid |= regex_match(token, reg);
	}

	if(!valid) throw std::regex_error("token not matched");
}

void Polynomial::check_vars_match(Polynomial const& pa, Polynomial const& pb)
{
	if(pa.nvars_ != pb.nvars_){
		throw std::exception("cannot operate on functions with different sets of vars.");
	}
}

Polynomial::Polynomial(size_t const& nvar)
{
	nvars_ = nvars;
	if(!terms_.empty){
		terms_.clear();
		terms_.shrink_to_fit();
	}
	terms_.resize(nvars_+1, std::vector<float>(nvars_+1, 0));
}

Polynomial::Polynomial(Polynomial const& other)
{
	terms_ = other.terms_;
	nvars_ = other.nvars_;
}

Polynomial::Polynomial(Polynomial && other)
{
	terms_ = std::move(other.terms_);
	std::swap(nvars_, other.nvars_);
}

Polynomial::~Polynomial()
{
	for(int i=0; i<terms_.size(); ++i){
		terms_[i].clear();
		terms_[i].shrink_to_fit();
	}
}

Polynomial& Polynomial::operator=(Polynomial const& other)
{
	if(this != &other){
		terms_ = other.terms_;
		nvars_ = other.nvars_;
	}

	return *this;
}

Polynomial& Polynomial::operator=(Polynomial && other)
{
	if(this != &other){
		for(int i=0; i<terms_.size(); ++i){
			terms_[i] = std::move(other.terms_[i]);
		}

		std::swap(nvars_, other.nvars_);
	}

	return *this;
}

Polynomial& Polynomial::operator=(std::string func_str)
{
	// example: 1 + x_3 - 2*x_5 + x_2*x_3 + p*x_7
	std::vector<std::string> tokens;
	str::string token = "";
	for(int i=0; i<func_str.length(); ++i){
		char c = str[i];
		switch(c){
			case '+':
				check_reg_exp(token);
				tokens.push_back(token);
				token.clear();
				break;
			case '-':
				check_reg_exp(token);
				tokens.push_back(token);
				token.clear();
				token += c; // put minus back for the next term
				break;
			case ' ':
				break;
			default:
				token += c;
				break;
		}
	}

	for(int i=0; i<tokens.size(); ++i){
		token = tokens[i];
		std::string constant = "";
		int j=0;

		// constant
		for(; j<token.size(); ++j){
			char tc = token[j];
			if(tc == '-'){
				constant += tc;
				continue;
			}
			if(tc != 'x' || tc != 'p'){
				constant += tc;
			}
			else{
				break;
			}
		}

		int coefficient = std::stoi(constant);

		// variables
		--j;
		std::vector<short> index(2, -1);
		size_t count=0;

		while(j<token.size()){
			char tc=token[j];
			bool has_p = false;
			if(tc == 'p'){
				has_p = true;
				++j;
				continue;
			}
			else if(tc='x'){
				j+=2;
				index[count++]=token[j++];
			}
		}

		// assign coefficient to term
		int idx1=index[0], idx2=index[1];

		if(idx2 == -1){
			if(has_p) idx2 = nvars_;
			else idx2 = idx1;
			idx1=0;
		}
		
		if(idx1 > idx2) swap(idx1, idx2);
		terms_[idx1][idx2] = coefficient;
	}
}

Polynomial Polynomial::operator+(Polynomial const& pa, Polynomial const& pb)
{
	check_vars_match(pa, pb);

	Polynomial new_poly(pa.nvars_);
	for(int i=0; i<=nvars_; ++i){
		for(int j=i; j<=nvars_; ++j){
			new_poly.terms_[i][j] = pa.terms_[i][j]+pb.terms_[i][j];
		}
	}
}

Polynomial Polynomial::operator-(Polynomial const& pa, Polynomial const& pb)
{
	check_vars_match(pa, pb);

	Polynomial new_poly(pa.nvars_);
	for(int i=0; i<=nvars_; ++i){
		for(int j=i; j<=nvars_; ++j){
			new_poly.terms_[i][j] = pa.terms_[i][j]-pb.terms_[i][j];
		}
	}
}

Polynomial Polynomial::operator*(Polynomial const& pa, Polynomial const& pb)
{
	check_vars_match(pa, pb);

	Polynomial new_poly(pa.nvars_);
	for(int i=0; i<=nvars_; ++i){
		for(int j=0; j<=nvars_; ++j){
			if(i<j){
				new_poly.terms_[i][j] = pa.terms_[0][i]+pb.terms_[0][j];
			}
			else{
				new_poly.terms_[j][i] = pa.terms_[0][i]+pb.terms_[0][j];
			}
		}
	}

	return new_poly;
}

Polynomial& Polynomial::first_derivative(size_t const& var_idx)
{
	Polynomial fderiv(this->nvars_);
	
	// row var_idx
	for(int j=var_idx; j<=this->nvars_; ++j){
		int res = this->terms_[var_idx][j];
		if(j == var_idx) res *= 2;
		fderiv->terms_[0][j] = res;
	}

	// col var_idx
	for(int i=0; i<this->nvars_; ++i){
		fderiv->terms_[i][0] = this->terms_[i][var_idx];;
	}

	this->first_derivs_[var_idx] = fderiv;

	return this->first_derivs_[var_idx];
}

float Polynomial::second_derivative(size_t const& var1_idx, size_t const& var2_idx)
{
	if(this->first_derivs_.find(var1_idx) != this->first_derivs_.end()){
		Polynomial fderiv=this->first_derivs_[var1_idx];
		Polynomial sderiv=fderiv.first_derivative(var2_idx);
		return sderiv[0][0];
	}
	
	if(this->first_derivs_.find(var2_idx) != this->first_derivs_.end()){
		Polynomial fderiv=this->first_derivs_[var2_idx];
		Polynomial sderiv=fderiv.first_derivative(var1_idx);
		return sderiv[0][0];
	}

	Polynomial fderiv=first_derivative(var1_idx),
				sderiv=fderiv.first_derivative(var2_idx);

	return sderiv[0][0];			
}