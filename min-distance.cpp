#include <format>
#include <iostream>
#include <optional>
#include <string_view>

int min_distance_recursive(
  const std::string_view lhs, const std::string_view rhs) {
  // std::cout << "lhs: " << lhs << " " << "rhs: " << rhs << '\n';
  if (lhs.empty() || rhs.empty()) {
    return lhs.size() + rhs.size();
  }
  if (lhs.back() == rhs.back()) {
    return min_distance_recursive(
      lhs.substr(0, lhs.size() - 1), rhs.substr(0, rhs.size() - 1));
  } else {
    return 1
         + std::min(
             min_distance_recursive(
               lhs.substr(0, lhs.size() - 1), rhs.substr(0, rhs.size() - 1)),
             std::min(
               min_distance_recursive(lhs, rhs.substr(0, rhs.size() - 1)),
               min_distance_recursive(lhs.substr(0, lhs.size() - 1), rhs)));
  }
}

int min_distance_recursive_2(
  const std::string_view lhs, const std::string_view rhs) {
  if (lhs.empty() || rhs.empty()) {
    return lhs.size() + rhs.size();
  }
  if (lhs.back() == rhs.back()) {
    return min_distance_recursive(
      lhs.substr(0, lhs.size() - 1), rhs.substr(0, rhs.size() - 1));
  } else {
    int a = min_distance_recursive(
      lhs.substr(0, lhs.size() - 1), rhs.substr(0, rhs.size() - 1));
    int b = min_distance_recursive(lhs, rhs.substr(0, rhs.size() - 1));
    int c = min_distance_recursive(lhs.substr(0, lhs.size() - 1), rhs);
    return 1 + std::min(a, std::min(b, c));
  }
}

int min_distance_iterative_recursive(
  const std::string_view lhs, const std::string_view rhs) {
  enum class return_address_e {
    before,
    recursive_1,
    recursive_2,
    recursive_3,
    recursive_4
  };
  struct frame_t {
    std::optional<return_address_e> return_address;
    std::string_view lhs;
    std::string_view rhs;
    int a;
    int b;
    int c;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.lhs = lhs, .rhs = rhs});

  int return_value = 0;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      // std::cout << "lhs: " << top.lhs << " " << "rhs: " << top.rhs << '\n';
      if (top.lhs.empty() || top.rhs.empty()) {
        return_value = top.lhs.size() + top.rhs.size();
        call_stack.pop();
        continue;
      }
      if (top.lhs.back() == top.rhs.back()) {
        top.return_address = return_address_e::recursive_1;
        call_stack.push(
          frame_t{

            .lhs = top.lhs.substr(0, top.lhs.size() - 1),
            .rhs = top.rhs.substr(0, top.rhs.size() - 1)});
      } else {
        top.return_address = return_address_e::recursive_2;
        call_stack.push(
          frame_t{

            .lhs = top.lhs.substr(0, top.lhs.size() - 1),
            .rhs = top.rhs.substr(0, top.rhs.size() - 1)});
      }
    } else if (top.return_address == return_address_e::recursive_1) {
      call_stack.pop();
    } else if (top.return_address == return_address_e::recursive_2) {
      top.a = return_value;
      top.return_address = return_address_e::recursive_3;
      call_stack.push(
        frame_t{

          .lhs = top.lhs, .rhs = top.rhs.substr(0, top.rhs.size() - 1)});
    } else if (top.return_address == return_address_e::recursive_3) {
      top.b = return_value;
      top.return_address = return_address_e::recursive_4;
      call_stack.push(
        frame_t{

          .lhs = top.lhs.substr(0, top.lhs.size() - 1), .rhs = top.rhs});
    } else if (top.return_address == return_address_e::recursive_4) {
      top.c = return_value;
      return_value = 1 + std::min(top.a, std::min(top.b, top.c));
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  std::cout << "recursive: \n";
  std::cout << std::format(
    "Min distance: {} and {} is {}\n", "kitten", "sitting",
    min_distance_recursive("kitten", "sitting"));

  std::cout << '\n';

  std::cout << "iterative: \n";
  std::cout << std::format(
    "Min distance: {} and {} is {}\n", "kitten", "sitting",
    min_distance_iterative_recursive("kitten", "sitting"));
  return 0;
}
