#include <iostream>
#include <memory>

// include "ast.hh" for ASTNode class
// include "parser.hh" for string and file parsing

#include "ast.hh"
#include "parser.hh" // also includes ast.hh

using namespace std;
using namespace libmltl;

int main(int argc, char *argv[]) {

  // shared_ptr<ASTNode> test = parse("true");
  // test = parse("(p0&~p1)R[0,3](p2)");
  // cout << test->as_string() << "\n";
  // cout << test->evaluate(
  //             {"001", "001", "101", "000", "000", "101", "100", "110"})
  //      << "\n";

  // call the parse function to return an AST
  shared_ptr<ASTNode> ast = parse("G[0,4](~p1&p2)");

  // print the parsed AST, either as_string or as_pretty_string
  // as_pretty_string adds additional white space and removes redundant parens
  // to make the formula more human readable, while as_string attempts to be
  // more true to the representation of the parse tree.
  cout << ast->as_string() << "\n";
  cout << ast->as_pretty_string() << "\n";

  // evaluate traces
  bool result;
  vector<string> trace = {"101", "101", "001", "101", "101", "001"};
  result = ast->evaluate(trace); // true
  cout << (result ? "true" : "false") << "\n";
  trace = {"101", "101", "001", "101", "111", "001"};
  result = ast->evaluate(trace); // false
  cout << (result ? "true" : "false") << "\n";

  // calculate future reach of root node
  size_t future_reach = ast->future_reach();
  cout << "future reach: " << future_reach << "\n";

  // some methods are specific to a certain node types.
  if (ast->get_type() != ASTNode::Type::Globally) {
    return -1;
  }
  shared_ptr<Globally> globally = static_pointer_cast<Globally>(ast);

  // get lower/upper bound of root node
  size_t lb = globally->get_lower_bound();
  size_t ub = globally->get_upper_bound();
  cout << "lower bound: " << lb << "\n";
  cout << "upper bound: " << ub << "\n";

  // get operand, for binary operators use get_left/get_right
  cout << "operand: " << globally->get_operand().as_string() << "\n";
  // count total operators or variables in formula
  cout << "num variables: " << ast->count(ASTNode::Type::Variable) << "\n";

  // copy
  shared_ptr<ASTNode> copy = ast->deep_copy();

  // let's modify the ast by changing the operand
  shared_ptr<ASTNode> new_op = parse("(~p1&p0|F[0,2](p1))");
  static_pointer_cast<Globally>(copy)->set_operand(new_op);

  cout << "original formula: " << ast->as_pretty_string() << "\n";
  cout << "modified formula: " << copy->as_pretty_string() << "\n";

  // build formula without parser
  shared_ptr<Finally> new_finally = make_shared<Finally>();
  new_finally->set_lower_bound(0);
  new_finally->set_upper_bound(2);
  new_finally->set_operand(make_shared<Variable>(1)); // p1
  cout << "newly built formula: " << new_finally->as_pretty_string() << "\n";

  // use comparison operators: ==, !=, <, >, <=, >=
  result = *parse("(F[0,2](p3))") == *parse("(F[0,2](p1))");
  cout << (result ? "true" : "false") << "\n";
  // lexicographical compare
  result = *parse("(F[0,2](p3))") < *parse("(F[0,3](p3))");
  cout << (result ? "true" : "false") << "\n";

  // error example (illegal bounds)
  cout << "Example syntax error: ";
  try {
    ast = parse("G[11,10](~p1)");
  } catch (syntax_error &e) {
    cout << e.what() << "\n";
  }

  return 0;
}
