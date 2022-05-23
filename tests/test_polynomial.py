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
		symbols = symbols(tmp);
		return symbols

	def make_sympy_exp(self, poly, sb):
		n = poly.dim-1
		expr = poly[0][0];

		for j in range(1, n):
			if poly[0, j] != 0:
				expr += poly[0, j] * sb[j];

		for i in range(1, n):
			for j in range(i, n):
				if poly[i, j] != 0:
					expr += poly[0, j] * sb[i] * sb[j];

		return exp;

	# functions of different dimensions cannot work together
	def test_dim_unmatch(self):
		func_str = "4x_1+5x_1x_2+6x_2x_3"
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
			
	# def test_multiplication(self):
	# 	dim = 5
	# 	func_str1 = "0.4x_1+5x_1+0.86x_3"
	# 	func_str2 = "0.2+0.7x_3+0.01x_1"
	# 	c = 0.5;

	# 	poly1 = self.make_polynomial(dim, func_str1)
	# 	poly2 = self.make_polynomial(dim, func_str2)

	# 	sb = self.make_symbols(dim);
	# 	expr1 = 0.4*sb[1]+5*sb[1]+0.86*sb[3];
	# 	expr2 = 0.2+0.7*sb[3]+0.01*sb[1];
		
	# 	ret = _solver.multiply_poly(poly1, poly2);
	# 	ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
	# 	ans_sp = sp.expand(expr1 * expr2);
		
	# 	self.assertEqual(ans_poly, ans_sp);

	# 	ret = _solver.multiply_const(poly2, c);
	# 	ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
	# 	ans_sp = sp.expand(expr2 * c);
		
	# 	self.assertEqual(ans_poly, ans_sp);

	# def test_addition(self):
	# 	dim = 5
	# 	func_str1 = "0.4x_1x_3+5x_1x_2+0.86x_3x_3"
	# 	func_str2 = "0.2+0.7x_3x_4+0.01x_3x_1"
	# 	c = 0.5;

	# 	poly1 = self.make_polynomial(dim, func_str1)
	# 	poly2 = self.make_polynomial(dim, func_str2)

	# 	sb = self.make_symbols(dim);
	# 	expr1 = 0.4*sb[1]*sb[3]+5*sb[1]*sb[2]+0.86*sb[3]**2;
	# 	expr2 = 0.2+0.7*sb[3]*sb[4]+0.01*sb[3]*sb[1];
		
	# 	ret = _solver.add(poly1, poly2);
	# 	ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
	# 	ans_sp = sp.expand(expr1 + expr2);
		
	# 	self.assertEqual(ans_poly, ans_sp);
		
	# def test_substraction(self):
	# 	dim = 5
	# 	func_str1 = "0.4x_1x_3+5x_1x_2+0.86x_3x_3"
	# 	func_str2 = "0.2+0.7x_3x_4+0.01x_3x_1"
	# 	c = 0.5;

	# 	poly1 = self.make_polynomial(dim, func_str1)
	# 	poly2 = self.make_polynomial(dim, func_str2)

	# 	sb = self.make_symbols(dim);
	# 	expr1 = 0.4*sb[1]*sb[3]+5*sb[1]*sb[2]+0.86*sb[3]**2;
	# 	expr2 = 0.2+0.7*sb[3]*sb[4]+0.01*sb[3]*sb[1];
		
	# 	ret = _solver.substract(poly1, poly2);
	# 	ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
	# 	ans_sp = sp.expand(expr1 - expr2);
		
	# 	self.assertEqual(ans_poly, ans_sp);
		
	# def test_substitution(self):
	# 	dim = 5
	# 	func_str1 = "0.4x_1x_3+5x_1x_2+0.86x_3x_3"
	# 	func_str2 = "1.5+0.01x_1+0.6x_2"
	# 	var = 3
	# 	c = 2
		
	# 	poly1 = self.make_polynomial(dim, func_str1)
	# 	poly2 = self.make_polynomial(dim, func_str2)

	# 	sb = self.make_symbols(dim);
	# 	expr1 = 0.4*sb[1]*sb[3]+5*sb[1]*sb[2]+0.86*sb[3]**2;
	# 	expr2 = 1.5+0.01*sb[1]+0.6*sb[2];

	# 	ret = _solver.substitute(var, poly2, poly1);
	# 	ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
	# 	ans_sp = sp.expand(expr1.subs(sb[var], expr2));

	# 	self.assertEqual(ans_poly, ans_sp);

	# 	ret = _solver.substitute(var, c, poly1);
	# 	ans_poly = sp.expand(self.make_sympy_exp(ret, sb))
	# 	ans_sp = sp.expand(expr1.subs(sb[var], c));

	# 	self.assertEqual(ans_poly, ans_sp);

if __name__ == '__main__':
	unittest.main()