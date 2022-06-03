# SewUP

## Introduction

There exists competitions of many forms in the world and whether or not a competition can be resolved hinges on the existence of [Nash equalibriums (NE)](https://en.wikipedia.org/wiki/Nash_equilibrium). But as a matter of fact, as a competition goes on and on, NEs can be difficult to reach even if they are proven existed.

In order to spot NEs, some competitions can be reduced into [Stackelberg game](https://en.wikipedia.org/wiki/Stackelberg_competition) and further take advantages of [backward induction](https://en.wikipedia.org/wiki/Backward_induction), which guarantees a NE in only two steps.
Note that for backward induction to work properly, a specific form of mathematic function is also a must.

However, Stackelberg game often involves high-dimensional multi-variable optimization problems. That being said, the complexity of such problems tend to stump people and thus they cannot be done by hand even backward induction is drawn on. Consequently, [SewUP](https://github.com/minchuncho/sewup) is created to serve the purpose.

[SewUP](https://github.com/minchuncho/sewup) focuses on the multi-variable optimization problems of Stackelberg game which is designed based on backward induction. With only a few steps, the answer is at your finger tips.

## Prerequisites
+ python 3 or above
+ pybind11
+ sympy

## Installation
```sh
$ git clone --recursive git@github.com:minchuncho/sewup.git
$ cd sewup && make
```

## Simple Guide
```python
import _sewup                   # import the SewUP module

nf = 2                          # number of followers
nl = 2                          # number of leaders
f_ftype = _sewup.utility        # follower function type
l_ftype = _sewup.utility        # leader function type

# initialize a Stackelberg game solver
solver = _sewup.Solver(nf, nl, f_ftype, l_ftype)

# set leader functions
solver.set_leader(0, "1*x1-1*x3x2-3.1*x3x3-2*x4x4");
solver.set_leader(1, "1*x2-1.1*x2x4-4*x3x3-4.5*x4x4");

# set worker functions
solver.set_follower(0, "1*x1x2-5.5*x1x1-9*x2x2+4.3*x3x1");
solver.set_follower(1, "1*x1x2-8.6*x1x1-3.2*x2x2+4.3*x4x2+5.1*x3x2");

# solve equations based on backward induction
solver.solve_followers()
solver.solve_leaders()

# get solutions
fsols = solver.fsols
lsols = solver.lsols

# show solutions
solver.show_ans()
```

## API Description
### Solver Initialization
A solver is in demand for a set of followers and leaders.
```python
Solver(nf, nl, f_ftype, l_ftype)
```
- `nf`: number of followers (`uint32`)
- `nl`: number of leaders (`uint32`)
- `f_ftype`: the function type for followers, either `_sewup.cost` or `_sewup.utility` (`Ftype`)
- `l_ftype`: the function type for leaders, either `_sewup.cost` or `_sewup.utility` (`Ftype`)

Note that `enum Ftype` is used to specify leader's and follower's mathematic functions are "utility" or "cost," respectively. This is important because we need the information to judge it is a maximum or a minimum that we are looking for.

### Set leader and follower mathematic function
The order of variables are `x0`, `x1`, ..., `x[nf]` for followers and `x[nf+1]`, ..., `x[nf+nl]` for leaders.

For instance, if `nf=2` and `nl=3`, then the followers own the variable `x1` and `x2` and the leaders own the variables `x3`, `x4`, and `x5`.

```python
solver.set_leader(i, func_str);
```
- `i`: the index of leader starting from 0 (`uint32`)
- `func_str`: string for the desired mathematic function (`string`)

**>> IMPORTANT <<**

As mentioned previously, math functions must be in a special form so the rule is described below:
- Variable is recognized as `x1 x2 x3 ...` instead of `x_1 x_2 x_3 ...` or other forms.
- When multiplying one variable with another, `x1x2` is valid and `x1*x2` is **not**. That is, no "\*" between variables.
  - `x1x1` stands for the sqaure of `x1` while `x1^2` is **not recognizable**.
- When multiplying one variable with a constant, "\*" is **required**. For example, `1*x1x4`.
  - **A constant must be specified** even though it is just 1.
- A term more than two variables such as `x1x3x4` or `x1x1x2` is **not allowed**.
- A term with variables in a fraction form such as `x1/x3` is **not allowed**.

As for `set_follower(i, func_str)`, it works the same way as `set_leader(i, func_str)`.

### Start Solving the Problem
After the previous steps are all set, call the following functions **in order** to evoke the process of solving equations.
```python
solver.solve_followers()
solver.solve_leaders()
```
The following are the error messages you might receive:
- Some follower's hessian matrix with respect to all followers' variables are not concave or convex (corresponding to `_sewup.utility` or `_sewup.cost`)
- Some leader's hessian matrix with respect to all leaders' variables are not concave or convex (corresponding to `_sewup.utility` or `_sewup.cost`)
- There are no solution to the problem.

If you finish without receiving any exception, the solver has gotten the answer (NE) in hand.

### Check the Answer
This API simply prints out the NE.
```python
solver.show_ans()
```
Or you can use the following APIs to check the answer by `np.array`.
```python
fsols = solver.fsols        # followers' solution kept in np.array
lsols = solver.lsols        # leaders' solution kept in np.array
```
