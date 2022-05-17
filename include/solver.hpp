//
//  solver.hpp
//  sewup-xcode
//
//  Created by MinChunCho on 2022/5/9.
//

#ifndef solver_hpp
#define solver_hpp

#include <vector>
#include <pybind11/pybind11.h>
// #include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include "player.hpp"
#include "polynomial.hpp"
#include "matrix.hpp"

namespace py = pybind11;

class Solver{
public:
    Solver() = delete;
    Solver(size_t nf, size_t nl, Ftype f_ftype, Ftype l_ftype);
    Solver(Solver const& other);
    Solver(Solver && other);
    Solver& operator=(Solver const& other);
    Solver& operator=(Solver && other);
    ~Solver() = default;
    
    friend std::ostream& operator<<(std::ostream& os, Solver solver);
    void show_ans();
    void solve_followers();
    void solve_leaders();
    
private:
    void substitute_to_followers();
    void substitute_to_leaders();
    
    std::vector<Player> followers_;
    Ftype f_ftype_;
    size_t nf_;
    std::vector<Player> leaders_;
    Ftype l_ftype_;
    size_t nl_;
    Matrix f_eq_system_;
    std::vector<Polynomial> f_const_system_;
    Matrix l_eq_system_;
    Matrix l_const_system_;
};

std::ostream& operator<<(std::ostream& os, Solver solver);

PYBIND11_MODULE(_solver, m) {
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t const&, size_t const&>())
        .def(py::init<size_t const&, size_t const&, std::vector<double> const&>())
        .def(py::self == py::self)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__setitem__", &Matrix::set_element)
        .def("__getitem__", &Matrix::get_element)
        .def("__repr__", &Matrix::get_matrix_str)
        .def("inverse", &Matrix::inverse)
        .def("dfness", &Matrix::dfness);
        
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);

    py::enum_<Dtype>(m, "Dtype")
        .value("positive", Dtype::positive)
        .value("negative", Dtype::negative)
        .export_values();
};

#endif /* solver_hpp */
