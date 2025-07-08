#include <format>
#include <iostream>

std::vector<std::string> get_combinations(
  const std::string& characters, int k, int indent = 0) {
  if (k == 0) {
    return std::vector<std::string>({""});
  } else if (characters == "") {
    return std::vector<std::string>();
  }
  std::vector<std::string> combinations;
  const std::string head = characters.substr(0, 1);
  const std::string tail = characters.substr(1);
  const auto tail_combos = get_combinations(tail, k - 1, indent + 1);
  for (const auto& tail_combo : tail_combos) {
    combinations.push_back(head + tail_combo);
  }
  const auto next_combinations = get_combinations(tail, k, indent + 1);
  combinations.insert(
    combinations.end(), next_combinations.begin(), next_combinations.end());
  return combinations;
}

int main(int argc, char** argv) {
  {
    const auto combinations = get_combinations("ABC", 2);
    std::cout << "recursive combinations:\n";
    for (const auto& combination : combinations) {
      std::cout << std::format("{}\n", combination);
    }
  }
  std::cout << "\n";
  {
    const auto combinations = get_combinations("ABC", 3);
    std::cout << "recursive combinations:\n";
    for (const auto& combination : combinations) {
      std::cout << std::format("{}\n", combination);
    }
  }
  std::cout << "\n";
  {
    const auto combinations = get_combinations("ABC", 1);
    std::cout << "recursive combinations:\n";
    for (const auto& combination : combinations) {
      std::cout << std::format("{}\n", combination);
    }
  }
  std::cout << "\n";
  {
    const auto combinations = get_combinations("ABCDE", 3);
    std::cout << "recursive combinations:\n";
    for (const auto& combination : combinations) {
      std::cout << std::format("{}\n", combination);
    }
  }
  return 0;
}
