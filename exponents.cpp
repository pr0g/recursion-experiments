#include <format>
#include <iostream>

int exponents_iterative(const int number, const int power) {
  int result = 1;
  for (int i = 0; i < power; i++) {
    result *= number;
  }
  return result;
}

// 1. What is the base case?
// power == 1 or power == 0
// 2. What argument is passed to the recursive function call?
// number and power decreased by some value
// 3. How does the argument become closer to the base case?
//

// 4 ^ 4

// before 4, 2
// before 2, 1,
// before 1

int exponents_recursive(const int number, const int power) {
  if (power == 1) {
    return number;
  }
  if (power == 0) {
    return 1;
  }
  const int return_value = exponents_recursive(number, power / 2);
  if (power % 2 == 0) {
    return return_value * return_value;
  } else {
    return return_value * return_value * number;
  }
}

int main(int argc, char** argv) {
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "exp of {} to power {} is {}\n", 5, i, exponents_iterative(5, i));
  }
  std::cout << "---\n";
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "exp of {} to power {} is {}\n", 5, i, exponents_recursive(5, i));
  }
  return 0;
}
