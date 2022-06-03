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
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

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
    
    void show_ans();
    void solve_followers();
    void solve_leaders();
    Player& set_leader(size_t const& i, std::string const& func);
    Player& set_follower(size_t const& i, std::string const& func);
    py::array_t<double> fsols();
    py::array_t<double> lsols();

    friend std::ostream& operator<<(std::ostream& os, Solver solver);
    
private:
    void substitute_to_followers();
    void substitute_to_leaders();
    
    size_t nf_;
    size_t nl_;
    Ftype f_ftype_;
    Ftype l_ftype_;
    std::vector<Player> followers_;
    std::vector<Player> leaders_;
    Matrix f_eq_system_;
    std::vector<Polynomial> f_const_system_;
    Matrix l_eq_system_;
    Matrix l_const_system_;
    std::vector<double> fsols_;
    std::vector<double> lsols_;
};

std::ostream& operator<<(std::ostream& os, Solver solver);

PYBIND11_MODULE(_sewup, m) {
   py::enum_<Dtype>(m, "Dtype")
       .value("positive", Dtype::positive)
       .value("negative", Dtype::negative)
       .export_values();

   py::enum_<Ftype>(m, "Ftype")
       .value("utility", Ftype::utility)
       .value("cost", Ftype::cost)
       .export_values();

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

   py::class_<Polynomial>(m, "Polynomial")
       .def(py::init<size_t const&>())
       .def(py::init<size_t const&, std::string const&>())
       .def(py::self == py::self)
       .def_property_readonly("dim", &Polynomial::dim)
       .def("__set__", static_cast<void (Polynomial::*)(std::string const&)>(&Polynomial::operator=))
       .def("__setitem__", &Polynomial::set_element)
       .def("__getitem__", &Polynomial::get_element)
       .def("__repr__", &Polynomial::get_sympy_str)
       .def("first_deriv", &Polynomial::first_deriv);
       
   m.def("multiply", static_cast<Polynomial (*)(Polynomial const&, Polynomial const&)>(&multiply));
   m.def("multiply", static_cast<Polynomial (*)(Polynomial const&, double const&)>(&multiply));
   m.def("add", &add);
   m.def("substract", &substract);
   m.def("substitute", static_cast<Polynomial (*)(size_t const&, Polynomial const&, Polynomial const&)>(&substitute));
   m.def("substitute", static_cast<Polynomial (*)(size_t const&, double const&, Polynomial const&)>(&substitute));

   py::class_<Player>(m, "Player");

   py::class_<Solver>(m, "Solver")
       .def(py::init<size_t, size_t, Ftype, Ftype>())
       .def("show_ans", &Solver::show_ans)
       .def("solve_followers", &Solver::solve_followers)
       .def("solve_leaders", &Solver::solve_leaders)
       .def("set_leader", &Solver::set_leader)
       .def("set_follower", &Solver::set_follower)
       .def_property("fsols", &Solver::fsols, nullptr)
       .def_property("lsols", &Solver::lsols, nullptr);
};

#endif /* solver_hpp */
