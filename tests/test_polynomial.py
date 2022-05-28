#!/usr/bin/python3

import numpy as np
import sympy as sp
import unittest
import math

import _solver

class PolynomialTest(unittest.TestCase):
	def make_polynomial(self, dim, func_str):
		poly = _solver.Polynomial(dim, func_str)
		return poly

	def make_symbols(self, dim):
		tmp = "x:1" + str(dim-1);
		symbols = sp.symbols(tmp);
		return symbols

	def make_sympy_exp(self, poly, sb):
		n = poly.dim
		expr = poly[0, 0]

		for j in range(1, n):
			if poly[0, j] != 0:
				expr += poly[0, j] * sb[j]

		for i in range(1, n):
			for j in range(i, n):
				if poly[i, j] != 0:
					if i == j :
						expr += poly[i, j] * sb[i]**2
					else:
						expr += poly[i, j] * sb[i] * sb[j]

		return expr

	def test_invalid_function_string(self):
		dim = 4
		func_str1 = "4x_1+5x_1x_2+6x_2x_3"
		func_str2 = "6x_2x_3x_3"
		func_str3 = "4x_1^2"
		func_str4 = "4*x5x7"
		func_str5 = "4*x_5x_7"
		
		with self.assertRaises(ValueError):
			poly = self.make_polynomial(dim, func_str1)

		with self.assertRaises(ValueError):
			poly = self.make_polynomial(dim, func_str2)
		
		with self.assertRaises(ValueError):
			poly = self.make_polynomial(dim, func_str3)

		with self.assertRaises(IndexError):
			poly = self.make_polynomial(dim, func_str4)

		with self.assertRaises(ValueError):
			poly = self.make_polynomial(dim, func_str5)

	# functions of different dimensions cannot work together
	def test_dimension(self):
		func_str = "4*x1+5*x1x2+6*x2x3"
		dim1 = 4
		dim2 = 6
		poly1 = self.make_polynomial(dim1, func_str)
		poly2 = self.make_polynomial(dim2, func_str)
		
		with self.assertRaises(IndexError):
			ret = _solver.multiply_poly(poly1, poly2);

		with self.assertRaises(IndexError):
			ret = _solver.add(poly1, poly2);

		with self.assertRaises(IndexError):
			ret = _solver.substract(poly1, poly2);
			
	def test_multiplication(self):
		dim = 5
		func_str1 = "0.4*x1+5*x1+0.86*x3"
		func_str2 = "0.2+0.7*x3+0.01*x1"
		c = 0.5;

		poly1 = self.make_polynomial(dim, func_str1)
		poly2 = self.make_polynomial(dim, func_str2)

		sb = self.make_symbols(dim);
		expr1 = 0.4*sb[1]+5*sb[1]+0.86*sb[3];
		expr2 = 0.2+0.7*sb[3]+0.01*sb[1];
		
		ret = _solver.multiply(poly1, poly2);
		ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
		ans_sp = sp.expand(expr1 * expr2);
		
		self.assertTrue(sp.simplify(ans_poly - ans_sp) == 0);

		ret = _solver.multiply(poly2, c);
		ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
		ans_sp = sp.expand(expr2 * c);
		
		self.assertTrue(sp.simplify(ans_poly - ans_sp) == 0);

	def test_addition(self):
		dim = 5
		func_str1 = "0.4*x1x3+5*x1x2+0.86*x3x3"
		func_str2 = "0.2+0.7*x3x4+0.01*x3x1"
		c = 0.5;

		poly1 = self.make_polynomial(dim, func_str1)
		poly2 = self.make_polynomial(dim, func_str2)

		sb = self.make_symbols(dim);
		expr1 = 0.4*sb[1]*sb[3]+5*sb[1]*sb[2]+0.86*sb[3]**2;
		expr2 = 0.2+0.7*sb[3]*sb[4]+0.01*sb[3]*sb[1];
		
		ret = _solver.add(poly1, poly2);
		ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
		ans_sp = sp.expand(expr1 + expr2);
		
		self.assertTrue(sp.simplify(ans_poly - ans_sp) == 0);
		
	def test_substraction(self):
		dim = 5
		func_str1 = "0.4*x1x3+5*x1x2+0.86*x3x3"
		func_str2 = "0.2+0.7*x3x4+0.01*x3x1"
		c = 0.5;

		poly1 = self.make_polynomial(dim, func_str1)
		poly2 = self.make_polynomial(dim, func_str2)

		sb = self.make_symbols(dim);
		expr1 = 0.4*sb[1]*sb[3]+5*sb[1]*sb[2]+0.86*sb[3]**2;
		expr2 = 0.2+0.7*sb[3]*sb[4]+0.01*sb[3]*sb[1];
		
		ret = _solver.substract(poly1, poly2);
		ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
		ans_sp = sp.expand(expr1 - expr2);
		
		self.assertTrue(sp.simplify(ans_poly - ans_sp) == 0);
		
	def test_substitution(self):
		dim = 5
		func_str1 = "0.4*x1x3+5*x1x2+0.86*x3x3"
		func_str2 = "1.5+0.01*x1+0.6*x2"
		var = 3
		c = 2
		
		poly1 = self.make_polynomial(dim, func_str1)
		poly2 = self.make_polynomial(dim, func_str2)

		sb = self.make_symbols(dim)
		expr1 = 0.4*sb[1]*sb[3]+5*sb[1]*sb[2]+0.86*sb[3]**2
		expr2 = 1.5+0.01*sb[1]+0.6*sb[2]

		ret = _solver.substitute(var, poly2, poly1)
		ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
		ans_sp = sp.expand(expr1.subs(sb[var], expr2))

		self.assertTrue(sp.simplify(ans_poly - ans_sp) == 0)

		expr1 = 0.4*sb[1]*sb[3]+5*sb[1]*sb[2]+0.86*sb[3]**2

		ret = _solver.substitute(var, c, poly1)
		ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
		ans_sp = sp.expand(expr1.subs(sb[var], c))

		self.assertTrue(sp.simplify(ans_poly - ans_sp) == 0)

	def test_first_deriv(self):
		dim = 5
		func_str = "0.4*x1x3+5*x1x2+0.86*x3x3"
		poly = self.make_polynomial(dim, func_str)
		var = 3

		sb = self.make_symbols(dim)
		expr = 0.4*sb[1]*sb[3]+5*sb[1]*sb[2]+0.86*sb[3]**2
		
		ret = poly.first_deriv(var)
		ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
		ans_sp = sp.expand(sp.diff(expr, sb[var]))	

		self.assertTrue(sp.simplify(ans_poly - ans_sp) == 0)

if __name__ == '__main__':
	unittest.main()