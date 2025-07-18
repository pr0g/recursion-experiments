#include <format>
#include <iostream>
#include <optional>
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

std::vector<int> mergesort_iterative_recursive(std::span<const int> items) {
  enum class return_address_e { recursive_1, recursive_2 };
  struct frame_t {
    std::optional<return_address_e> return_address;
    std::span<const int> items;
    std::vector<int> lhs;
    std::vector<int> rhs;
    int half;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.items = items});

  std::vector<int> return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.items.size() <= 1) {
        return_value = std::vector<int>{top.items[0]};
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive_1;
      top.half = top.items.size() / 2;
      call_stack.push(frame_t{.items = top.items.subspan(0, top.half)});
    } else if (top.return_address == return_address_e::recursive_1) {
      top.return_address = return_address_e::recursive_2;
      top.lhs = return_value;
      call_stack.push(frame_t{.items = top.items.subspan(top.half)});
    } else if (top.return_address == return_address_e::recursive_2) {
      top.rhs = return_value;
      std::vector<int> merged;
      int l = 0, r = 0;
      for (; l < top.lhs.size() && r < top.rhs.size();) {
        if (top.lhs[l] < top.rhs[r]) {
          merged.push_back(top.lhs[l++]);
        } else {
          merged.push_back(top.rhs[r++]);
        }
      }
      for (; l < top.lhs.size(); l++) {
        merged.push_back(top.lhs[l]);
      }
      for (; r < top.rhs.size(); r++) {
        merged.push_back(top.rhs[r]);
      }
      return_value = merged;
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  std::vector<int> items = {2, 6, 1, 5, 10, 15, 11, 3, 9, 20, 8};

  std::cout << "recursive mergesort: \n";
  const auto sorted_recursive = mergesort_recursive(items);
  print_items(sorted_recursive);

  std::cout << "iterative recursive mergesort: \n";
  const auto iterative_recursive_sorted = mergesort_iterative_recursive(items);
  print_items(iterative_recursive_sorted);

  return 0;
}
