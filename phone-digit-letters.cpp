#include <format>
#include <iostream>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>

std::string letters_for_digit(int digit) {
  if (digit == 2) {
    return "abc";
  }
  if (digit == 3) {
    return "def";
  }
  if (digit == 4) {
    return "ghi";
  }
  if (digit == 5) {
    return "jkl";
  }
  if (digit == 6) {
    return "mno";
  }
  if (digit == 7) {
    return "pqrs";
  }
  if (digit == 8) {
    return "tuv";
  }
  if (digit == 9) {
    return "wxyz";
  }
  return "";
}

std::vector<std::string> visit_combos_recursive(
  std::span<const std::string> tails, std::string acc = "") {
  if (tails.empty()) {
    if (acc.empty()) {
      return std::vector<std::string>();
    } else {
      return std::vector<std::string>(1, acc);
    }
  }

  std::vector<std::string> combos;
  auto head_l = tails | std::views::take(1);
  auto tail_l = tails | std::views::drop(1);

  for (int i = 0; i < head_l[0].size(); i++) {
    auto head = head_l[0].substr(i, 1);
    const auto new_combos = visit_combos_recursive(tail_l, acc + head);
    combos.insert(combos.end(), new_combos.begin(), new_combos.end());
  }

  return combos;
}

std::vector<std::string> visit_combos_iterative_recursive(
  std::span<const std::string> tails, std::string acc = "") {
  enum class return_address_e { recursive };
  struct frame_t {
    std::optional<return_address_e> return_address;
    // function parameters
    std::span<const std::string> tails;
    std::string acc;
    // local variables
    std::span<const std::string> head_l;
    std::span<const std::string> tail_l;
    std::vector<std::string> combos;
    int i;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.tails = tails, .acc = acc});
  std::vector<std::string> return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.tails.empty()) {
        if (top.acc.empty()) {
          return_value = std::vector<std::string>();
          call_stack.pop();
          continue;
        } else {
          return_value = std::vector<std::string>(1, top.acc);
          call_stack.pop();
          continue;
        }
      }
      top.return_address = return_address_e::recursive;
      top.head_l = top.tails | std::views::take(1);
      top.tail_l = top.tails | std::views::drop(1);
      top.i = 0;
      if (top.i < top.head_l[0].size()) {
        auto head = top.head_l[0].substr(top.i, 1);
        call_stack.push(frame_t{.tails = top.tail_l, .acc = top.acc + head});
      }
    } else if (top.return_address == return_address_e::recursive) {
      const auto& new_combos = return_value;
      top.combos.insert(top.combos.end(), new_combos.begin(), new_combos.end());
      top.i++;
      if (top.i < top.head_l[0].size()) {
        auto head = top.head_l[0].substr(top.i, 1);
        call_stack.push(frame_t{.tails = top.tail_l, .acc = top.acc + head});
      } else {
        return_value = top.combos;
        call_stack.pop();
      }
    }
  }
  return return_value;
}

std::vector<std::string> letter_combinations_recursive(
  const std::string& digits) {
  std::vector<std::string> letters;
  for (const auto ch : digits) {
    letters.push_back(letters_for_digit(ch - '0'));
  }
  return visit_combos_recursive(letters);
}

std::vector<std::string> letter_combinations_iterative_recursive(
  const std::string& digits) {
  std::vector<std::string> letters;
  for (const auto ch : digits) {
    letters.push_back(letters_for_digit(ch - '0'));
  }
  return visit_combos_iterative_recursive(letters);
}

int main(int argc, char** argv) {
  {
    std::cout << "letter combinations recursive:\n";
    const auto combinations = letter_combinations_recursive({"39"});
    for (const auto& combination : combinations) {
      std::cout << combination << '\n';
    }
  }
  std::cout << '\n';
  {
    std::cout << "letter combinations iterative recursive:\n";
    const auto combinations = letter_combinations_iterative_recursive({"39"});
    for (const auto& combination : combinations) {
      std::cout << combination << '\n';
    }
  }
  return 0;
}
