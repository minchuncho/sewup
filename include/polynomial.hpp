#include <vector>
#include <map>
#include <regex>

class Polynomial {
public:
	Polynomial() = delete;
	Polynomial(size_t const& nvars);
	Polynomial(Polynomial const& other);
	Polynomial(Polynomial && other);
	~Polynomial();
	Polynomial& operator=(Polynomial const& other);
	Polynomial& operator=(Polynomial && other);
	
	Polynomial& operator=(std::string func_str);
	Polynomial operator+(Polynomial const& pa, Polynomial const& pb);
	Polynomial operator-(Polynomial const& pa, Polynomial const& pb);
	Polynomial operator*(Polynomial const& pa, Polynomial const& pb);
	Polynomial operator*(const float& constant);	// multiply a costant

	Polynomial first_derivative(size_t const& var_idx);
	float second_derivative(size_t const& var1_idx, size_t const& var2_idx);
	size_t const& nvars() const { return nvars_; }
	size_t& nvars() { return nvars_; }
private:
	void check_vars_match(Polynomial const& pa, Polynomial const& pb);
	static void check_reg_exp(std::string const& token);
	static void make_reg_vector();

	size_t nvars_;									// number of variables: one leader, followers, and one constant
	std::vector<float><float> terms_;				// coefficient of each term
	static std::vector<std::regex> regs_;			// regular expressions to different kinds of term
};