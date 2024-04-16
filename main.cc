#include <cmath>
#include <cstdint>
#include <iostream>
#include <sys/time.h>

#include "libmltl.hh"

using namespace std;


int main(int argc, char *argv[]) {
  // default options
  // TODO

  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];

    if (arg == "-h" || arg == "--help") {
      // TODO
      // eval_func_p1 = stoi(argv[i + 1]);
      // i++;
    } else {
      cerr << "error: unknown option " << arg << endl;
      return 1;
    }
  }

  string base_path = "../dataset/basic_global";
  vector<vector<string>> traces_pos_train =
      read_trace_files(base_path + "/pos_train");
  size_t max_pos_train_trace_len = max_trace_length(traces_pos_train);
  vector<vector<string>> traces_neg_train =
      read_trace_files(base_path + "/neg_train");
  size_t max_neg_train_trace_len = max_trace_length(traces_neg_train);
  vector<vector<string>> traces_pos_test =
      read_trace_files(base_path + "/pos_test");
  size_t max_pos_test_trace_len = max_trace_length(traces_pos_test);
  vector<vector<string>> traces_neg_test =
      read_trace_files(base_path + "/neg_test");
  size_t max_neg_test_trace_len = max_trace_length(traces_neg_test);

  struct timeval start, end;
  double time_taken = 0;
  gettimeofday(&start, NULL); // start timer

  const uint32_t num_vars = 2;
  const uint32_t truth_table_rows = pow(2, num_vars);
  const uint64_t num_boolean_functions = pow(2, pow(2, num_vars));
  vector<string> inputs(truth_table_rows);
  vector<MLTLUnaryTempOpNode *> boolean_functions_asts(num_boolean_functions);
  vector<MLTLBinaryTempOpNode *> boolean_functions_asts_until(
      num_boolean_functions);
  vector<string> boolean_functions_asts_string(num_boolean_functions);
  vector<string> boolean_functions_asts_until_string(num_boolean_functions);

  for (uint32_t i = 0; i < truth_table_rows; ++i) {
    inputs[i] = int_to_bin_str(i, num_vars);
  }

#pragma omp parallel for num_threads(12)
  for (uint64_t i = 0; i < num_boolean_functions; ++i) {
    vector<string> implicants;
    for (uint32_t j = 0; j < truth_table_rows; ++j) {
      if ((i >> j) & 1) {
        implicants.emplace_back(inputs[j]);
      }
    }
    boolean_functions_asts[i] = new MLTLUnaryTempOpNode(
        MLTLUnaryTempOpType::Globally, 0, 10, quine_mccluskey(implicants));
    boolean_functions_asts_string[i] = boolean_functions_asts[i]->as_string();
  }

#pragma omp parallel for num_threads(12)
  for (uint64_t i = 0; i < num_boolean_functions; ++i) {
    vector<string> implicants;
    for (uint32_t j = 0; j < truth_table_rows; ++j) {
      if ((i >> j) & 1) {
        implicants.emplace_back(inputs[j]);
      }
    }
    boolean_functions_asts_until[i] = new MLTLBinaryTempOpNode(
        MLTLBinaryTempOpType::Until, 0, 10, quine_mccluskey(implicants),
        quine_mccluskey(implicants));
    boolean_functions_asts_until_string[i] =
        boolean_functions_asts_until[i]->as_string();
  }

  // #pragma omp parallel for
  // for (int i = 0; i < num_boolean_functions; ++i) {
  //     string formula = "G[0,3](" + boolean_functions[i] + ")";
  //     bool val =
  //         evaluate_mltl(formula, {"0101", "1101", "0101", "1101"}, false);
  // }
  /*
  #pragma omp parallel for num_threads(1)
    for (uint64_t i = 0; i < num_boolean_functions; ++i) {
      // dynamic_cast<MLTLUnaryTempOpNode>(boolean_functions_asts[i])->child;
      // for (int ub = 0; ub < max_pos_train_trace_len; ++ub) {
      //   for (int lb = 0; lb <= ub; ++lb) {
      //     if (ub - lb < 8) {
      //       continue;
      //     }
      int traces_satisified = 0;
      // boolean_functions_asts[i]->lb = lb;
      // boolean_functions_asts[i]->ub = ub;
      boolean_functions_asts[i]->lb = 0;
      boolean_functions_asts[i]->ub = 10;
      for (size_t j = 0; j < traces_pos_train.size(); ++j) {
        traces_satisified +=
            boolean_functions_asts[i]->evaluate(traces_pos_train[j]);
      }
      for (size_t j = 0; j < traces_neg_train.size(); ++j) {
        traces_satisified +=
            !boolean_functions_asts[i]->evaluate(traces_neg_train[j]);
      }

      float accuracy = traces_satisified /
                       (float)(traces_pos_train.size() +
  traces_neg_train.size()); if (accuracy >= 1) { cout <<
  boolean_functions_asts[i]->as_string() << "\n"; cout << "accuracy: " <<
  accuracy << "\n";
      }
      //   }
      // }
    }
  */
  // gettimeofday(&start, NULL); // start timer
  //
  // for (uint64_t i = 0; i < num_boolean_functions; ++i) {
  //   // MLTLNode *new_node = parse(boolean_functions_asts_string[i]);
  //   for (size_t j = 0; j < traces_pos_train.size(); ++j) {
  //     MLTLNode *new_node = parse(boolean_functions_asts_string[i]);
  //     new_node->evaluate(traces_pos_train[j]);
  //     delete new_node;
  //   }
  //   for (size_t j = 0; j < traces_neg_train.size(); ++j) {
  //     MLTLNode *new_node = parse(boolean_functions_asts_string[i]);
  //     new_node->evaluate(traces_neg_train[j]);
  //     delete new_node;
  //   }
  //   // delete new_node;
  // }
  // for (uint64_t i = 0; i < num_boolean_functions; ++i) {
  //   MLTLNode *new_node = parse(boolean_functions_asts_until_string[i]);
  //   for (size_t j = 0; j < traces_pos_train.size(); ++j) {
  //     new_node->evaluate(traces_pos_train[j]);
  //   }
  //   for (size_t j = 0; j < traces_neg_train.size(); ++j) {
  //     new_node->evaluate(traces_neg_train[j]);
  //   }
  //   delete new_node;
  // }

  // gettimeofday(&end, NULL); // stop timer
  // time_taken = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec -
  //              start.tv_usec / 1e6; // in seconds
  // cout << "total time taken: " << time_taken << "s\n";
  // gettimeofday(&start, NULL); // start timer
  //
  // for (uint64_t i = 0; i < num_boolean_functions; ++i) {
  //   for (size_t j = 0; j < traces_pos_train.size(); ++j) {
  //     evaluate_mltl(boolean_functions_asts_string[i], traces_pos_train[j]);
  //   }
  //   for (size_t j = 0; j < traces_neg_train.size(); ++j) {
  //     evaluate_mltl(boolean_functions_asts_string[i], traces_neg_train[j]);
  //   }
  // }
  // for (uint64_t i = 0; i < num_boolean_functions; ++i) {
  //   for (size_t j = 0; j < traces_pos_train.size(); ++j) {
  //     evaluate_mltl(boolean_functions_asts_until_string[i],
  //     traces_pos_train[j]);
  //   }
  //   for (size_t j = 0; j < traces_neg_train.size(); ++j) {
  //     evaluate_mltl(boolean_functions_asts_until_string[i],
  //     traces_neg_train[j]);
  //   }
  // }

  gettimeofday(&end, NULL); // stop timer
  time_taken = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec -
               start.tv_usec / 1e6; // in seconds
  cout << "total time taken: " << time_taken << "s\n";

  // for (int i = 0; i < num_boolean_functions; ++i) {
  //   cout << boolean_functions[i] << "\n";
  // }

  // cout << "total nodes generated: " << numNodes << "\n";
  // cout << "path length: " << path.length() << "\n";
  // cout << "path: " << path << "\n";

  for (MLTLUnaryTempOpNode *p : boolean_functions_asts) {
    delete p;
  }

  MLTLNode *test = parse("true");
  // cout << test->as_string() << "\n";
  // test = parse("false");
  // cout << test->as_string() << "\n";
  // test = parse("(true)");
  // cout << test->as_string() << "\n";
  // test = parse("~(false)");
  // cout << test->as_string() << "\n";
  // // test = parse("~fale");
  // test = parse("G[0,1]true");
  // cout << test->as_string() << "\n";
  // test = parse("F[0,1]!false");
  // cout << test->as_string() << "\n";
  // // test = parse("G[10,0]false");
  // test = parse("~falseU[0,10]!false");
  // cout << test->as_string() << "\n";
  // test = parse("(falseR[0,10](!false))");
  // // test = parse("(falsasdR[0,10](!false))");
  // // test = parse("(falseR[0,10](!falsasdfe))");
  // cout << test->as_string() << "\n";
  // test = parse("false&true");
  // cout << test->as_string() << "\n";
  // test = parse("((false&false)R[0,10](!false))");
  // cout << test->as_string() << "\n";
  // test = parse("((false&false|true)R[0,10](!false))");
  // cout << test->as_string() << "\n";
  // test = parse("~(true)&true");
  // cout << test->as_string() << "\n";
  // test = parse("G[0,4](true)&true");
  // cout << test->as_string() << "\n";
  //
  // test = parse("((false&false&true^G[0,4]true&~true)R[0,10](!false))");
  // cout << test->as_string() << "\n";
  //   test = parse("((false&false&true^G[0,4](true)&~true)R[0,10](!false))");
  //   cout << test->as_string() << "\n";
  //   test = parse("((false&false&true^(G[0,4](true))&~true)R[0,10](!false))");
  //   cout << test->as_string() << "\n";
  // // ((false&false&true^G[0,4](true)&~true)R[0,10](!false))
  //   test = parse("~(true)&true");
  // cout << test->as_string() << "\n";
  //   test = parse("G[0,4](true)&true");
  // cout << test->as_string() << "\n";
  //   test = parse("G[0,1](p&true");
  // cout << test->as_string() << "\n";
  test = parse("(p0&~p1)R[0,3](p2)");
  cout << test->as_string() << "\n";
  cout << test->evaluate(
              {"001", "001", "101", "000", "000", "101", "100", "110"})
       << "\n";

  return 0;
}
