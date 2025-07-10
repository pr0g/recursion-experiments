#include <format>
#include <iostream>
#include <stack>

int64_t factorial_tail(int64_t n, int64_t acc = 1) {
  if (n <= 1) {
    return acc;
  }
  return factorial_tail(n - 1, acc * n);
}

int main(int argc, char** argv) {
  std::cout << "factorial recursive tail call:\n";
  for (int i = 0; i <= 20; i++) {
    std::cout << std::format("{} is {}\n", i, factorial_tail(i));
  }
  // runs in Release (value too large to represent), segmentation fault in Debug
  // std::cout << std::format("{} is {}\n", 1000000, factorial_tail(1000000));
  return 0;
}
