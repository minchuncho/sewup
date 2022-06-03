CC = g++
PY_IDIR = $(shell python3-config --includes)
PY_EXTS = $(shell python3-config --extension-suffix)
SDIR = src

# osx
CFLAGS = -O3 -Wall -shared -std=c++17 -fPIC -undefined dynamic_lookup
PYBIND = -Iextern/pybind11/include
OBJS = $(shell find src -name '*.cpp')

EXEC = _sewup

all: $(OBJS)
	$(CC) $(CFLAGS) $(PY_IDIR) $(PYBIND) -Iinclude $(OBJS) -o $(EXEC)$(PY_EXTS)

test: $(OBJS)
	python3 -m pytest tests -v -s
# 	python3 -m pytest tests/test_solver.py -v -s

.PHONY: clean
clean:
	$(RM) $(EXEC)$(PY_EXTS)
	$(RM) -rf .pytest_cache __pycache__
