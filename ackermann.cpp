#include <format>
#include <iostream>

int ackermann_recursive(int m, int n, int indentation = 0) {
  std::string indent;
  std::generate_n(std::back_inserter(indent), indentation, [] { return ' '; });
  std::cout << std::format("{} ackermann({}, {})\n", indent, m, n);
  if (m == 0) {
    return n + 1;
  } else if (m > 0 && n == 0) {
    return ackermann_recursive(m - 1, 1, indentation + 1);
  } else if (m > 0 && n > 0) {
    return ackermann_recursive(
      m - 1, ackermann_recursive(m, n - 1, indentation + 1), indentation + 1);
  }
  return 0;
}

int main(int argc, char** argv) {
  const int result1 = ackermann_recursive(1, 1);
  std::cout << std::format("\nAckermann (1, 1) - {}\n", result1);

  std::cout << "\n";

  const int result2 = ackermann_recursive(2, 3);
  std::cout << std::format("\nAckermann (2, 3) - {}\n", result2);

  std::cout << "\n";

  const int result3 = ackermann_recursive(3, 3);
  std::cout << std::format("\nAckermann (3, 3) - {}\n", result3);
  return 0;
}
