#include <format>
#include <iostream>
#include <ranges>
#include <span>
#include <vector>

void print_items(std::span<const int> items) {
  for (const auto item : items | std::views::take(items.size() - 1)) {
    std::cout << std::format("{}, ", item);
  }
  std::cout << std::format("{}\n", items.back());
}

std::vector<int> mergesort_recursive(std::span<const int> items) {
  if (items.size() <= 1) {
    return std::vector<int>{items.begin(), items.end()};
  }
  int half = items.size() / 2;
  const auto lhs = mergesort_recursive(items.subspan(0, half));
  const auto rhs = mergesort_recursive(items.subspan(half));
  std::vector<int> merged;
  int l = 0, r = 0;
  for (; l < lhs.size() && r < rhs.size();) {
    if (lhs[l] < rhs[r]) {
      merged.push_back(lhs[l++]);
    } else {
      merged.push_back(rhs[r++]);
    }
  }
  for (; l < lhs.size(); l++) {
    merged.push_back(lhs[l]);
  }
  for (; r < rhs.size(); r++) {
    merged.push_back(rhs[r]);
  }
  return merged;
}

int main(int argc, char** argv) {
  std::vector<int> items = {2, 6, 1, 5, 10, 15, 11, 3, 9, 20, 8};

  std::cout << "recursive mergesort: \n";
  const auto sorted = mergesort_recursive(items);
  print_items(sorted);

  return 0;
}
