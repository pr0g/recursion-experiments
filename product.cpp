#include <array>
#include <format>
#include <iostream>
#include <optional>
#include <span>

int product_recursive(std::span<const int> numbers) {
  if (numbers.empty()) {
    return 0;
  }
  if (numbers.size() == 1) {
    return numbers[0];
  }
  return numbers[0] * product_recursive(numbers.subspan(1, numbers.size() - 1));
}

int product_iterative(std::span<const int> numbers) {
  int result = 1;
  for (const auto number : numbers) {
    result *= number;
  }
  return result;
}

int product_iterative_recursive(std::span<const int> numbers) {
  enum class return_address_e { recursive };
  struct frame_t {
    std::optional<return_address_e> return_address;
    std::span<const int> numbers;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.numbers = numbers});

  int return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.numbers.empty()) {
        return_value = 0;
        call_stack.pop();
        continue;
      }
      if (top.numbers.size() == 1) {
        return_value = top.numbers[0];
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive;
      call_stack.push(
        frame_t{.numbers = top.numbers.subspan(1, top.numbers.size() - 1)});
    } else if (top.return_address == return_address_e::recursive) {
      return_value = top.numbers[0] * return_value;
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  {
    const auto numbers = std::array<int, 3>{2, 3, 4};
    std::cout << std::format(
      "product_recursive of {{2, 3, 4}} is {}\n",
      product_recursive(std::span(numbers)));
  }
  {
    const auto numbers = std::array<int, 0>{};
    std::cout << std::format(
      "product_recursive of {{}} is {}\n",
      product_recursive(std::span(numbers)));
  }
  {
    const auto numbers = std::array<int, 1>{1};
    std::cout << std::format(
      "product_recursive of {{1}} is {}\n",
      product_recursive(std::span(numbers)));
  }
  {
    const auto numbers = std::array<int, 4>{2, 2, 2, 2};
    std::cout << std::format(
      "product_recursive of {{2, 2, 2, 2}} is {}\n",
      product_recursive(std::span(numbers)));
  }
  std::cout << '\n';
  {
    const auto numbers = std::array<int, 3>{2, 3, 4};
    std::cout << std::format(
      "product_iterative of {{2, 3, 4}} is {}\n",
      product_iterative(std::span(numbers)));
  }
  {
    const auto numbers = std::array<int, 0>{};
    std::cout << std::format(
      "product_iterative of {{}} is {}\n",
      product_iterative(std::span(numbers)));
  }
  {
    const auto numbers = std::array<int, 1>{1};
    std::cout << std::format(
      "product_iterative of {{1}} is {}\n",
      product_iterative(std::span(numbers)));
  }
  {
    const auto numbers = std::array<int, 4>{2, 2, 2, 2};
    std::cout << std::format(
      "product_iterative of {{2, 2, 2, 2}} is {}\n",
      product_iterative(std::span(numbers)));
  }
  std::cout << '\n';
  {
    const auto numbers = std::array<int, 3>{2, 3, 4};
    std::cout << std::format(
      "product_iterative_recursive of {{2, 3, 4}} is {}\n",
      product_iterative_recursive(std::span(numbers)));
  }
  {
    const auto numbers = std::array<int, 0>{};
    std::cout << std::format(
      "product_iterative_recursive of {{}} is {}\n",
      product_iterative_recursive(std::span(numbers)));
  }
  {
    const auto numbers = std::array<int, 1>{1};
    std::cout << std::format(
      "product_iterative_recursive of {{1}} is {}\n",
      product_iterative_recursive(std::span(numbers)));
  }
  {
    const auto numbers = std::array<int, 4>{2, 2, 2, 2};
    std::cout << std::format(
      "product_iterative_recursive of {{2, 2, 2, 2}} is {}\n",
      product_iterative_recursive(std::span(numbers)));
  }

  return 0;
}
