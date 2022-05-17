CC = g++
PY_IDIR = $(shell python3-config --includes)
PY_EXTS = $(shell python3-config --extension-suffix)
SDIR = src

# ubuntu
CFLAGS = -O3 -Wall -shared -std=c++17 -fPIC
PYBIND = -Iextern/pybind11/include -lpython3.8
OBJS = $(shell find src -name '*.cpp')

# osx
# CFLAGS = -O3 -Wall -shared -std=c++17 -fPIC -undefined dynamic_lookup
# # PYBIND = -I/usr/local/include/pybind11
# PYBIND = -Iextern/pybind11/include
# OBJS = $(shell find src -name '*.cpp')

EXEC = _solver

all: $(OBJS)
	$(CC) $(CFLAGS) $(PY_IDIR) $(PYBIND) -Iinclude $(OBJS) -o $(EXEC)$(PY_EXTS)

test: $(OBJS)
# 	python3 -m pytest tests -v -s
	python3 -m pytest tests/test_matrix.py -v -s

.PHONY: clean
clean:
	$(RM) $(EXEC)$(PY_EXTS)
	$(RM) -rf .pytest_cache __pycache__