#include <format>
#include <iostream>

int sum_series_iterative(int n) {
  int result = 0;
  for (int i = 1; i <= n; i++) {
    result += i;
  }
  return result;
}

int sum_series_recursive(int n) {
  if (n == 1) {
    return 1;
  }
  return n + sum_series_recursive(n - 1);
}

int power_series_iterative(int n) {
  int power = 2;
  int result = 0;
  for (int i = 0; i < n; i++) {
    result += power;
    power = power * 2;
  }
  return result;
}

int power_series_recursive(int n, int power = 2) {
  if (n == 1) {
    return 2;
  }
  return power + power_series_recursive(n - 1, power) * 2;
}

int main(int argc, char** argv) {
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "sum of numbers up to {} is {}\n", i + 1, sum_series_iterative(i + 1));
  }
  std::cout << "---\n";
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "sum of numbers up to {} is {}\n", i + 1, sum_series_recursive(i + 1));
  }
  std::cout << "---\n";
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "sum of power of 2 numbers up to {} is {}\n", i + 1,
      power_series_iterative(i + 1));
  }
  std::cout << "---\n";
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "sum of power of 2 numbers up to {} is {}\n", i + 1,
      power_series_recursive(i + 1));
  }
  return 0;
}
