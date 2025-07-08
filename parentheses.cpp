#include <format>
#include <iostream>
#include <ranges>
#include <string>

// parentheses

// 3
//
// ((()))
// ()()()
// (())()
// ()(())
// (()())

std::vector<std::string> balanced_parentheses_internal(
  int opening, int closing, std::string current) {
  if (opening == 0 && closing == 0) {
    return std::vector<std::string>(1, current);
  }
  std::vector<std::string> results;
  if (opening > 0) {
    auto r = balanced_parentheses_internal(opening - 1, closing, current + "(");
    results.insert(results.end(), r.begin(), r.end());
  }
  if (closing > opening) {
    auto r = balanced_parentheses_internal(opening, closing - 1, current + ")");
    results.insert(results.end(), r.begin(), r.end());
  }
  return results;
}

std::vector<std::string> balanced_parentheses(int parentheses_count) {
  return balanced_parentheses_internal(
    parentheses_count, parentheses_count, "");
}

int main(int argc, char** argv) {

  std::cout << "balanced parens recursive:\n";
  const auto balanced_parens = balanced_parentheses(3);
  for (const auto balanced_paren :
       balanced_parens | std::views::take(balanced_parens.size() - 1)) {
    std::cout << balanced_paren << ", ";
  }
  std::cout << balanced_parens.back() << '\n';

  return 0;
}
