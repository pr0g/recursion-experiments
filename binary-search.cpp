#include <chrono>
#include <format>
#include <iostream>
#include <numeric>
#include <span>

std::optional<int> binary_search_recursive(
  int needle, std::span<const int> haystack,
  std::optional<int> left = std::nullopt,
  std::optional<int> right = std::nullopt) {
  if (left == std::nullopt) {
    left = 0;
  }
  if (right == std::nullopt) {
    right = haystack.size() - 1;
  }
  if (*left > *right) {
    return std::nullopt;
  }
  std::cout << "searching: [";
  for (int i = *left; i < *right; i++) {
    std::cout << haystack[i] << ", ";
  }
  std::cout << haystack[*right] << "]\n";
  const int mid = *left + (*right - *left) / 2;
  if (needle == haystack[mid]) {
    return mid;
  } else if (needle < haystack[mid]) {
    return binary_search_recursive(needle, haystack, left, mid - 1);
  } else /*if (needle > haystack[mid]) */ {
    return binary_search_recursive(needle, haystack, mid + 1, right);
  }
}

std::optional<int> binary_search_iterative(
  int needle, std::span<const int> haystack,
  std::optional<int> left = std::nullopt,
  std::optional<int> right = std::nullopt) {
  if (left == std::nullopt) {
    left = 0;
  }
  if (right == std::nullopt) {
    right = haystack.size() - 1;
  }
  while (*left <= *right) {
    // std::cout << "searching: [";
    // for (int i = *left; i < *right; i++) {
    //   std::cout << haystack[i] << ", ";
    // }
    // std::cout << haystack[*right] << "]\n";
    const int mid = *left + (*right - *left) / 2;
    if (needle == haystack[mid]) {
      return mid;
    } else if (needle < haystack[mid]) {
      right = mid - 1;
    } else if (needle > haystack[mid]) {
      left = mid + 1;
    }
  }
  return std::nullopt;
}

std::optional<int> binary_search_iterative_recursive(
  int needle, std::span<const int> haystack,
  std::optional<int> left = std::nullopt,
  std::optional<int> right = std::nullopt) {
  enum class return_address_e { before, recursive_1, recursive_2 };
  struct frame_t {
    return_address_e return_address;
    std::optional<int> left;
    std::optional<int> right;
  };
  if (left == std::nullopt) {
    left = 0;
  }
  if (right == std::nullopt) {
    right = haystack.size() - 1;
  }
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{
      .return_address = return_address_e::before,
      .left = left,
      .right = right});
  std::optional<int> return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      if (*top.left > *top.right) {
        return_value = std::nullopt;
        call_stack.pop();
        continue;
      }
      std::cout << "searching: [";
      for (int i = *top.left; i < *top.right; i++) {
        std::cout << haystack[i] << ", ";
      }
      std::cout << haystack[*top.right] << "]\n";
      const int mid = *top.left + (*top.right - *top.left) / 2;
      if (needle == haystack[mid]) {
        return_value = mid;
        call_stack.pop();
        continue;
      } else if (needle < haystack[mid]) {
        top.return_address = return_address_e::recursive_1;
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .left = top.left,
            .right = mid - 1});
      } else {
        top.return_address = return_address_e::recursive_2;
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .left = mid + 1,
            .right = top.right});
      }
    } else if (top.return_address == return_address_e::recursive_1) {
      call_stack.pop();
    } else if (top.return_address == return_address_e::recursive_2) {
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  const int needle = 13;
  {
    const auto found =
      binary_search_recursive(needle, {{1, 4, 8, 11, 13, 16, 19, 19}});
    std::cout << std::format(
      "position of {} in [1, 4, 8, 11, 13, 16, 19, 19] is {}\n", needle,
      found.value_or(-1));
  }
  std::cout << '\n';
  {
    const auto found =
      binary_search_iterative(needle, {{1, 4, 8, 11, 13, 16, 19, 19}});
    std::cout << std::format(
      "position of {} in [1, 4, 8, 11, 13, 16, 19, 19] is {}\n", needle,
      found.value_or(-1));
  }
  std::cout << '\n';
  {
    const auto found = binary_search_iterative_recursive(
      needle, {{1, 4, 8, 11, 13, 16, 19, 19}});
    std::cout << std::format(
      "position of {} in [1, 4, 8, 11, 13, 16, 19, 19] is {}\n", needle,
      found.value_or(-1));
  }

  std::cout << '\n';
  
  // already sorted...
  std::vector<int> values(10'000, 0);
  std::iota(values.begin(), values.end(), 1);

  using fp_seconds = std::chrono::duration<float, std::chrono::seconds::period>;

  {
    const auto before = std::chrono::steady_clock::now();
    int64_t acc = 0;
    for (int i = 0; i < 100'000; i++) {
      auto it = std::find(values.begin(), values.end(), 7452);
      acc += *it;
    }
    const auto after = std::chrono::steady_clock::now();
    const auto duration = fp_seconds(after - before);
    std::cout << std::format(
      "linear search duration: {}, acc: {}\n", duration, acc);
  }

  {
    const auto before = std::chrono::steady_clock::now();
    int64_t acc = 0;
    for (int i = 0; i < 100'000; i++) {
      auto index = binary_search_iterative(7452, values);
      acc += values[*index];
    }
    const auto after = std::chrono::steady_clock::now();
    const auto duration = fp_seconds(after - before);
    std::cout << std::format(
      "binary search duration: {}, acc: {}\n", duration, acc);
  }

  return 0;
}
