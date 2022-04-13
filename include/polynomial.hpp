#include <vector>

class Polynomial {
public:
	Polynomial();
	Polynomial(size_t nvars);
	Polynomial(Polynomial const& other);
	Polynomial(Polynomial && other);
	~Polynomial();

	Polynomial& operator=(Polynomial const& other);
	Polynomial& operator=(Polynomial && other);
	Polynomial& operator=(std::string func_str);
	Polynomial operator+(Polynomial const & pa, Polynomial const &pb);
	Polynomial operator-(Polynomial const & pa, Polynomial const &pb);
	Polynomial operator*(Polynomial const & pa, Polynomial const &pb);

	Polynomial first_deriv(size_t var_idx);
	float second_deriv(size_t var1_idx, size_t var2_idx);
private:
	size_t const& term(size_t var1_idx, size_t var2_idx);
 
	std::vector<float> terms_;
	static std::vector<std::string> vars_;
	static std::vector<size_t> prefix_sum_;
};