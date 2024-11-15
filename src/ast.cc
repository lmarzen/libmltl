#include "ast.hh"

using namespace std;
namespace libmltl {

Constant::Constant(bool value) : val(value) {}

ASTNode::Type Constant::get_type() const { return ASTNode::Type::Constant; }
bool Constant::is_unary_op() const { return false; }
bool Constant::is_binary_op() const { return false; }
bool Constant::is_propositional_op() const { return false; }
bool Constant::is_temporal_op() const { return false; }
std::string Constant::as_string() const { return val ? "true" : "false"; }
std::string Constant::as_pretty_string() const { return as_string(); }
std::string Constant::get_symbol() const { return as_string(); }
bool Constant::evaluate_subt(
    [[maybe_unused]] const std::vector<std::string> &trace,
    [[maybe_unused]] size_t begin, [[maybe_unused]] size_t end) const {
  return val;
}
size_t Constant::future_reach() const { return 0; }
size_t Constant::size() const { return 1; }
size_t Constant::depth() const { return 0; }
size_t Constant::count(ASTNode::Type target_type) const {
  return (get_type() == target_type);
}
std::shared_ptr<ASTNode> Constant::deep_copy() const {
  return std::make_shared<Constant>(val);
}
bool Constant::operator==(const ASTNode &other) const {
  return ((get_type() == other.get_type()) &&
          (val == static_cast<const Constant &>(other).val));
}
bool Constant::operator<(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() < other.get_type());
  } else {
    return (val < static_cast<const Constant &>(other).val);
  }
}
bool Constant::operator<=(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() <= other.get_type());
  } else {
    return (val <= static_cast<const Constant &>(other).val);
  }
}

Variable::Variable(unsigned int id) : id(id) {}

ASTNode::Type Variable::get_type() const { return ASTNode::Type::Variable; }
bool Variable::is_unary_op() const { return false; }
bool Variable::is_binary_op() const { return false; }
bool Variable::is_propositional_op() const { return false; }
bool Variable::is_temporal_op() const { return false; }
std::string Variable::as_string() const { return 'p' + std::to_string(id); }
std::string Variable::as_pretty_string() const { return as_string(); }
std::string Variable::get_symbol() const { return as_string(); }
bool Variable::evaluate_subt(const std::vector<std::string> &trace,
                             size_t begin, size_t end) const {
  if (begin == end || id >= trace[0].length()) {
    return false;
  }
  return (trace[begin][id] == '1');
}
size_t Variable::future_reach() const { return 1; }
size_t Variable::size() const { return 1; }
size_t Variable::depth() const { return 0; }
size_t Variable::count(ASTNode::Type target_type) const {
  return (get_type() == target_type);
}
std::shared_ptr<ASTNode> Variable::deep_copy() const {
  return std::make_shared<Variable>(id);
}
bool Variable::operator==(const ASTNode &other) const {
  return ((get_type() == other.get_type()) &&
          (id == static_cast<const Variable &>(other).id));
}
bool Variable::operator<(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() < other.get_type());
  } else {
    return (id < static_cast<const Variable &>(other).id);
  }
}
bool Variable::operator<=(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() <= other.get_type());
  } else {
    return (id <= static_cast<const Variable &>(other).id);
  }
}

UnaryOp::UnaryOp() : operand(nullptr) {}
UnaryOp::UnaryOp(std::shared_ptr<ASTNode> operand)
    : operand(std::move(operand)) {}

bool UnaryOp::is_unary_op() const { return true; }
bool UnaryOp::is_binary_op() const { return false; }
size_t UnaryOp::size() const { return 1 + operand->size(); }
size_t UnaryOp::depth() const { return 1 + operand->depth(); }
size_t UnaryOp::count(ASTNode::Type target_type) const {
  return (get_type() == target_type) + operand->count(target_type);
}

bool UnaryPropOp::is_propositional_op() const { return true; }
bool UnaryPropOp::is_temporal_op() const { return false; }
std::string UnaryPropOp::as_string() const {
  return get_symbol() + "(" + operand->as_string() + ")";
}
std::string UnaryPropOp::as_pretty_string() const {
  if (operand->is_binary_op()) {
    return get_symbol() + "(" + operand->as_pretty_string() + ")";
  }
  return get_symbol() + operand->as_pretty_string();
}
size_t UnaryPropOp::future_reach() const { return operand->future_reach(); }
bool UnaryPropOp::operator==(const ASTNode &other) const {
  return ((get_type() == other.get_type()) &&
          (*operand == *static_cast<const UnaryPropOp &>(other).operand));
}
bool UnaryPropOp::operator<(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() < other.get_type());
  } else {
    return (*operand < *static_cast<const UnaryPropOp &>(other).operand);
  }
}
bool UnaryPropOp::operator<=(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() <= other.get_type());
  } else {
    return (*operand <= *static_cast<const UnaryPropOp &>(other).operand);
  }
}

ASTNode::Type Negation::get_type() const { return ASTNode::Type::Negation; }
std::string Negation::get_symbol() const { return "~"; }
bool Negation::evaluate_subt(const std::vector<std::string> &trace,
                             size_t begin, size_t end) const {
  return !operand->evaluate_subt(trace, begin, end);
}
std::shared_ptr<ASTNode> Negation::deep_copy() const {
  return std::make_shared<Negation>(operand->deep_copy());
}

UnaryTempOp::UnaryTempOp() : UnaryOp(), lb(0), ub(0) {}
UnaryTempOp::UnaryTempOp(std::shared_ptr<ASTNode> operand, size_t lb, size_t ub)
    : UnaryOp(std::move(operand)), lb(lb), ub(ub) {}

bool UnaryTempOp::is_propositional_op() const { return false; }
bool UnaryTempOp::is_temporal_op() const { return true; }
std::string UnaryTempOp::as_string() const {
  return get_symbol() + "[" + std::to_string(lb) + "," + std::to_string(ub) +
         "](" + operand->as_string() + ")";
}
std::string UnaryTempOp::as_pretty_string() const {
  if (operand->is_binary_op()) {
    return get_symbol() + "[" + std::to_string(lb) + "," + std::to_string(ub) +
           "](" + operand->as_pretty_string() + ")";
  }
  return get_symbol() + "[" + std::to_string(lb) + "," + std::to_string(ub) +
         "]" + operand->as_pretty_string();
}
size_t UnaryTempOp::future_reach() const {
  return ub + operand->future_reach();
}
bool UnaryTempOp::operator==(const ASTNode &other) const {
  return ((get_type() == other.get_type()) &&
          (*operand == *static_cast<const UnaryTempOp &>(other).operand) &&
          (lb == static_cast<const UnaryTempOp &>(other).lb) &&
          (ub == static_cast<const UnaryTempOp &>(other).ub));
}
bool UnaryTempOp::operator<(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() < other.get_type());
  } else if (*operand != *static_cast<const UnaryTempOp &>(other).operand) {
    return (*operand < *static_cast<const UnaryTempOp &>(other).operand);
  } else if (lb != static_cast<const UnaryTempOp &>(other).lb) {
    return (lb < static_cast<const UnaryTempOp &>(other).lb);
  } else {
    return (ub < static_cast<const UnaryTempOp &>(other).ub);
  }
}
bool UnaryTempOp::operator<=(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() <= other.get_type());
  } else if (*operand != *static_cast<const UnaryTempOp &>(other).operand) {
    return (*operand <= *static_cast<const UnaryTempOp &>(other).operand);
  } else if (lb != static_cast<const UnaryTempOp &>(other).lb) {
    return (lb <= static_cast<const UnaryTempOp &>(other).lb);
  } else {
    return (ub <= static_cast<const UnaryTempOp &>(other).ub);
  }
}

ASTNode::Type Finally::get_type() const { return ASTNode::Type::Finally; }
std::string Finally::get_symbol() const { return "F"; }
bool Finally::evaluate_subt(const vector<string> &trace, size_t begin,
                            size_t end) const {
  size_t subt_len = end - begin;
  if (subt_len <= lb) {
    return false;
  }
  size_t idx_lb = begin + lb;
  size_t idx_ub = begin + ub;
  size_t idx_end = min(idx_ub + 1, end);
  for (size_t i = idx_lb; i < idx_end; ++i) {
    if (operand->evaluate_subt(trace, i, end)) {
      return true;
    }
  }
  return false;
}
std::shared_ptr<ASTNode> Finally::deep_copy() const {
  return std::make_shared<Finally>(operand->deep_copy(), lb, ub);
}

ASTNode::Type Globally::get_type() const { return ASTNode::Type::Globally; }
std::string Globally::get_symbol() const { return "G"; }
bool Globally::evaluate_subt(const vector<string> &trace, size_t begin,
                             size_t end) const {
  size_t subt_len = end - begin;
  if (subt_len <= lb) {
    return true;
  }
  size_t idx_lb = begin + lb;
  size_t idx_ub = begin + ub;
  size_t idx_end = min(idx_ub + 1, end);
  for (size_t i = idx_lb; i < idx_end; ++i) {
    if (!operand->evaluate_subt(trace, i, end)) {
      return false;
    }
  }
  return true;
}
std::shared_ptr<ASTNode> Globally::deep_copy() const {
  return std::make_shared<Globally>(operand->deep_copy(), lb, ub);
}

BinaryOp::BinaryOp() : left(nullptr), right(nullptr) {}
BinaryOp::BinaryOp(std::shared_ptr<ASTNode> left,
                   std::shared_ptr<ASTNode> right)
    : left(std::move(left)), right(std::move(right)) {}

bool BinaryOp::is_unary_op() const { return false; }
bool BinaryOp::is_binary_op() const { return true; }
size_t BinaryOp::size() const { return 1 + left->size() + right->size(); }
size_t BinaryOp::depth() const {
  return 1 + std::max(left->depth(), right->depth());
}
size_t BinaryOp::count(ASTNode::Type target_type) const {
  return (get_type() == target_type) + left->count(target_type) +
         right->count(target_type);
}

bool BinaryPropOp::is_propositional_op() const { return true; }
bool BinaryPropOp::is_temporal_op() const { return false; }
std::string BinaryPropOp::as_string() const {
  return "(" + left->as_string() + ")" + get_symbol() + "(" +
         right->as_string() + ")";
}
std::string BinaryPropOp::as_pretty_string() const {
  std::string result = "";
  if (left->is_binary_op() && (get_type() != left->get_type())) {
    result += "(" + left->as_pretty_string() + ")";
  } else {
    result += left->as_pretty_string();
  }
  result += " " + get_symbol() + " ";
  if (right->is_binary_op() && (get_type() != right->get_type())) {
    result += "(" + right->as_pretty_string() + ")";
  } else {
    result += right->as_pretty_string();
  }
  return result;
}
size_t BinaryPropOp::future_reach() const {
  return std::max(left->future_reach(), right->future_reach());
}
bool BinaryPropOp::operator==(const ASTNode &other) const {
  return ((get_type() == other.get_type()) &&
          (*left == *static_cast<const BinaryPropOp &>(other).left) &&
          (*right == *static_cast<const BinaryPropOp &>(other).right));
}
bool BinaryPropOp::operator<(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() < other.get_type());
  } else if (*left != *static_cast<const BinaryPropOp &>(other).left) {
    return (*left < *static_cast<const BinaryPropOp &>(other).left);
  } else {
    return (*right < *static_cast<const BinaryPropOp &>(other).right);
  }
}
bool BinaryPropOp::operator<=(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() <= other.get_type());
  } else if (*left != *static_cast<const BinaryPropOp &>(other).left) {
    return (*left <= *static_cast<const BinaryPropOp &>(other).left);
  } else {
    return (*right <= *static_cast<const BinaryPropOp &>(other).right);
  }
}

ASTNode::Type And::get_type() const { return ASTNode::Type::And; }
std::string And::get_symbol() const { return "&"; }
bool And::evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                        size_t end) const {
  return left->evaluate_subt(trace, begin, end) &&
         right->evaluate_subt(trace, begin, end);
}
std::shared_ptr<ASTNode> And::deep_copy() const {
  return std::make_shared<And>(left->deep_copy(), right->deep_copy());
}

ASTNode::Type Xor::get_type() const { return ASTNode::Type::Xor; }
std::string Xor::get_symbol() const { return "^"; }
bool Xor::evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                        size_t end) const {
  return left->evaluate_subt(trace, begin, end) !=
         right->evaluate_subt(trace, begin, end);
}
std::shared_ptr<ASTNode> Xor::deep_copy() const {
  return std::make_shared<Xor>(left->deep_copy(), right->deep_copy());
}

ASTNode::Type Or::get_type() const { return ASTNode::Type::Or; }
std::string Or::get_symbol() const { return "|"; }
bool Or::evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                       size_t end) const {
  return left->evaluate_subt(trace, begin, end) ||
         right->evaluate_subt(trace, begin, end);
}
std::shared_ptr<ASTNode> Or::deep_copy() const {
  return std::make_shared<Or>(left->deep_copy(), right->deep_copy());
}

ASTNode::Type Implies::get_type() const { return ASTNode::Type::Implies; }
std::string Implies::get_symbol() const { return "->"; }
bool Implies::evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                            size_t end) const {
  return !left->evaluate_subt(trace, begin, end) ||
         right->evaluate_subt(trace, begin, end);
}
std::shared_ptr<ASTNode> Implies::deep_copy() const {
  return std::make_shared<Implies>(left->deep_copy(), right->deep_copy());
}

ASTNode::Type Equiv::get_type() const { return ASTNode::Type::Equiv; }
std::string Equiv::get_symbol() const { return "<->"; }
bool Equiv::evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                          size_t end) const {
  return left->evaluate_subt(trace, begin, end) ==
         right->evaluate_subt(trace, begin, end);
}
std::shared_ptr<ASTNode> Equiv::deep_copy() const {
  return std::make_shared<Equiv>(left->deep_copy(), right->deep_copy());
}

BinaryTempOp::BinaryTempOp() : BinaryOp(), lb(0), ub(0) {}
BinaryTempOp::BinaryTempOp(std::shared_ptr<ASTNode> left,
                           std::shared_ptr<ASTNode> right, size_t lb, size_t ub)
    : BinaryOp(std::move(left), std::move(right)), lb(lb), ub(ub) {}

bool BinaryTempOp::is_propositional_op() const { return false; }
bool BinaryTempOp::is_temporal_op() const { return true; }
std::string BinaryTempOp::as_string() const {
  return '(' + left->as_string() + ")" + get_symbol() + "[" +
         std::to_string(lb) + "," + std::to_string(ub) + "](" +
         right->as_string() + ")";
}
std::string BinaryTempOp::as_pretty_string() const {
  std::string result = "";
  if (left->is_binary_op()) {
    result += "(" + left->as_pretty_string() + ")";
  } else {
    result += left->as_pretty_string();
  }
  result += " " + get_symbol() + "[" + std::to_string(lb) + "," +
            std::to_string(ub) + "] ";
  if (right->is_binary_op()) {
    result += "(" + right->as_pretty_string() + ")";
  } else {
    result += right->as_pretty_string();
  }
  return result;
}
size_t BinaryTempOp::future_reach() const {
  // need to be careful here to avoid an underflow when subtracting 1
  size_t lfr = left->future_reach();
  size_t rfr = right->future_reach();
  if (lfr > rfr) {
    return ub + lfr - 1;
  }
  return ub + rfr;
}
bool BinaryTempOp::operator==(const ASTNode &other) const {
  return ((get_type() == other.get_type()) &&
          (*left == *static_cast<const BinaryTempOp &>(other).left) &&
          (*right == *static_cast<const BinaryTempOp &>(other).right) &&
          (lb == static_cast<const BinaryTempOp &>(other).lb) &&
          (ub == static_cast<const BinaryTempOp &>(other).ub));
}
bool BinaryTempOp::operator<(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() < other.get_type());
  } else if (*left != *static_cast<const BinaryTempOp &>(other).left) {
    return (*left < *static_cast<const BinaryTempOp &>(other).left);
  } else if (*right != *static_cast<const BinaryTempOp &>(other).right) {
    return (*right < *static_cast<const BinaryTempOp &>(other).right);
  } else if (lb != static_cast<const BinaryTempOp &>(other).lb) {
    return (lb < static_cast<const BinaryTempOp &>(other).lb);
  } else {
    return (ub < static_cast<const BinaryTempOp &>(other).ub);
  }
}
bool BinaryTempOp::operator<=(const ASTNode &other) const {
  if (get_type() != other.get_type()) {
    return (get_type() <= other.get_type());
  } else if (*left != *static_cast<const BinaryTempOp &>(other).left) {
    return (*left <= *static_cast<const BinaryTempOp &>(other).left);
  } else if (*right != *static_cast<const BinaryTempOp &>(other).right) {
    return (*right <= *static_cast<const BinaryTempOp &>(other).right);
  } else if (lb != static_cast<const BinaryTempOp &>(other).lb) {
    return (lb <= static_cast<const BinaryTempOp &>(other).lb);
  } else {
    return (ub <= static_cast<const BinaryTempOp &>(other).ub);
  }
}

ASTNode::Type Until::get_type() const { return ASTNode::Type::Until; }
std::string Until::get_symbol() const { return "U"; }
bool Until::evaluate_subt(const vector<string> &trace, size_t begin,
                          size_t end) const {
  size_t subt_len = end - begin;
  if (subt_len <= lb) {
    return false;
  }
  size_t idx_lb = begin + lb;
  size_t idx_ub = begin + ub;
  size_t idx_end = min(idx_ub + 1, end);
  size_t i = -1;
  for (size_t k = idx_lb; k < idx_end; ++k) {
    if (right->evaluate_subt(trace, k, end)) {
      i = k;
      break;
    }
  }
  if (i == (size_t)-1) {
    return false;
  }
  for (size_t j = idx_lb; j < i; ++j) {
    if (!left->evaluate_subt(trace, j, end)) {
      return false;
    }
  }
  return true;
}
std::shared_ptr<ASTNode> Until::deep_copy() const {
  return std::make_shared<Until>(left->deep_copy(), right->deep_copy(), lb, ub);
}

ASTNode::Type Release::get_type() const { return ASTNode::Type::Release; }
std::string Release::get_symbol() const { return "R"; }
bool Release::evaluate_subt(const vector<string> &trace, size_t begin,
                            size_t end) const {
  size_t subt_len = end - begin;
  if (subt_len <= lb) {
    return true;
  }
  size_t idx_lb = begin + lb;
  size_t idx_ub = begin + ub;
  size_t idx_end = min(idx_ub + 1, end);
  size_t i;
  for (i = idx_lb; i < idx_end; ++i) {
    if (!right->evaluate_subt(trace, i, end)) {
      break;
    }
  }
  if (i == idx_end) {
    return true;
  }
  size_t j = -1;
  size_t k;
  for (k = idx_lb; k < idx_end; ++k) {
    if (left->evaluate_subt(trace, k, end)) {
      j = k + 1;
      break;
    }
  }
  if (k == idx_end) {
    j = k + 1;
  } else if (j == (size_t)-1) {
    return false;
  }
  idx_end = min(j, end);
  for (k = idx_lb; k < idx_end; ++k) {
    if (!right->evaluate_subt(trace, k, end)) {
      return false;
    }
  }
  return true;
}
std::shared_ptr<ASTNode> Release::deep_copy() const {
  return std::make_shared<Release>(left->deep_copy(), right->deep_copy(), lb,
                                   ub);
}

} // namespace libmltl
