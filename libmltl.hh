#pragma once

#include <string>
#include <vector>

using namespace std;

enum class MLTLNodeType {
  PropCons,
  PropVar,
  UnaryPropOp,
  BinaryPropOp,
  UnaryTempOp,
  BinaryTempOp,
};

enum class MLTLUnaryPropOpType {
  Neg,
};

enum class MLTLBinaryPropOpType {
  And, // highest precedence
  Xor,
  Or,
  Implies,
  Equiv, // lowest precedence
};

enum class MLTLUnaryTempOpType {
  Finally,
  Globally,
};

enum class MLTLBinaryTempOpType {
  Until,
  Release,
};

class MLTLNode;

/* Parses a string representing an MLTL formula. Returns a pointer to the root
 * node of the AST representation. To avoid memory leaks delete must be called
 * on the pointer when it is no longer needed. Calling delete will recursively
 * free all nodes in the AST.
 *
 * On an invalid MLTL formula, program will exit and print syntax error.
 */
MLTLNode *parse(const string &f);

/* Reads a trace from file.
 *
 * Expected file format:
 *   Each line consists of comma separated 0/1's representing the truth value of
 *   p0,p1,p2,...,pi. The state at the first time step of the trace is the first
 *   line, the second time step is the second line, and so on.
 *
 *   ex:
 *     0,1,1
 *     1,1,1
 *     0,1,0
 *     0,1,1
 *     0,0,0
 *
 *     Returns {"011","111","010","011"."000"}
 */
vector<string> read_trace_file(const string &trace_file_path);

/* Reads all files in a directory and parses them as traces.
 */
vector<vector<string>> read_trace_files(const string &trace_directory_path);

/* Takes an integer and returns a string of 0s and 1s corresponding to the
 * binary value of n. The string will be zero left-padded or truncated to
 * length. Useful function for generating traces. width. ex: int_to_bin_str(11,
 * 5) == "01011"
 */
string int_to_bin_str(unsigned int n, int width);

class MLTLNode {
protected:
  const MLTLNodeType type;
  MLTLNode(MLTLNodeType type);

public:
  MLTLNodeType getType() const;

  virtual ~MLTLNode() = 0;
  virtual string as_string() const = 0;
  virtual size_t future_reach() const = 0;
  virtual bool evaluate(const vector<string> &trace) const = 0;
  virtual size_t size() const = 0;
  virtual size_t count(MLTLNodeType target_type) const = 0;
  virtual size_t count(MLTLUnaryPropOpType target_type) const = 0;
  virtual size_t count(MLTLBinaryPropOpType target_type) const = 0;
  virtual size_t count(MLTLUnaryTempOpType target_type) const = 0;
  virtual size_t count(MLTLBinaryTempOpType target_type) const = 0;
};

class MLTLPropConsNode : public MLTLNode {
public:
  bool val;

  MLTLPropConsNode(bool val);
  ~MLTLPropConsNode();

  string as_string() const;
  size_t future_reach() const;
  bool evaluate(const vector<string> &trace) const;
  size_t size() const;
  size_t count(MLTLNodeType target_type) const;
  size_t count(MLTLUnaryPropOpType target_type) const;
  size_t count(MLTLBinaryPropOpType target_type) const;
  size_t count(MLTLUnaryTempOpType target_type) const;
  size_t count(MLTLBinaryTempOpType target_type) const;
};

class MLTLPropVarNode : public MLTLNode {
public:
  size_t var_id;

  MLTLPropVarNode(unsigned int var_id);
  ~MLTLPropVarNode();

  string as_string() const;
  size_t future_reach() const;
  bool evaluate(const vector<string> &trace) const;
  size_t size() const;
  size_t count(MLTLNodeType target_type) const;
  size_t count(MLTLUnaryPropOpType target_type) const;
  size_t count(MLTLBinaryPropOpType target_type) const;
  size_t count(MLTLUnaryTempOpType target_type) const;
  size_t count(MLTLBinaryTempOpType target_type) const;
};

class MLTLUnaryPropOpNode : public MLTLNode {
public:
  MLTLUnaryPropOpType op_type;
  MLTLNode *operand;

  MLTLUnaryPropOpNode(MLTLUnaryPropOpType op_type, MLTLNode *operand);
  ~MLTLUnaryPropOpNode();

  string as_string() const;
  size_t future_reach() const;
  bool evaluate(const vector<string> &trace) const;
  size_t size() const;
  size_t count(MLTLNodeType target_type) const;
  size_t count(MLTLUnaryPropOpType target_type) const;
  size_t count(MLTLBinaryPropOpType target_type) const;
  size_t count(MLTLUnaryTempOpType target_type) const;
  size_t count(MLTLBinaryTempOpType target_type) const;
};

class MLTLBinaryPropOpNode : public MLTLNode {
public:
  MLTLBinaryPropOpType op_type;
  MLTLNode *left;
  MLTLNode *right;

  MLTLBinaryPropOpNode(MLTLBinaryPropOpType op_type, MLTLNode *left,
                       MLTLNode *right);
  ~MLTLBinaryPropOpNode();

  string as_string() const;
  size_t future_reach() const;
  bool evaluate(const vector<string> &trace) const;
  size_t size() const;
  size_t count(MLTLNodeType target_type) const;
  size_t count(MLTLUnaryPropOpType target_type) const;
  size_t count(MLTLBinaryPropOpType target_type) const;
  size_t count(MLTLUnaryTempOpType target_type) const;
  size_t count(MLTLBinaryTempOpType target_type) const;
};

class MLTLUnaryTempOpNode : public MLTLNode {
public:
  MLTLUnaryTempOpType op_type;
  size_t lb;
  size_t ub;
  MLTLNode *operand;

  MLTLUnaryTempOpNode(MLTLUnaryTempOpType op_type, size_t lb, size_t ub,
                      MLTLNode *operand);
  ~MLTLUnaryTempOpNode();

  string as_string() const;
  size_t future_reach() const;
  bool evaluate(const vector<string> &trace) const;
  size_t size() const;
  size_t count(MLTLNodeType target_type) const;
  size_t count(MLTLUnaryPropOpType target_type) const;
  size_t count(MLTLBinaryPropOpType target_type) const;
  size_t count(MLTLUnaryTempOpType target_type) const;
  size_t count(MLTLBinaryTempOpType target_type) const;
};

class MLTLBinaryTempOpNode : public MLTLNode {
public:
  MLTLBinaryTempOpType op_type;
  size_t lb;
  size_t ub;
  MLTLNode *left;
  MLTLNode *right;

  MLTLBinaryTempOpNode(MLTLBinaryTempOpType op_type, size_t lb, size_t ub,
                       MLTLNode *left, MLTLNode *right);
  ~MLTLBinaryTempOpNode();

  string as_string() const;
  size_t future_reach() const;
  bool evaluate(const vector<string> &trace) const;
  size_t size() const;
  size_t count(MLTLNodeType target_type) const;
  size_t count(MLTLUnaryPropOpType target_type) const;
  size_t count(MLTLBinaryPropOpType target_type) const;
  size_t count(MLTLUnaryTempOpType target_type) const;
  size_t count(MLTLBinaryTempOpType target_type) const;
};
