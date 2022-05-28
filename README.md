# SewUP

## Introduction

[SewUP](https://github.com/minchuncho/sewup) is a tool for settling the competition in a [Stackelberg game](https://en.wikipedia.org/wiki/Stackelberg_competition) devised based on [backward induction](https://en.wikipedia.org/wiki/Backward_induction).
Formally speaking, a competition can be formlated into a multi-variable optimization problem and the tool is here to help solve the problem.

## Installation

Before installing, make sure the following **dependencies** are also installed:
+ python 3 or above
+ pybind11

**Install SewUP** by entering the following commands:
```sh
git clone --recursive git@github.com:minchuncho/sewup.git
cd sewup && make
```

## Simple Guide
```python
import _solver                                                # import the SewUP module

nf=2
nl=2
sv = _solver.Solver(nf, nl, _solver.cost, _solver.utility);   # initialize a solver with 2 followers and 2 leaders

sv.followers(0) = "5*x1+6.7*x1x2+7*x3+x4x4"                 # set up 1st follower's function
...
sv.leaders(0) = "x1x3+5.5*x3x3+x1x1"                      # set up 1st leader's function
...

sv.solve_followers();
sv.solve_leaders();
sv.show_ans();
```

## API Description
```python
Solver(size_t nf, size_t nl, Ftype f_ftype, Ftype l_ftype)
```
- `nf`: number of followers
- `nl`: number of leaders
- `f_ftype`: the function type for followers, either `_solver.cost` or `_solver.utility`
- `l_ftype`: the function type for leaders, either `_solver.cost` or `_solver.utility`
  - `_solver.cost` and `_solver.utility` belong to `enum Ftype`
