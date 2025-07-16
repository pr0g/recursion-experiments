#include <format>
#include <iostream>
#include <numeric>
#include <ranges>
#include <stack>

int factorial_iterative_1(int n) {
  int fact = 1;
  for (int index = n; index > 0; index--) {
    fact *= n;
    n--;
  }
  return fact;
}

int factorial_iterative_2(int n) {
  int fact = 1;
  for (; n > 0; n--) {
    fact *= n;
  }
  return fact;
}

int factorial_iterative_3(int n) {
  const auto r = std::views::iota(1, n + 1);
  return std::accumulate(
    r.begin(), r.end(), 1, [](int acc, int n) { return acc * n; });
}

// 1. What is the base case?
// 2. What argument is passed to the recursive function call?
// 3. How does the argument become closer to the base case?

// half of the call happens before the recursive call, half takes place
// after the recursive call returns

// first half is factorial_recursive(number - 1)
// second half is <number> * <return value> (e.g. 5 * 24)

int factorial_recursive(int n) {
  if (n <= 1) {
    return 1;
  }
  return n * factorial_recursive(n - 1);
}

int factorial_recursive_iterative(int n) {
  enum class return_address_e { before, recursive };
  struct frame_t {
    return_address_e return_address;
    int n;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.return_address = return_address_e::before, .n = n});
  int return_value; // uninitialised
  while (call_stack.size() > 0) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      if (top.n <= 1) {
        return_value = 1;
        call_stack.pop();
      } else {
        top.return_address = return_address_e::recursive;
        call_stack.push(
          frame_t{.return_address = return_address_e::before, .n = top.n - 1});
      }
    } else if (top.return_address == return_address_e::recursive) {
      return_value = top.n * return_value;
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  const int count = 10;
  std::cout << "factorial iterative 1:\n";
  for (int i = 0; i <= count; i++) {
    std::cout << std::format("{}! is {}\n", i, factorial_iterative_1(i));
  }
  std::cout << '\n';
  std::cout << "factorial iterative 2:\n";
  for (int i = 0; i <= count; i++) {
    std::cout << std::format("{}! is {}\n", i, factorial_iterative_2(i));
  }
  std::cout << '\n';
  std::cout << "factorial iterative 3:\n";
  for (int i = 0; i <= count; i++) {
    std::cout << std::format("{}! is {}\n", i, factorial_iterative_3(i));
  }
  std::cout << '\n';
  std::cout << "factorial recursive:\n";
  for (int i = 0; i <= count; i++) {
    std::cout << std::format("{}! is {}\n", i, factorial_recursive(i));
  }
  std::cout << '\n';
  std::cout << "factorial iterative recursive:\n";
  for (int i = 0; i <= count; i++) {
    std::cout << std::format(
      "{}! is {}\n", i, factorial_recursive_iterative(i));
  }
  return 0;
}
