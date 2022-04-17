#include <vector>
#include <regex>

typedef struct
{
	size_t nvars_;
	size_t nterms_;
	std::vector<size_t> prefix_sum_;
} VarsStruct;

class Polynomial {
public:
	Polynomial() = delete;
	Polynomial(VarsStrust const * const vs_ptr);
	Polynomial(Polynomial const& other);
	Polynomial(Polynomial && other);
	~Polynomial();
	Polynomial& operator=(Polynomial const& other);
	Polynomial& operator=(Polynomial && other);
	
	Polynomial& operator=(std::string func_str);
	Polynomial operator+(Polynomial const& pa, Polynomial const& pb);
	Polynomial operator-(Polynomial const& pa, Polynomial const& pb);
	Polynomial operator*(Polynomial const& pa, Polynomial const& pb);

	void set_func();
	size_t const& term(size_t var1_idx, size_t var2_idx) const;
	size_t& term(size_t var1_idx, size_t var2_idx);
	Polynomial first_derivative(size_t var_idx);
	float second_derivative(size_t var1_idx, size_t var2_idx);
private:
	void check_vars_match(Polynomial const& pa, Polynomial const& pb);

	std::vector<float> terms_;
	VarsStruct* vs_ptr_;
};