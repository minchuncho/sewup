CC = g++
PY_IDIR = $(shell python3-config --includes)
PY_EXTS = $(shell python3-config --extension-suffix)
SDIR = src

# ubuntu
CFLAGS = -O3 -Wall -shared -std=c++17 -fPIC
PYBIND = -I/usr/include/pybind11 -lpython3.8
OBJS = $(shell find src -name '*.cpp')

# osx
# CFLAGS = -O3 -Wall -shared -std=c++17 -fPIC -undefined dynamic_lookup
# PYBIND = -I/usr/local/include/pybind11
# OBJS = $(SDIR)/*.c

EXEC = _solver

all: $(OBJS)
	$(CC) $(CFLAGS) $(PY_IDIR) $(PYBIND) -I. $(SDIR)$(OBJS) -o $(EXEC)$(PY_EXTS)

test: $(OBJS)
	python3 -m unittest tests

.PHONY: clean
clean:
	$(RM) $(EXEC)$(PY_EXTS)
	$(RM) -rf .pytest_cache __pycache__