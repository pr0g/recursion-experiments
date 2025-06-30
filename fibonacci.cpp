#include <format>
#include <iostream>

std::ostream_iterator<char> g_out(std::cout);

// 0 1 2 3
// 0 1 1 2 3 5 8 13 21
int fibonacci_iterative(int number) {
  if (number == 0) {
    return 0;
  }
  if (number == 1) {
    return 1;
  }
  int prev = 0;
  int curr = 1;
  int next = 0;
  for (int i = 1; i < number; i++) {
    next = prev + curr;
    prev = curr;
    curr = next;
  }

  return next;
}

//   next  prev
//    0     1
//    1     0
//    2

int fibonacci_recursive(int number) {
  return 0;
}

int main(int argc, char** argv) {
  for (int i = 0; i < 20; i++) {
    std::format_to(g_out, "{} fib is {}\n", i, fibonacci_iterative(i));
  }
  return 0;
}
