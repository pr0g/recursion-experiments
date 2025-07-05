#include <format>
#include <iostream>
#include <ranges>
#include <span>

void quicksort_recursive(
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

  quicksort_recursive(items.subspan(0, std::distance(items.begin(), midpoint)));
  quicksort_recursive(items.subspan(std::distance(midpoint, items.end())));
}

int main(int argc, char** argv) {
  std::vector<int> items = {2, 6, 1, 5, 10, 15, 11, 3, 9, 20, 8};

  {
    auto items_recursive = items;
    quicksort_recursive(items_recursive);
    std::cout << "recursive quicksort: \n";
    for (const auto item :
         items_recursive | std::views::take(items_recursive.size() - 1)) {
      std::cout << std::format("{}, ", item);
    }
    std::cout << std::format("{}\n", items_recursive.back());
  }

  return 0;
}
