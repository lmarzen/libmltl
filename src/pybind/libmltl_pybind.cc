#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "parser.hh"

namespace py = pybind11;
using namespace std;
using namespace libmltl;

PYBIND11_MODULE(libmltl, m) {
  /* ast.hh
   */
  py::class_<ASTNode, shared_ptr<ASTNode>>(m, "ASTNode")
      .def("get_type", &ASTNode::get_type)
      .def("is_unary_op", &ASTNode::is_unary_op)
      .def("is_binary_op", &ASTNode::is_binary_op)
      .def("is_propositional_op", &ASTNode::is_propositional_op)
      .def("is_temporal_op", &ASTNode::is_temporal_op)
      .def("as_string", &ASTNode::as_string)
      .def("as_pretty_string", &ASTNode::as_pretty_string)
      .def("get_symbol", &ASTNode::get_symbol)
      .def("evaluate", &ASTNode::evaluate)
      .def("evaluate_subt", &ASTNode::evaluate_subt)
      .def("future_reach", &ASTNode::future_reach)
      .def("size", &ASTNode::size)
      .def("depth", &ASTNode::depth)
      .def("count", &ASTNode::count)
      .def("deep_copy", &ASTNode::deep_copy)
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def(py::self < py::self)
      .def(py::self > py::self)
      .def(py::self <= py::self)
      .def(py::self >= py::self);

  py::enum_<ASTNode::Type>(m, "Type")
      .value("Constant", ASTNode::Type::Constant)
      .value("Variable", ASTNode::Type::Variable)
      .value("Negation", ASTNode::Type::Negation)
      .value("And", ASTNode::Type::And)
      .value("Xor", ASTNode::Type::Xor)
      .value("Or", ASTNode::Type::Or)
      .value("Implies", ASTNode::Type::Implies)
      .value("Equiv", ASTNode::Type::Equiv)
      .value("Finally", ASTNode::Type::Finally)
      .value("Globally", ASTNode::Type::Globally)
      .value("Until", ASTNode::Type::Until)
      .value("Release", ASTNode::Type::Release);

  py::class_<Constant, ASTNode, shared_ptr<Constant>>(m, "Constant")
      .def(py::init<bool>())
      .def("get_value", &Constant::get_value)
      .def("set_value", &Constant::set_value);

  py::class_<Variable, ASTNode, shared_ptr<Variable>>(m, "Variable")
      .def(py::init<unsigned int>())
      .def("get_id", &Variable::get_id)
      .def("set_id", &Variable::set_id);

  py::class_<UnaryOp, ASTNode, shared_ptr<UnaryOp>>(m, "UnaryOp")
      .def("get_operand", py::overload_cast<>(&UnaryOp::get_operand),
           py::return_value_policy::reference)
      .def("set_operand", &UnaryOp::set_operand);

  py::class_<UnaryPropOp, UnaryOp, shared_ptr<UnaryPropOp>>(m, "UnaryPropOp");

  py::class_<Negation, UnaryPropOp, shared_ptr<Negation>>(m, "Negation")
      .def(py::init<shared_ptr<ASTNode>>())
      .def(py::init<>());

  py::class_<UnaryTempOp, UnaryOp, shared_ptr<UnaryTempOp>>(m, "UnaryTempOp")
      .def("get_lower_bound", &UnaryTempOp::get_lower_bound)
      .def("get_upper_bound", &UnaryTempOp::get_upper_bound)
      .def("set_lower_bound", &UnaryTempOp::set_lower_bound)
      .def("set_upper_bound", &UnaryTempOp::set_upper_bound);

  py::class_<Finally, UnaryTempOp, shared_ptr<Finally>>(m, "Finally")
      .def(py::init<shared_ptr<ASTNode>, size_t, size_t>())
      .def(py::init<>());

  py::class_<Globally, UnaryTempOp, shared_ptr<Globally>>(m, "Globally")
      .def(py::init<shared_ptr<ASTNode>, size_t, size_t>())
      .def(py::init<>());

  py::class_<BinaryOp, ASTNode, shared_ptr<BinaryOp>>(m, "BinaryOp")
      .def("get_left", py::overload_cast<>(&BinaryOp::get_left),
           py::return_value_policy::reference)
      .def("get_right", py::overload_cast<>(&BinaryOp::get_right),
           py::return_value_policy::reference)
      .def("set_left", &BinaryOp::set_left)
      .def("set_right", &BinaryOp::set_right);

  py::class_<BinaryPropOp, BinaryOp, shared_ptr<BinaryPropOp>>(m,
                                                               "BinaryPropOp");

  py::class_<And, BinaryPropOp, shared_ptr<And>>(m, "And")
      .def(py::init<shared_ptr<ASTNode>, shared_ptr<ASTNode>>())
      .def(py::init<>());

  py::class_<Xor, BinaryPropOp, shared_ptr<Xor>>(m, "Xor")
      .def(py::init<shared_ptr<ASTNode>, shared_ptr<ASTNode>>())
      .def(py::init<>());

  py::class_<Or, BinaryPropOp, shared_ptr<Or>>(m, "Or")
      .def(py::init<shared_ptr<ASTNode>, shared_ptr<ASTNode>>())
      .def(py::init<>());

  py::class_<Implies, BinaryPropOp, shared_ptr<Implies>>(m, "Implies")
      .def(py::init<shared_ptr<ASTNode>, shared_ptr<ASTNode>>())
      .def(py::init<>());

  py::class_<Equiv, BinaryPropOp, shared_ptr<Equiv>>(m, "Equiv")
      .def(py::init<shared_ptr<ASTNode>, shared_ptr<ASTNode>>())
      .def(py::init<>());

  py::class_<BinaryTempOp, BinaryOp, shared_ptr<BinaryTempOp>>(m,
                                                               "BinaryTempOp")
      .def("get_lower_bound", &BinaryTempOp::get_lower_bound)
      .def("get_upper_bound", &BinaryTempOp::get_upper_bound)
      .def("set_lower_bound", &BinaryTempOp::set_lower_bound)
      .def("set_upper_bound", &BinaryTempOp::set_upper_bound);

  py::class_<Until, BinaryTempOp, shared_ptr<Until>>(m, "Until")
      .def(py::init<shared_ptr<ASTNode>, shared_ptr<ASTNode>, size_t, size_t>())
      .def(py::init<>());

  py::class_<Release, BinaryTempOp, shared_ptr<Release>>(m, "Release")
      .def(py::init<shared_ptr<ASTNode>, shared_ptr<ASTNode>, size_t, size_t>())
      .def(py::init<>());

  /* parser.hh
   */
  m.def("parse", &parse);
  m.def("read_trace_file", &read_trace_file);
  m.def("read_trace_files", &read_trace_files);
  m.def("int_to_bin_str", &int_to_bin_str);
}
