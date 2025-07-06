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

int main(int argc, char** argv) {
  std::vector<int> numbers = {1, 2, 3, 6, 10, 20, 50};
  const int total = array_sum_div_recursive(numbers);
  std::cout << std::format("array_sum_div recursive total: {}\n", total);
  return 0;
}
