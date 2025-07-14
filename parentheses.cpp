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

std::vector<std::string> balanced_parentheses_recursive(int parentheses_count) {
  return balanced_parentheses_internal(
    parentheses_count, parentheses_count, "");
}

std::vector<std::string> balanced_parentheses_iterative_recursive(
  int parentheses_count) {
  enum class return_address_e { before, recursive_1, recursive_2 };
  struct frame_t {
    return_address_e return_address;
    int opening;
    int closing;
    std::string current;
    std::vector<std::string> results;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{
      .return_address = return_address_e::before,
      .closing = parentheses_count,
      .opening = parentheses_count,
      .current = ""});
  std::vector<std::string> return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      if (top.opening == 0 && top.closing == 0) {
        return_value = std::vector<std::string>(1, top.current);
        call_stack.pop();
        continue;
      }
      if (top.opening > 0) {
        top.return_address = return_address_e::recursive_1;
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .opening = top.opening - 1,
            .closing = top.closing,
            .current = top.current + "("});
        continue;
      }
      if (top.closing > top.opening) {
        top.return_address = return_address_e::recursive_2;
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .opening = top.opening,
            .closing = top.closing - 1,
            .current = top.current + ")"});
        continue;
      }
      return_value = top.results;
      call_stack.pop();
    } else if (top.return_address == return_address_e::recursive_1) {
      top.results.insert(
        top.results.end(), return_value.begin(), return_value.end());
      if (top.closing > top.opening) {
        top.return_address = return_address_e::recursive_2;
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .opening = top.opening,
            .closing = top.closing - 1,
            .current = top.current + ")"});
        continue;
      }
      return_value = top.results;
      call_stack.pop();
    } else if (top.return_address == return_address_e::recursive_2) {
      top.results.insert(
        top.results.end(), return_value.begin(), return_value.end());
      return_value = top.results;
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  std::cout << "balanced parens recursive:\n";
  const auto balanced_parens_recursive = balanced_parentheses_recursive(3);
  for (const auto balanced_paren :
       balanced_parens_recursive
         | std::views::take(balanced_parens_recursive.size() - 1)) {
    std::cout << balanced_paren << ", ";
  }
  std::cout << balanced_parens_recursive.back() << '\n';
  std::cout << '\n';
  std::cout << "balanced parens iterative:\n";
  const auto balanced_parens_iterative =
    balanced_parentheses_iterative_recursive(3);
  for (const auto balanced_paren :
       balanced_parens_iterative
         | std::views::take(balanced_parens_iterative.size() - 1)) {
    std::cout << balanced_paren << ", ";
  }
  std::cout << balanced_parens_iterative.back() << '\n';
  return 0;
}
