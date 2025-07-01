#include <format>
#include <iostream>

int exponents_iterative(const int number, const int power) {
  int result = 1;
  for (int i = 0; i < power; i++) {
    result *= number;
  }
  return result;
}

// 1. What is the base case?
//  power == 1 or power == 0
// 2. What argument is passed to the recursive function call?
//  number and power decreased by some value
// 3. How does the argument become closer to the base case?
//  halving the power

int exponents_recursive(const int number, const int power) {
  if (power == 1) {
    return number;
  }
  if (power == 0) {
    return 1;
  }
  const int return_value = exponents_recursive(number, power / 2);
  if (power % 2 == 0) {
    return return_value * return_value;
  } else {
    return return_value * return_value * number;
  }
}

int exponents_iterative_recursive(const int number, const int power) {
  if (power == 0) {
    return 1;
  }
  enum class return_address_e { before, recursive };
  struct frame_t {
    return_address_e return_address;
    int power;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{.return_address = return_address_e::before, .power = power});
  int return_value = 1;
  while (!call_stack.empty()) {
    const auto frame = call_stack.top();
    if (frame.return_address == return_address_e::before) {
      if (frame.power == 1) {
        return_value = number;
        call_stack.pop();
        continue;
      } else {
        call_stack.top().return_address = return_address_e::recursive;
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .power = frame.power / 2});
        continue;
      }
    } else if (frame.return_address == return_address_e::recursive) {
      if (frame.power % 2 == 0) {
        return_value = return_value * return_value;
      } else {
        return_value = return_value * return_value * number;
      }
      call_stack.pop();
      continue;
    }
  }
  return return_value;
}

// alternate version - not a direct translation of the recursive algorithm
int exponents_iterative_recursive_2(const int number, int power) {
  if (power == 0) {
    return 1;
  }
  enum class op_e { multiply, square };
  std::stack<op_e> op_stack;
  while (power > 1) {
    if (power % 2 == 0) {
      op_stack.push(op_e::square);
      power /= 2;
    } else {
      op_stack.push(op_e::multiply);
      power -= 1;
    }
  }

  int result = number;
  while (!op_stack.empty()) {
    const auto op = op_stack.top();
    if (op == op_e::multiply) {
      result *= number;
    } else if (op == op_e::square) {
      result *= result;
    }
    op_stack.pop();
  }

  return result;
}

int main(int argc, char** argv) {
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "exp of {} to power {} is {}\n", 5, i, exponents_iterative(5, i));
  }
  std::cout << "---\n";
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "exp of {} to power {} is {}\n", 5, i, exponents_recursive(5, i));
  }
  std::cout << "---\n";
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "exp of {} to power {} is {}\n", 5, i,
      exponents_iterative_recursive(5, i));
  }
  std::cout << "---\n";
  for (int i = 0; i < 5; i++) {
    std::cout << std::format(
      "exp of {} to power {} is {}\n", 5, i,
      exponents_iterative_recursive_2(5, i));
  }
  std::cout << "---\n";
  std::cout << std::format(
    "exp of {} to power {} is {}\n", 6, 5,
    exponents_iterative_recursive_2(6, 5));
  return 0;
}
