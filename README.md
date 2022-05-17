# SewUP

## Introduction

[SewUP](https://github.com/minchuncho/sewup) is a tool for settling the competition in a [Stackelberg game](https://en.wikipedia.org/wiki/Stackelberg_competition) devised based on [backward induction](https://en.wikipedia.org/wiki/Backward_induction).
Formally speaking, a competition can be formlated into a multi-variable optimization problem and the tool is here to help solve the problem.

## Installation

Before installing, make sure the following **dependencies** are also installed:
+ python 3 or above

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

sv.follower[0] = "5x_1+6.7x_1x_2+7x_3+x_4x_4"                 # set up 1st follower's function
...
sv.leader[0] = "x_1x_3+5.5x_3x_3+x_1x_1"                      # set up 1st leader's function
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
- `f_ftype`: the function type for followers, either `cost` or `utility`
- `l_ftype`: the function type for leaders, either `cost` or `utility`
