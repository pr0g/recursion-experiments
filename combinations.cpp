#include <format>
#include <iostream>

std::vector<std::string> get_combinations_logging(
  const std::string& characters, int k, int indent_count = 0) {
  std::string indent(indent_count, '.');
  const std::string debug_message =
    std::format("{}In get_combinations(\"{}\", \"{}\")", indent, characters, k);
  std::cout << std::format("{} start\n", debug_message);
  if (k == 0) {
    std::cout << std::format("{} base case returns [\"\"]\n", debug_message);
    return std::vector<std::string>({""});
  } else if (characters == "") {
    std::cout << std::format("{} base case returns []\n", debug_message);
    return std::vector<std::string>();
  }
  std::vector<std::string> combinations;
  const std::string head = characters.substr(0, 1);
  const std::string tail = characters.substr(1);
  std::cout << std::format(
    "{} part 1, get combinations with head \"{}\"\n", debug_message, head);
  const auto tail_combinations =
    get_combinations_logging(tail, k - 1, indent_count + 1);
  std::cout << std::format(
    "{}Adding head \"{}\" to tail combinations\n", indent, head);
  for (const auto& tail_combination : tail_combinations) {
    std::cout << std::format(
      "{}New combination {}\n", indent, head + tail_combination);
    combinations.push_back(head + tail_combination);
  }
  std::cout << std::format(
    "{} part 2, get combinations without head \"{}\"\n", debug_message, head);
  const auto next_combinations =
    get_combinations_logging(tail, k, indent_count + 1);
  combinations.insert(
    combinations.end(), next_combinations.begin(), next_combinations.end());
  std::cout << std::format("{} results are: ", debug_message);
  for (const auto& combination : combinations) {
    std::cout << std::format("{} ", combination);
  }
  std::cout << '\n';
  return combinations;
}

std::vector<std::string> get_combinations(
  const std::string& characters, int k, int indent_count = 0) {
  if (k == 0) {
    return std::vector<std::string>({""});
  } else if (characters == "") {
    return std::vector<std::string>();
  }
  std::vector<std::string> combinations;
  const std::string head = characters.substr(0, 1);
  const std::string tail = characters.substr(1);
  const auto tail_combinations =
    get_combinations(tail, k - 1, indent_count + 1);
  for (const auto& tail_combination : tail_combinations) {
    combinations.push_back(head + tail_combination);
  }
  const auto next_combinations = get_combinations(tail, k, indent_count + 1);
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
