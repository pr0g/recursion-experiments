#include <format>
#include <iostream>
#include <stack>

int64_t factorial_recursive_tail(int64_t n, int64_t acc = 1) {
  if (n <= 1) {
    return acc;
  }
  return factorial_recursive_tail(n - 1, acc * n);
}

std::string reverse_string_recursive_tail_1(
  std::string_view string, std::string acc = "") {
  if (string.empty()) {
    return acc;
  }
  return reverse_string_recursive_tail_1(
    string.substr(1), std::string(string.substr(0, 1)) + acc);
}

std::string reverse_string_recursive_tail_internal(
  std::string_view string, std::string& acc) {
  if (string.empty()) {
    return acc;
  }
  acc.insert(acc.begin(), string.at(0));
  return reverse_string_recursive_tail_internal(string.substr(1), acc);
}

std::string reverse_string_recursive_tail_2(std::string_view string) {
  std::string buffer;
  buffer.reserve(string.size());
  return reverse_string_recursive_tail_internal(string, buffer);
}

bool is_odd_recursive_tail(int n, bool inversion_acc = false) {
  if (n == 0) {
    return inversion_acc;
  }
  return is_odd_recursive_tail(n - 1, !inversion_acc);
}

int main(int argc, char** argv) {
  std::cout << "factorial recursive tail call:\n";
  for (int i = 0; i <= 20; i++) {
    std::cout << std::format("{} is {}\n", i, factorial_recursive_tail(i));
  }
  std::cout << '\n';
  // runs in Release (value too large to represent), segmentation fault in Debug
  // std::cout << std::format("{} is {}\n", 1000000, factorial_tail(1000000));
  std::string value = "hello world";
  std::cout << "reverse string recursive tail call 1:\n";
  std::cout << std::format(
    "\"{}\" reversed \"{}\"\n", value, reverse_string_recursive_tail_1(value));
  std::cout << '\n';
  std::cout << "reverse string recursive tail call 2:\n";
  std::cout << std::format(
    "\"{}\" reversed \"{}\"\n", value, reverse_string_recursive_tail_2(value));
  // runs in Release, segmentation fault in Debug
  std::string many = std::string(100000, 'a');
  // reverse_string_recursive_tail_2(many);
  std::cout << '\n';
  std::cout << "recursive is odd:\n";
  std::cout << std::format(
    "{} is {}\n", 42, is_odd_recursive_tail(42) ? "odd" : "even");
  std::cout << std::format(
    "{} is {}\n", 99, is_odd_recursive_tail(99) ? "odd" : "even");
  return 0;
}
