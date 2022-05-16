#!/usr/bin/python3

import numpy as np
import unittest

import _solver

class MatrixTest(unittest.TestCase):
	def make_matrix(self, size):
		rand_list = np.random.rand(size, size);
		mat = _solver.Matrix(size, size, rand_list.flatten())
		return mat, rand_list;

	def make_matrix_arr(self, size, arr):
		mat = _solver.Matrix(size, size, arr.flatten())
		return mat;

	def test_multiplication(self):
		size = 1000
		tsize = 16
		mat1, rl1  = self.make_matrix(size)
		mat2, rl2 = self.make_matrix(size)

		naive = _solver.multiply_naive(mat1, mat2)
		tile = _solver.multiply_tile(mat1, mat2, tsize)
		
		self.assertEqual(naive, tile)

	# def test_inverse(self):
	# 	size = 3
	# 	mat, rl = self.make_matrix(size)
		
	# 	inv = mat.inverse()
	# 	for i in range(size):
	# 		arr = []
	# 		for j in range(size): 
	# 			arr.append(mat[i, j])

	# 	self.assertEqual(arr, np.linalg.inv(rl))

	# def test_dfness(self):
	# 	size = 3

	# 	rl1 = np.array([(2, -1, 0), (-1, 2, -1), (0, -1, 2)])
	# 	rl2 = np.array([(-1, 1, -2), (1, -5, 2), (-2, 2, -5)])
	# 	mat1 = self.make_matrix_arr(size, rl1)
	# 	mat2 = self.make_matrix_arr(size, rl2)

	# 	dfness1 = np.all(np.linalg.eigvals(rl1) > 0) # positive dfness
	# 	dfness2 = np.all(np.linalg.eigvals(rl2) < 0) # negative dfness

	# 	self.assertEqual(dfness1, mat1.dfness(_solver.positive))
	# 	self.assertEqual(dfness2, mat2.dfness(_solver.negative))

# if __name__ == '__main__':
# 	unittest.main()