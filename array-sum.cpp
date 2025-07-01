#include <format>
#include <iostream>

// head (element), tail (rest of array)

int sum_array(std::vector<int> tail) {
  if (tail.empty()) {
    return 0;
  }
  const int head = tail[0];
  tail = std::vector(tail.begin() + 1, tail.end());
  return head + sum_array(tail);
}

// use offset instead of making a copy
int sum_array_2(const std::vector<int>& tail) {
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
  std::cout << std::format("array total {}\n", sum_array({1, 2, 3, 4, 5}));
  std::cout << std::format("array total {}\n", sum_array_2({1, 2, 3, 4, 5, 6}));
  return 0;
}
