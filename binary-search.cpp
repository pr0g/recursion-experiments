#include <format>
#include <iostream>
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
    std::cout << "searching: [";
    for (int i = *left; i < *right; i++) {
      std::cout << haystack[i] << ", ";
    }
    std::cout << haystack[*right] << "]\n";
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

  return 0;
}
