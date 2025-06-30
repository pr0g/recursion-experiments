#include <format>
#include <iostream>

std::ostream_iterator<char> g_out(std::cout);

int fibonacci_iterative(int n) {
  if (n == 0) {
    return 0;
  }
  if (n == 1) {
    return 1;
  }
  int prev = 0;
  int curr = 1;
  for (int i = 1; i < n; i++) {
    int next = prev + curr;
    prev = curr;
    curr = next;
  }

  return curr;
}

// 1. What is the base case?
// 2. What argument is passed to the recursive function call?
// 3. How does the argument become closer to the base case?

int fibonacci_recursive(int n) {
  if (n == 0) {
    return 0;
  }
  if (n == 1) {
    return 1;
  }
  return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

int main(int argc, char** argv) {
  for (int i = 0; i < 20; i++) {
    std::format_to(g_out, "{} fib it is {}\n", i, fibonacci_iterative(i));
  }

  for (int i = 0; i < 20; i++) {
    std::format_to(g_out, "{} fib rec is {}\n", i, fibonacci_recursive(i));
  }
  return 0;
}
