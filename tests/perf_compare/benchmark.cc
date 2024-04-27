#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <regex>
#include <sys/time.h>

#include "evaluate_mltl.h"
#include "parser.hh"

using namespace std;
using namespace libmltl;

string replace_bounds(const string &str, int n) {
  regex re("\\[0,\\d+\\]");
  string result = regex_replace(str, re, "[0," + std::to_string(n) + "]");
  return result;
}

int main(int argc, char *argv[]) {
  // default options
  const vector<int> trace_length_arr = {4, 8, 16, 32, 64, 128, 256, 512, 1024};
  const int num_traces = 2048;
  const int num_var = 4;
  const int seed = 0;

  mt19937 mt(seed);

  struct timeval start, end;
  double time_taken = 0;

  vector<vector<string>> traces;

  vector<string> formulas_str;
  ifstream file("MLTL_interpreter/formulas.txt");
  string line;

  if (file.is_open()) {
    while (std::getline(file, line)) {
      // trim whitespace
      line.erase(remove_if(line.begin(), line.end(),
                           [](unsigned char c) { return isspace(c); }),
                 line.end());
      formulas_str.push_back(line);
    }
    file.close();
  } else {
    std::cerr << "Unable to open file formulas.txt\n";
    return 1;
  }

  vector<shared_ptr<ASTNode>> formulas;
  gettimeofday(&start, NULL); // start timer
  for (const auto &f : formulas_str) {
    formulas.emplace_back(parse(f));
  }
  gettimeofday(&end, NULL); // stop timer
  time_taken = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec -
               start.tv_usec / 1e6; // in seconds
  cout << "[libmltl] formula parsing took: " << time_taken << "s\n";

  int timeout = 60;
  bool libmltl_eval_timeout = false;
  bool libmltl_parse_eval_timeout = false;
  bool mltl_eval_timeout = false;

  // BENCHMARK LOOP
  for (int trace_length : trace_length_arr) {
    traces.clear();
    for (size_t i = 0; i < num_traces; ++i) {
      vector<string> new_trace;
      for (int j = 0; j < trace_length; ++j) {
        new_trace.emplace_back(int_to_bin_str(mt(), num_var));
      }
      traces.emplace_back(new_trace);
    }

    for (string &f : formulas_str) {
      f = replace_bounds(f, trace_length / 2);
    }

    formulas.clear();
    for (const auto &f : formulas_str) {
      formulas.emplace_back(parse(f));
    }

    cout << "Running benchmarks for trace length " << trace_length << "\n";

    if (!libmltl_eval_timeout) {
      gettimeofday(&start, NULL); // start timer
      for (size_t i = 0; i < formulas.size(); ++i) {
        for (size_t j = 0; j < num_traces; ++j) {
          formulas[i]->evaluate(traces[j]);
        }
      }
      gettimeofday(&end, NULL); // stop timer
      time_taken = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec -
                   start.tv_usec / 1e6; // in seconds
      cout << "  [libmltl] formula evaluation took   : " << time_taken << "s\n";
      libmltl_eval_timeout = (end.tv_sec - start.tv_sec > timeout);
    }

    if (!libmltl_parse_eval_timeout) {
      gettimeofday(&start, NULL); // start timer
      for (size_t i = 0; i < formulas.size(); ++i) {
        for (size_t j = 0; j < num_traces; ++j) {
          parse(formulas_str[i])->evaluate(traces[j]);
        }
      }
      gettimeofday(&end, NULL); // stop timer
      time_taken = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec -
                   start.tv_usec / 1e6; // in seconds
      cout << "  [libmltl] formula (parse+)evaluation: " << time_taken << "s\n";
      libmltl_parse_eval_timeout = (end.tv_sec - start.tv_sec > timeout);
    }

    if (!mltl_eval_timeout) {
      gettimeofday(&start, NULL); // start timer
      for (size_t i = 0; i < formulas.size(); ++i) {
        for (size_t j = 0; j < num_traces; ++j) {
          evaluate_mltl(formulas_str[i], traces[j]);
        }
      }
      gettimeofday(&end, NULL); // stop timer
      time_taken = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec -
                   start.tv_usec / 1e6; // in seconds
      cout << "  [mltl evaluator] formula evaluation : " << time_taken << "s\n";
      mltl_eval_timeout = (end.tv_sec - start.tv_sec > timeout);
    }
  }

  return 0;
}
