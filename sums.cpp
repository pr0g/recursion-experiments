#include <format>
#include <iostream>
#include <stack>

int sum_series_iterative(int n) {
  int result = 0;
  for (int i = 1; i <= n; i++) {
    result += i;
  }
  return result;
}

int sum_series_recursive(int n) {
  if (n == 1) {
    return 1;
  }
  return n + sum_series_recursive(n - 1);
}

int sum_series_iterative_recursive(int n) {
  enum class return_address_e { before, recursive };
  struct frame_t {
    return_address_e return_address;
    int n;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.return_address = return_address_e::before, .n = n});
  int return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      if (top.n == 1) {
        return_value = 1;
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive;
      call_stack.push(
        frame_t{.return_address = return_address_e::before, .n = top.n - 1});
    } else if (top.return_address == return_address_e::recursive) {
      return_value = top.n + return_value;
      call_stack.pop();
    }
  }
  return return_value;
}

int power_series_iterative(int n) {
  int power = 2;
  int result = 0;
  for (int i = 0; i < n; i++) {
    result += power;
    power = power * 2;
  }
  return result;
}

int power_series_recursive(int n, int power = 2) {
  if (n == 1) {
    return 2;
  }
  return power + power_series_recursive(n - 1, power) * 2;
}

int main(int argc, char** argv) {
  std::cout << std::format("sum series iterative:\n");
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "sum of numbers up to {} is {}\n", i + 1, sum_series_iterative(i + 1));
  }
  std::cout << '\n';
  std::cout << std::format("sum series recursive:\n");
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "sum of numbers up to {} is {}\n", i + 1, sum_series_recursive(i + 1));
  }
  std::cout << '\n';
  std::cout << std::format("power series iterative:\n");
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "sum of power of 2 numbers up to {} is {}\n", i + 1,
      power_series_iterative(i + 1));
  }
  std::cout << '\n';
  std::cout << std::format("power series recursive:\n");
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "sum of power of 2 numbers up to {} is {}\n", i + 1,
      power_series_recursive(i + 1));
  }
  std::cout << '\n';
  std::cout << std::format("sum series iterative recursive:\n");
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "sum of power of 2 numbers up to {} is {}\n", i + 1,
      sum_series_iterative_recursive(i + 1));
  }
  return 0;
}
