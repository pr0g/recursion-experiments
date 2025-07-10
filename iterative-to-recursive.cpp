#include <format>
#include <iostream>

void say_hello_iterative(int n) {
  for (int i = 0; i < n; i++) {
    std::cout << std::format("hello {}\n", i + 1);
  }
}

void say_hello_recursive(int n, int i = 0) {
  if (i < n) {
    std::cout << std::format("hello {}\n", i + 1);
    say_hello_recursive(n, i + 1);
  }
}

int main(int argc, char** argv) {
  std::cout << "say hello iterative:\n";
  say_hello_iterative(5);
  std::cout << '\n';
  std::cout << "say hello recursive:\n";
  say_hello_recursive(5);
  return 0;
}
