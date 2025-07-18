#include <format>
#include <iostream>
#include <stack>
#include <optional>

// head (element), tail (rest of array)

int sum_array_recursive_1(const std::vector<int>& tail) {
  if (tail.empty()) {
    return 0;
  }
  return tail[0]
       + sum_array_recursive_1(std::vector(tail.begin() + 1, tail.end()));
}

// use offset instead of making a copy
int sum_array_recursive_2(const std::vector<int>& tail) {
  const std::function<int(const std::vector<int>&, int)> sum =
    [&sum](const std::vector<int>& tail, int offset) {
      if (offset == tail.size()) {
        return 0;
      }
      const int head = tail[offset++];
      return head + sum(tail, offset);
    };
  return sum(tail, 0);
}

// iterative recursive version
int sum_array_iterative_recursive(const std::vector<int>& tail) {
  enum class return_address_e { recursive };
  struct frame_t {
    std::optional<return_address_e> return_address;
    std::vector<int> tail;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{ .tail = tail});

  int return_value = 0;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.tail.empty()) {
        return_value = 0;
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive;
      call_stack.push(
        frame_t{
          
          .tail = std::vector(top.tail.begin() + 1, top.tail.end())});
    } else if (top.return_address == return_address_e::recursive) {
      return_value = top.tail[0] + return_value;
      call_stack.pop();
    }
  }
  return return_value;
}

// head 1, tail 2, 3, 4, 5
// head 2, tail 3, 4, 5
// head 3, tail 4, 5
// head 4, tail 5
// head 5, tail
// 5 + 0
// 4 + 5
// 3 + 9
// 2 + 12
// 1 + 14
// 15

int main(int argc, char** argv) {
  std::cout << "array sum recursive 1:\n";
  std::cout << std::format(
    "total: {}\n", sum_array_recursive_1({1, 2, 3, 4, 5, 6}));
  std::cout << "array sum recursive 2:\n";
  std::cout << std::format(
    "total: {}\n", sum_array_recursive_2({1, 2, 3, 4, 5, 6}));
  std::cout << "array sum iterative recursive:\n";
  std::cout << std::format(
    "total: {}\n", sum_array_iterative_recursive({1, 2, 3, 4, 5, 6}));
  return 0;
}
