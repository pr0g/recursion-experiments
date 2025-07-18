#include <format>
#include <iostream>
#include <stack>

void count_down_and_up_recursive(const int number) {
  std::cout << std::format("{}\n", number);
  if (number == 0) {
    std::cout << std::format("reached base case\n");
    return;
  }
  count_down_and_up_recursive(number - 1);
  std::cout << std::format("{} returning\n", number);
}

void count_down_and_up_iterative_recursive(const int number) {
  enum class return_address_e { recursive };
  struct frame_t {
    std::optional<return_address_e> return_address;
    int number;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{ .number = number});
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      std::cout << std::format("{}\n", top.number);
      if (top.number == 0) {
        std::cout << std::format("reached base case\n");
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive;
      call_stack.push(
        frame_t{
          
          .number = top.number - 1});
    } else if (top.return_address == return_address_e::recursive) {
      std::cout << std::format("{} returning\n", top.number);
      call_stack.pop();
    }
  }
}

int main(int argc, char** argv) {
  std::cout << "count up and down recursive:\n";
  count_down_and_up_recursive(10);
  std::cout << '\n';
  std::cout << "count up and down iterative recursive:\n";
  count_down_and_up_iterative_recursive(10);
  return 0;
}
