#include <chrono>
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

int64_t karabutsa_recursive(const int64_t x, const int64_t y) {
  auto x_str = std::to_string(x);
  auto y_str = std::to_string(y);

  if (x_str.size() == 1 && y_str.size() == 1) {
    // std::cout << std::format(
    //   "Lookup {} * {} = {}\n", x_str, y_str,
    //   g_mult_table[std::stol(y_str) * 10 + std::stol(x_str)]);
    return g_mult_table[std::stol(y_str) * 10 + std::stol(x_str)];
  }

  // std::cout << std::format("Multiplying {} * {}\n", x_str, y_str);

  if (x_str.size() < y_str.size()) {
    x_str = pad_zeros(x_str, y_str.size() - x_str.size(), side_e::left);
  } else if (y_str.size() < x_str.size()) {
    y_str = pad_zeros(y_str, x_str.size() - y_str.size(), side_e::left);
  }

  const int64_t half_digits = x_str.size() / 2;

  const auto a = std::stol(x_str.substr(0, half_digits));
  const auto b = std::stol(x_str.substr(half_digits));
  const auto c = std::stol(y_str.substr(0, half_digits));
  const auto d = std::stol(y_str.substr(half_digits));

  const auto step1 = karabutsa_recursive(a, c);
  const auto step2 = karabutsa_recursive(b, d);
  const auto step3 = karabutsa_recursive(a + b, c + d);
  const auto step4 = step3 - step2 - step1;

  const auto step1_padding =
    (x_str.size() - half_digits) + (x_str.size() - half_digits);
  const auto step1_padded =
    std::stol(pad_zeros(std::to_string(step1), step1_padding, side_e::right));
  const auto step4_padding = x_str.size() - half_digits;
  const auto step4_padded =
    std::stol(pad_zeros(std::to_string(step4), step4_padding, side_e::right));

  const auto solved = step1_padded + step2 + step4_padded;
  // std::cout << std::format("Solved {} * {} = {}\n", x_str, y_str, solved);

  return solved;
}

int karabutsa_iterative_recursive(const int x, const int y) {
  enum class return_address_e { before, recursive1, recursive2, recursive3 };
  struct frame_t {
    return_address_e return_address;
    int x;
    int y;
    std::string x_str;
    std::string y_str;
    int half_digits;
    int a;
    int b;
    int c;
    int d;
    int step1;
    int step2;
    int step3;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{.return_address = return_address_e::before, .x = x, .y = y});

  int return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      top.x_str = std::to_string(top.x);
      top.y_str = std::to_string(top.y);
      if (top.x_str.size() == 1 && top.y_str.size() == 1) {
        std::cout << std::format(
          "Lookup {} * {} = {}\n", top.x_str, top.y_str,
          g_mult_table[std::stoi(top.y_str) * 10 + std::stoi(top.x_str)]);
        return_value =
          g_mult_table[std::stoi(top.y_str) * 10 + std::stoi(top.x_str)];
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive1;
      std::cout << std::format("Multiplying {} * {}\n", top.x_str, top.y_str);
      if (top.x_str.size() < top.y_str.size()) {
        top.x_str = pad_zeros(
          top.x_str, top.y_str.size() - top.x_str.size(), side_e::left);
      } else if (top.y_str.size() < top.x_str.size()) {
        top.y_str = pad_zeros(
          top.y_str, top.x_str.size() - top.y_str.size(), side_e::left);
      }
      top.half_digits = top.x_str.size() / 2;
      top.a = std::stoi(top.x_str.substr(0, top.half_digits));
      top.b = std::stoi(top.x_str.substr(top.half_digits));
      top.c = std::stoi(top.y_str.substr(0, top.half_digits));
      top.d = std::stoi(top.y_str.substr(top.half_digits));
      call_stack.push(
        frame_t{
          .return_address = return_address_e::before, .x = top.a, .y = top.c});
    } else if (top.return_address == return_address_e::recursive1) {
      top.return_address = return_address_e::recursive2;
      top.step1 = return_value;
      call_stack.push(
        frame_t{
          .return_address = return_address_e::before, .x = top.b, .y = top.d});
    } else if (top.return_address == return_address_e::recursive2) {
      top.return_address = return_address_e::recursive3;
      top.step2 = return_value;
      call_stack.push(
        frame_t{
          .return_address = return_address_e::before,
          .x = top.a + top.b,
          .y = top.c + top.d});
    } else if (top.return_address == return_address_e::recursive3) {
      top.step3 = return_value;
      const auto step4 = top.step3 - top.step2 - top.step1;

      const auto step1_padding = (top.x_str.size() - top.half_digits)
                               + (top.x_str.size() - top.half_digits);
      const auto step1_padded = std::stoi(
        pad_zeros(std::to_string(top.step1), step1_padding, side_e::right));
      const auto step4_padding = top.x_str.size() - top.half_digits;
      const auto step4_padded = std::stoi(
        pad_zeros(std::to_string(step4), step4_padding, side_e::right));

      const auto solved = step1_padded + top.step2 + step4_padded;
      std::cout << std::format(
        "Solved {} * {} = {}\n", top.x_str, top.y_str, solved);

      return_value = solved;
      call_stack.pop();
    }
  }
  return return_value;
}

const std::array<int64_t, 1000000> g_mult_table_999 = []() {
  std::array<int64_t, 1000000> mult_table;
  for (int64_t i = 0; i < 1000; i++) {
    for (int64_t j = 0; j < 1000; j++) {
      mult_table[j + i * 1000] = i * j;
    }
  }
  return mult_table;
}();

int64_t karabutsa_recursive_999(const int64_t x, const int64_t y) {
  auto x_str = std::to_string(x);
  auto y_str = std::to_string(y);

  if (x_str.size() <= 3 && y_str.size() <= 3) {
    // std::cout << std::format(
    //   "Lookup {} * {} = {}\n", x_str, y_str,
    //   g_mult_table_999[std::stol(y_str) * 1000 + std::stol(x_str)]);
    return g_mult_table_999[std::stol(y_str) * 1000 + std::stol(x_str)];
  }

  // std::cout << std::format("Multiplying {} * {}\n", x_str, y_str);

  if (x_str.size() < y_str.size()) {
    x_str = pad_zeros(x_str, y_str.size() - x_str.size(), side_e::left);
  } else if (y_str.size() < x_str.size()) {
    y_str = pad_zeros(y_str, x_str.size() - y_str.size(), side_e::left);
  }

  const int64_t half_digits = x_str.size() / 2;

  const auto a = std::stol(x_str.substr(0, half_digits));
  const auto b = std::stol(x_str.substr(half_digits));
  const auto c = std::stol(y_str.substr(0, half_digits));
  const auto d = std::stol(y_str.substr(half_digits));

  const auto step1 = karabutsa_recursive(a, c);
  const auto step2 = karabutsa_recursive(b, d);
  const auto step3 = karabutsa_recursive(a + b, c + d);
  const auto step4 = step3 - step2 - step1;

  const auto step1_padding =
    (x_str.size() - half_digits) + (x_str.size() - half_digits);
  const auto step1_padded =
    std::stol(pad_zeros(std::to_string(step1), step1_padding, side_e::right));
  const auto step4_padding = x_str.size() - half_digits;
  const auto step4_padded =
    std::stol(pad_zeros(std::to_string(step4), step4_padding, side_e::right));

  const auto solved = step1_padded + step2 + step4_padded;
  // std::cout << std::format("Solved {} * {} = {}\n", x_str, y_str, solved);

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

  {
    const auto lhs = 1357;
    const auto rhs = 2468;
    const auto recursive_result = karabutsa_recursive(lhs, rhs);
    std::cout << '\n';
    std::cout << std::format(
      "recursive karabutsa:\n{} * {} = {}\n{} * {} = {}\n", lhs, rhs,
      recursive_result, lhs, rhs, lhs * rhs);

    std::cout << '\n';
    const auto iterative_recursive_result =
      karabutsa_iterative_recursive(lhs, rhs);
    std::cout << '\n';
    std::cout << std::format(
      "iterative recursive karabutsa:\n{} * {} = {}\n{} * {} = {}\n", lhs, rhs,
      iterative_recursive_result, lhs, rhs, lhs * rhs);
  }

  using fp_seconds = std::chrono::duration<float, std::chrono::seconds::period>;

  {
    const auto lhs = int64_t{12345678};
    const auto rhs = int64_t{87654321};
    {
      const auto before = std::chrono::steady_clock::now();
      int64_t recursive_result_999;
      for (int i = 0; i < 10000; i++) {
        recursive_result_999 = karabutsa_recursive_999(lhs, rhs);
      }
      const auto after = std::chrono::steady_clock::now();
      const auto duration = fp_seconds(after - before);
      std::cout << '\n';
      std::cout << std::format(
        "recursive karabutsa 999:\n{} * {} = {}\n{} * {} = {}\n", lhs, rhs,
        recursive_result_999, lhs, rhs, lhs * rhs);
      std::cout << std::format("duration: {}\n", duration);
    }

    {
      const auto before = std::chrono::steady_clock::now();
      int64_t recursive_result;
      for (int i = 0; i < 10000; i++) {
        recursive_result = karabutsa_recursive(lhs, rhs);
      }
      const auto after = std::chrono::steady_clock::now();
      const auto duration = fp_seconds(after - before);
      std::cout << '\n';
      std::cout << std::format(
        "recursive karabutsa:\n{} * {} = {}\n{} * {} = {}\n", lhs, rhs,
        recursive_result, lhs, rhs, lhs * rhs);
      std::cout << std::format("duration: {}\n", duration);
    }
  }

  return 0;
}
