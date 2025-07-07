#include <format>
#include <iostream>
#include <span>
#include <vector>

int array_sum_div_recursive(std::span<int> numbers) {
  if (numbers.empty()) {
    return 0;
  }
  if (numbers.size() == 1) {
    return numbers[0];
  }
  int half = numbers.size() / 2;
  int lhs = array_sum_div_recursive(numbers.subspan(0, half));
  int rhs = array_sum_div_recursive(numbers.subspan(half));
  return lhs + rhs;
}

int array_sum_div_iterative_recursive(std::span<int> numbers) {
  enum class return_address_e { before, recursive1, recursive2 };
  struct frame_t {
    return_address_e return_address;
    std::span<int> numbers;
    int half;
    int lhs;
    int rhs;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{.return_address = return_address_e::before, .numbers = numbers});
  int return_value = 0;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      if (top.numbers.empty()) {
        return_value = 0;
        call_stack.pop();
        continue;
      }
      if (top.numbers.size() == 1) {
        return_value = top.numbers[0];
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive1;
      top.half = top.numbers.size() / 2;
      call_stack.push(
        frame_t{
          .return_address = return_address_e::before,
          .numbers = top.numbers.subspan(0, top.half)});
    } else if (top.return_address == return_address_e::recursive1) {
      top.return_address = return_address_e::recursive2;
      top.lhs = return_value;
      call_stack.push(
        frame_t{
          .return_address = return_address_e::before,
          .numbers = top.numbers.subspan(top.half)});
    } else if (top.return_address == return_address_e::recursive2) {
      top.rhs = return_value;
      return_value = top.lhs + top.rhs;
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  std::vector<int> numbers = {1, 2, 3, 6, 10, 20, 50, 4, 9, 13, 25};
  const int total_recursive = array_sum_div_recursive(numbers);
  std::cout << std::format(
    "array_sum_div recursive total: {}\n", total_recursive);

  const int total_iterative_recursive =
    array_sum_div_iterative_recursive(numbers);
  std::cout << std::format(
    "array_sum_div iterative recursive total: {}\n", total_iterative_recursive);
  return 0;
}
