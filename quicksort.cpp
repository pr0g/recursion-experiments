#include <format>
#include <iostream>
#include <ranges>
#include <span>

void print_items(std::span<const int> items) {
  for (const auto item : items | std::views::take(items.size() - 1)) {
    std::cout << std::format("{}, ", item);
  }
  std::cout << std::format("{}\n", items.back());
}

auto partition(std::span<int> items, int value) {
  int i = 0;
  for (int j = 0; j < items.size(); j++) {
    if (items[j] < value) {
      std::swap(items[j], items[i++]);
    }
  }
  return i;
}

void quicksort_recursive_1(
  std::span<int> items, std::optional<int> pivot = std::nullopt) {
  if (items.size() <= 1) {
    return;
  }
  if (pivot == std::nullopt) {
    pivot = items.back();
  }

  const auto midpoint = std::partition(
    items.begin(), items.end(), [pivot](int item) { return item < *pivot; });

  // put the pivot on the left side of right partition
  std::iter_swap(midpoint, std::prev(items.end()));

  const auto midpoint_index = std::distance(items.begin(), midpoint);
  quicksort_recursive_1(items.subspan(0, midpoint_index));
  quicksort_recursive_1(items.subspan(midpoint_index));
}

void quicksort_recursive_2(
  std::span<int> items, std::optional<int> pivot = std::nullopt) {
  if (items.size() <= 1) {
    return;
  }
  if (pivot == std::nullopt) {
    pivot = items.back();
  }

  const auto midpoint = partition(items, *pivot);

  // put the pivot on the left side of right partition
  std::swap(items[midpoint], items[items.size() - 1]);

  quicksort_recursive_2(items.subspan(0, midpoint));
  quicksort_recursive_2(items.subspan(midpoint));
}

void quicksort_iterative_recursive_2(std::span<int> items) {
  enum class return_address_e { recursive_1, recursive_2 };
  struct frame_t {
    std::optional<return_address_e> return_address;
    std::span<int> items;
    int midpoint;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{ .items = items});
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.items.size() <= 1) {
        call_stack.pop();
        continue;
      }
      top.midpoint = partition(top.items, top.items.back());
      std::swap(top.items[top.midpoint], top.items[top.items.size() - 1]);
      top.return_address = return_address_e::recursive_1;
      call_stack.push(
        frame_t{
          
          .items = top.items.subspan(0, top.midpoint)});
    } else if (top.return_address == return_address_e::recursive_1) {
      top.return_address = return_address_e::recursive_2;
      call_stack.push(
        frame_t{
          
          .items = top.items.subspan(top.midpoint)});
    } else if (top.return_address == return_address_e::recursive_2) {
      call_stack.pop();
    }
  }
}

int main(int argc, char** argv) {
  std::vector<int> items = {2, 6, 1, 5, 10, 15, 11, 3, 9, 20, 8};

  {
    auto items_recursive = items;
    quicksort_recursive_1(items_recursive);
    std::cout << "recursive quicksort: \n";
    print_items(items_recursive);
  }

  {
    auto items_recursive_2 = items;
    quicksort_recursive_2(items_recursive_2);
    std::cout << "recursive_2 quicksort: \n";
    print_items(items_recursive_2);
  }

  {
    auto items_iterative_recursive_2 = items;
    quicksort_iterative_recursive_2(items_iterative_recursive_2);
    std::cout << "iterative recursive_2 quicksort: \n";
    print_items(items_iterative_recursive_2);
  }

  {
    std::vector<int> items2 = {0, 7, 6, 3, 1, 2, 5, 4};
    auto midpoint = partition(items2, 1);
    std::cout << "partition midpoint: " << midpoint << '\n';
    print_items(items2);
  }

  {
    std::vector<int> items2 = {0, 7, 6, 3, 1, 2, 5, 4};
    auto midpoint =
      std::partition(items2.begin(), items2.end(), [](int v) { return v < 1; });
    std::cout << "std::partition midpoint: "
              << std::distance(items2.begin(), midpoint) << '\n';
    print_items(items2);
  }

  {
    auto items_custom = items;
    auto midpoint = partition(items_custom, 3);
    std::cout << "partition midpoint: " << midpoint << '\n';
    print_items(items_custom);
  }

  {
    auto items_std = items;
    auto midpoint = std::partition(
      items_std.begin(), items_std.end(), [](const int v) { return v < 3; });
    std::cout << "std::partition midpoint: "
              << std::distance(items_std.begin(), midpoint) << '\n';
    print_items(items_std);
  }

  return 0;
}
