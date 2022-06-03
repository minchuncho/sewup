#!/usr/bin/python3

import numpy as np
import unittest
import math

import _sewup

class SolverTest(unittest.TestCase):
	def make_solver(self, nf, nl, f_ftype, l_ftype):
		solver = _sewup.Solver(nf, nl, f_ftype, l_ftype)
		return solver	

	def test_unsolvable_cases_followers(self):
		nf = 2
		nl = 2
		f_ftype = _sewup.utility
		l_ftype = _sewup.utility
		
		solver = self.make_solver(nf, nl, f_ftype, l_ftype)

		solver.set_leader(0, "1*x1+1*x2");
		solver.set_leader(1, "1*x1-1*x2");
		solver.set_follower(0, "1*x1x2-1*x2x2+4.3*x3x1");
		solver.set_follower(1, "1*x1x2-0.6*x1x1+4.3*x4x2+5.1*x3x2");
		
		with self.assertRaises(RuntimeError):
			solver.solve_followers()

	def test_unsolvable_cases_leaders(self):
		nf = 2
		nl = 2
		f_ftype = _sewup.utility
		l_ftype = _sewup.utility
		
		solver = self.make_solver(nf, nl, f_ftype, l_ftype)

		solver.set_leader(0, "1*x1");
		solver.set_leader(1, "1*x2");
		solver.set_follower(0, "1*x1x2-5.5*x1x1-9*x2x2+4.3*x3x1");
		solver.set_follower(1, "1*x1x2-8.6*x1x1-3.2*x2x2+4.3*x4x2+5.1*x3x2");

		solver.solve_followers()
		with self.assertRaises(RuntimeError):
			solver.solve_leaders()

	def test_solvable_case(self):
		nf = 2
		nl = 2
		f_ftype = _sewup.utility
		l_ftype = _sewup.utility
		
		solver = self.make_solver(nf, nl, f_ftype, l_ftype)

		solver.set_leader(0, "1*x1-1*x3x2-3.1*x3x3-2*x4x4");
		solver.set_leader(1, "1*x2-1.1*x2x4-4*x3x3-4.5*x4x4");
		solver.set_follower(0, "1*x1x2-5.5*x1x1-9*x2x2+4.3*x3x1");
		solver.set_follower(1, "1*x1x2-8.6*x1x1-3.2*x2x2+4.3*x4x2+5.1*x3x2");

		solver.solve_followers()
		solver.solve_leaders()

		fsols = solver.fsols
		lsols = solver.lsols

		fans = [50320353379/1728091840720, 15193264759/172809184072]
		lans = [26914499/498009176, 14936757/249004588]

		for i in range(nf):
			self.assertTrue(math.isclose(fsols[i], fans[i]))

		for i in range(nl):
			self.assertTrue(math.isclose(lsols[i], lans[i]))

if __name__ == '__main__':
	unittest.main()