#include <format>
#include <iostream>

std::ostream_iterator<char> g_out(std::cout);

int factorial_iterative(int number) {
  int fact = 1;
  for (int index = number; index > 0; index--) {
    fact *= number;
    number--;
  }
  return fact;
}

// 1. What is the base case?
// 2. What argument is passed to the recursive function call?
// 3. How does the argument become closer to the base case?

// half of the call happens before the recursive call, half takes place
// after the recursive call returns

// first half is factorial_recursive(number - 1)
// second half is <number> * <return value> (e.g. 5 * 24)

int factorial_recursive(int number) {
  if (number <= 1) {
    return 1;
  }
  return number * factorial_recursive(number - 1);
}

int main(int argc, char** argv) {
  std::format_to(g_out, "{}! is {}\n", 5, factorial_iterative(5));
  std::format_to(g_out, "{}! is {}\n", 5, factorial_recursive(5));
  return 0;
}
