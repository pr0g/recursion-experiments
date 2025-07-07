#include <format>
#include <iostream>
#include <span>
#include <vector>

// clang-format off
// 1. multiply a and c either from a multiplication lookup table, or from a recursive call to karatsuba
// 2. multiply b and d either from a multiplication lookup table, or from a recursive call to karatsuba
// 3. multiple a + c and b + d either from the multiplication lookup table or from a recursive call to karatsuba
// 4. calculate step 3 - step 2 - step 1
// 5. pad the step 1 and step 4 results with zeros, then add them to step 2

// base case - multiplying single digit numbers (which can be done with a precomputed lookup table)
// recursive case arguments - the a, b, c, and d values derived from x and y arguments
// how recursive arguments change - since a, b, c, and d are each half of the digits of x and y, the recursive call's
//                                  arguments get closer and closer to the single digit numbers the base case needs
// clang-format on

enum class side_e { left, right };

const std::array<int, 100> g_mult_table = []() {
  std::array<int, 100> mult_table;
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      mult_table[j + i * 10] = i * j;
    }
  }
  return mult_table;
}();

std::string pad_zeros(
  std::string_view number_string, int zero_count, side_e side) {
  const auto pad = std::string(zero_count, '0');
  if (side == side_e::left) {
    return pad + std::string(number_string);
  } else if (side == side_e::right) {
    return std::string(number_string) + pad;
  }
  return std::string();
}

int karabutsa_recursive(const int x, const int y) {
  auto x_str = std::to_string(x);
  auto y_str = std::to_string(y);

  if (x_str.size() == 1 && y_str.size() == 1) {
    std::cout << std::format(
      "Lookup {} * {} = {}\n", x_str, y_str,
      g_mult_table[std::stoi(y_str) * 10 + std::stoi(x_str)]);
    return g_mult_table[std::stoi(y_str) * 10 + std::stoi(x_str)];
  }

  std::cout << std::format("Multiplying {} * {}\n", x_str, y_str);

  if (x_str.size() < y_str.size()) {
    x_str = pad_zeros(x_str, y_str.size() - x_str.size(), side_e::left);
  } else if (y_str.size() < x_str.size()) {
    y_str = pad_zeros(y_str, x_str.size() - y_str.size(), side_e::left);
  }

  const int half_digits = x_str.size() / 2;

  const auto a = std::stoi(x_str.substr(0, half_digits));
  const auto b = std::stoi(x_str.substr(half_digits));
  const auto c = std::stoi(y_str.substr(0, half_digits));
  const auto d = std::stoi(y_str.substr(half_digits));

  const auto step1 = karabutsa_recursive(a, c);
  const auto step2 = karabutsa_recursive(b, d);
  const auto step3 = karabutsa_recursive(a + b, c + d);
  const auto step4 = step3 - step2 - step1;

  const auto step1_padding =
    (x_str.size() - half_digits) + (x_str.size() - half_digits);
  const auto step1_padded =
    std::stoi(pad_zeros(std::to_string(step1), step1_padding, side_e::right));
  const auto step4_padding = x_str.size() - half_digits;
  const auto step4_padded =
    std::stoi(pad_zeros(std::to_string(step4), step4_padding, side_e::right));

  const auto solved = step1_padded + step2 + step4_padded;
  std::cout << std::format("Solved {} * {} = {}\n", x_str, y_str, solved);

  return solved;
}

int main(int argc, char** argv) {
#if 0 // debug
  const auto number_pad_left = pad_zeros(std::to_string(100), 4, side_e::left);
  std::cout << std::format("{}\n", number_pad_left);
  const auto number_pad_right =
    pad_zeros(std::to_string(100), 4, side_e::right);
  std::cout << std::format("{}\n", number_pad_right);

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      std::cout << std::format(
        "product of {} and {} is {}\n", i, j, karabutsa(i, j));
    }
  }
#endif

  const auto lhs = 1357;
  const auto rhs = 2468;
  const auto recursive_result = karabutsa_recursive(lhs, rhs);
  std::cout << '\n';
  std::cout << std::format(
    "recursive karabutsa:\n{} * {} = {}\n{} * {} = {}\n", lhs, rhs,
    recursive_result, lhs, rhs, lhs * rhs);

  return 0;
}
