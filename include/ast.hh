#pragma once

#include <memory>
#include <string>
#include <vector>

namespace libmltl {

class ASTNode {
public:
  enum class Type {
    Constant, // true,false
    Variable, // p0,p1,p2,...
    Negation, // ~
    And,      // &
    Xor,      // ^
    Or,       // |
    Implies,  // ->
    Equiv,    // <->
    Finally,  // F
    Globally, // G
    Until,    // U
    Release,  // R
  };

  virtual ASTNode::Type get_type() const = 0;
  virtual bool is_unary_op() const = 0;
  virtual bool is_binary_op() const = 0;
  virtual bool is_propositional_op() const = 0;
  virtual bool is_temporal_op() const = 0;
  virtual std::string as_string() const = 0;
  virtual std::string as_pretty_string() const = 0;
  virtual std::string get_symbol() const = 0;
  /* Evaluates as trace over time steps [begin, end).
   */
  virtual bool evaluate_subt(const std::vector<std::string> &trace,
                             size_t begin, size_t end) const = 0;
  bool evaluate(const std::vector<std::string> &trace) const {
    return evaluate_subt(trace, 0, trace.size());
  };
  /* Mission-time LTL (MLTL) Formula Validation Via Regular Expressions
   * https://temporallogic.org/research/WEST/WEST_extended.pdf
   * Definition 6
   */
  virtual size_t future_reach() const = 0;
  virtual size_t size() const = 0;
  virtual size_t depth() const = 0;
  virtual size_t count(ASTNode::Type target_type) const = 0;
  virtual std::shared_ptr<ASTNode> deep_copy() const = 0;
  virtual bool operator==(const ASTNode &other) const = 0;
  virtual bool operator!=(const ASTNode &other) const {
    return !(*this == other);
  }
  virtual bool operator<(const ASTNode &other) const = 0;
  bool operator>(const ASTNode &other) const { return other < *this; }
  virtual bool operator<=(const ASTNode &other) const = 0;
  bool operator>=(const ASTNode &other) const { return other <= *this; }
  virtual ~ASTNode() = default;
};

class Constant : public ASTNode {
private:
  bool val;

public:
  Constant(bool value);

  bool get_value() const { return val; }
  void set_value(bool new_value) { val = new_value; }

  // virtual functions
  ASTNode::Type get_type() const;
  bool is_unary_op() const;
  bool is_binary_op() const;
  bool is_propositional_op() const;
  bool is_temporal_op() const;
  std::string as_string() const;
  std::string as_pretty_string() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  size_t future_reach() const;
  size_t size() const;
  size_t depth() const;
  size_t count(ASTNode::Type target_type) const;
  std::shared_ptr<ASTNode> deep_copy() const;
  bool operator==(const ASTNode &other) const;
  bool operator<(const ASTNode &other) const;
  bool operator<=(const ASTNode &other) const;
};

class Variable : public ASTNode {
private:
  unsigned int id;

public:
  Variable(unsigned int id);

  unsigned int get_id() const { return id; }
  void set_id(unsigned int new_id) { id = new_id; }

  // virtual functions
  ASTNode::Type get_type() const;
  bool is_unary_op() const;
  bool is_binary_op() const;
  bool is_propositional_op() const;
  bool is_temporal_op() const;
  std::string as_string() const;
  std::string as_pretty_string() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  size_t future_reach() const;
  size_t size() const;
  size_t depth() const;
  size_t count(ASTNode::Type target_type) const;
  std::shared_ptr<ASTNode> deep_copy() const;
  bool operator==(const ASTNode &other) const;
  bool operator<(const ASTNode &other) const;
  bool operator<=(const ASTNode &other) const;
};

class UnaryOp : public ASTNode {
protected:
  std::shared_ptr<ASTNode> operand;

public:
  UnaryOp();
  UnaryOp(std::shared_ptr<ASTNode> operand);

  const ASTNode &get_operand() const { return *operand; }
  ASTNode &get_operand() { return *operand; }
  void set_operand(std::shared_ptr<ASTNode> new_operand) {
    operand = std::move(new_operand);
  }

  // virtual functions
  bool is_unary_op() const;
  bool is_binary_op() const;
  size_t size() const;
  size_t depth() const;
  size_t count(ASTNode::Type target_type) const;

  virtual ASTNode::Type get_type() const = 0;
  virtual bool is_propositional_op() const = 0;
  virtual bool is_temporal_op() const = 0;
  virtual std::string as_string() const = 0;
  virtual std::string as_pretty_string() const = 0;
  virtual std::string get_symbol() const = 0;
  virtual bool evaluate_subt(const std::vector<std::string> &trace,
                             size_t begin, size_t end) const = 0;
  virtual size_t future_reach() const = 0;
  virtual std::shared_ptr<ASTNode> deep_copy() const = 0;
  virtual bool operator==(const ASTNode &other) const = 0;
  virtual bool operator<(const ASTNode &other) const = 0;
  virtual bool operator<=(const ASTNode &other) const = 0;
};

class UnaryPropOp : public UnaryOp {
public:
  using UnaryOp::UnaryOp; // inherit base-class constructors

  // virtual functions
  bool is_propositional_op() const;
  bool is_temporal_op() const;
  std::string as_string() const;
  std::string as_pretty_string() const;
  size_t future_reach() const;
  bool operator==(const ASTNode &other) const;
  bool operator<(const ASTNode &other) const;
  bool operator<=(const ASTNode &other) const;

  virtual ASTNode::Type get_type() const = 0;
  virtual std::string get_symbol() const = 0;
  virtual bool evaluate_subt(const std::vector<std::string> &trace,
                             size_t begin, size_t end) const = 0;
  virtual std::shared_ptr<ASTNode> deep_copy() const = 0;
};

class Negation : public UnaryPropOp {
public:
  using UnaryPropOp::UnaryPropOp; // inherit base-class constructors

  // virtual functions
  ASTNode::Type get_type() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  std::shared_ptr<ASTNode> deep_copy() const;
};

class UnaryTempOp : public UnaryOp {
protected:
  size_t lb, ub;

public:
  UnaryTempOp();
  UnaryTempOp(std::shared_ptr<ASTNode> operand, size_t lb, size_t ub);

  size_t get_lower_bound() const { return lb; }
  size_t get_upper_bound() const { return ub; }
  void set_lower_bound(size_t new_lb) { lb = new_lb; }
  void set_upper_bound(size_t new_ub) { ub = new_ub; }

  // virtual functions
  bool is_propositional_op() const;
  bool is_temporal_op() const;
  std::string as_string() const;
  std::string as_pretty_string() const;
  size_t future_reach() const;
  bool operator==(const ASTNode &other) const;
  bool operator<(const ASTNode &other) const;
  bool operator<=(const ASTNode &other) const;

  virtual ASTNode::Type get_type() const = 0;
  virtual std::string get_symbol() const = 0;
  virtual bool evaluate_subt(const std::vector<std::string> &trace,
                             size_t begin, size_t end) const = 0;
  virtual std::shared_ptr<ASTNode> deep_copy() const = 0;
};

class Finally : public UnaryTempOp {
public:
  using UnaryTempOp::UnaryTempOp; // inherit base-class constructors

  // virtual functions
  ASTNode::Type get_type() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  std::shared_ptr<ASTNode> deep_copy() const;
};

class Globally : public UnaryTempOp {
public:
  using UnaryTempOp::UnaryTempOp; // inherit base-class constructors

  // virtual functions
  ASTNode::Type get_type() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  std::shared_ptr<ASTNode> deep_copy() const;
};

class BinaryOp : public ASTNode {
protected:
  std::shared_ptr<ASTNode> left, right;

public:
  BinaryOp();
  BinaryOp(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);

  const ASTNode &get_left() const { return *left; }
  const ASTNode &get_right() const { return *right; }
  ASTNode &get_left() { return *left; }
  ASTNode &get_right() { return *right; }
  void set_left(std::shared_ptr<ASTNode> new_left) {
    left = std::move(new_left);
  }
  void set_right(std::shared_ptr<ASTNode> new_right) {
    left = std::move(new_right);
  }

  // virtual functions
  bool is_unary_op() const;
  bool is_binary_op() const;
  size_t size() const;
  size_t depth() const;
  size_t count(ASTNode::Type target_type) const;

  virtual ASTNode::Type get_type() const = 0;
  virtual bool is_propositional_op() const = 0;
  virtual bool is_temporal_op() const = 0;
  virtual std::string as_string() const = 0;
  virtual std::string as_pretty_string() const = 0;
  virtual std::string get_symbol() const = 0;
  virtual bool evaluate_subt(const std::vector<std::string> &trace,
                             size_t begin, size_t end) const = 0;
  virtual size_t future_reach() const = 0;
  virtual std::shared_ptr<ASTNode> deep_copy() const = 0;
  virtual bool operator==(const ASTNode &other) const = 0;
  virtual bool operator<(const ASTNode &other) const = 0;
  virtual bool operator<=(const ASTNode &other) const = 0;
};

class BinaryPropOp : public BinaryOp {
public:
  using BinaryOp::BinaryOp; // inherit base-class constructor

  // virtual functions
  bool is_propositional_op() const;
  bool is_temporal_op() const;
  std::string as_string() const;
  std::string as_pretty_string() const;
  size_t future_reach() const;
  bool operator==(const ASTNode &other) const;
  bool operator<(const ASTNode &other) const;
  bool operator<=(const ASTNode &other) const;

  virtual ASTNode::Type get_type() const = 0;
  virtual std::string get_symbol() const = 0;
  virtual bool evaluate_subt(const std::vector<std::string> &trace,
                             size_t begin, size_t end) const = 0;
  virtual std::shared_ptr<ASTNode> deep_copy() const = 0;
};

class And : public BinaryPropOp {
public:
  using BinaryPropOp::BinaryPropOp; // inherit base-class constructors

  // virtual functions
  ASTNode::Type get_type() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  std::shared_ptr<ASTNode> deep_copy() const;
};

class Xor : public BinaryPropOp {
public:
  using BinaryPropOp::BinaryPropOp; // inherit base-class constructors

  // virtual functions
  ASTNode::Type get_type() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  std::shared_ptr<ASTNode> deep_copy() const;
};

class Or : public BinaryPropOp {
public:
  using BinaryPropOp::BinaryPropOp; // inherit base-class constructors

  // virtual functions
  ASTNode::Type get_type() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  std::shared_ptr<ASTNode> deep_copy() const;
};

class Implies : public BinaryPropOp {
public:
  using BinaryPropOp::BinaryPropOp; // inherit base-class constructors

  // virtual functions
  ASTNode::Type get_type() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  std::shared_ptr<ASTNode> deep_copy() const;
};

class Equiv : public BinaryPropOp {
public:
  using BinaryPropOp::BinaryPropOp; // inherit base-class constructors

  // virtual functions
  ASTNode::Type get_type() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  std::shared_ptr<ASTNode> deep_copy() const;
};

class BinaryTempOp : public BinaryOp {
protected:
  size_t lb, ub;

public:
  BinaryTempOp();
  BinaryTempOp(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right,
               size_t lb, size_t ub);

  size_t get_lower_bound() const { return lb; }
  size_t get_upper_bound() const { return ub; }
  void set_lower_bound(size_t new_lb) { lb = new_lb; }
  void set_upper_bound(size_t new_ub) { ub = new_ub; }

  // virtual functions
  bool is_propositional_op() const;
  bool is_temporal_op() const;
  std::string as_string() const;
  std::string as_pretty_string() const;
  size_t future_reach() const;
  bool operator==(const ASTNode &other) const;
  bool operator<(const ASTNode &other) const;
  bool operator<=(const ASTNode &other) const;

  virtual ASTNode::Type get_type() const = 0;
  virtual std::string get_symbol() const = 0;
  virtual bool evaluate_subt(const std::vector<std::string> &trace,
                             size_t begin, size_t end) const = 0;
  virtual std::shared_ptr<ASTNode> deep_copy() const = 0;
};

class Until : public BinaryTempOp {
public:
  using BinaryTempOp::BinaryTempOp; // inherit base-class constructors

  // virtual functions
  ASTNode::Type get_type() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  std::shared_ptr<ASTNode> deep_copy() const;
};

class Release : public BinaryTempOp {
public:
  using BinaryTempOp::BinaryTempOp; // inherit base-class constructors

  // virtual functions
  ASTNode::Type get_type() const;
  std::string get_symbol() const;
  bool evaluate_subt(const std::vector<std::string> &trace, size_t begin,
                     size_t end) const;
  std::shared_ptr<ASTNode> deep_copy() const;
};

} // namespace libmltl
