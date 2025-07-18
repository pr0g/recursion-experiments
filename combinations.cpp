#include <format>
#include <iostream>
#include <ranges>

std::vector<std::string> get_combinations_recursive_logging(
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
    get_combinations_recursive_logging(tail, k - 1, indent_count + 1);
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
    get_combinations_recursive_logging(tail, k, indent_count + 1);
  combinations.insert(
    combinations.end(), next_combinations.begin(), next_combinations.end());
  std::cout << std::format("{} results are: ", debug_message);
  for (const auto& combination : combinations) {
    std::cout << std::format("{} ", combination);
  }
  std::cout << '\n';
  return combinations;
}

std::vector<std::string> get_combinations_recursive(
  const std::string& characters, int k) {
  if (k == 0) {
    return std::vector<std::string>({""});
  } else if (characters == "") {
    return std::vector<std::string>();
  }
  std::vector<std::string> combinations;
  const std::string head = characters.substr(0, 1);
  const std::string tail = characters.substr(1);
  const auto tail_combinations = get_combinations_recursive(tail, k - 1);
  for (const auto& tail_combination : tail_combinations) {
    combinations.push_back(head + tail_combination);
  }
  const auto next_combinations = get_combinations_recursive(tail, k);
  combinations.insert(
    combinations.end(), next_combinations.begin(), next_combinations.end());
  return combinations;
}

std::vector<std::string> get_combinations_iterative_recursive(
  const std::string& characters, int k) {
  enum class return_address_e { recursive_1, recursive_2 };
  struct frame_t {
    std::optional<return_address_e> return_address;
    std::string characters;
    int k;
    std::vector<std::string> combinations;
    std::string head;
    std::string tail;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{
      
      .characters = characters,
      .k = k});
  std::vector<std::string> return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.k == 0) {
        return_value = std::vector<std::string>({""});
        call_stack.pop();
        continue;
      } else if (top.characters.empty()) {
        return_value = std::vector<std::string>();
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive_1;
      top.head = top.characters.substr(0, 1);
      top.tail = top.characters.substr(1);
      call_stack.push(
        frame_t{
          
          .characters = top.tail,
          .k = top.k - 1});
    } else if (top.return_address == return_address_e::recursive_1) {
      top.return_address = return_address_e::recursive_2;
      const auto tail_combinations = return_value;
      for (const auto& tail_combination : tail_combinations) {
        top.combinations.push_back(top.head + tail_combination);
      }
      call_stack.push(
        frame_t{
          
          .characters = top.tail,
          .k = top.k});
    } else if (top.return_address == return_address_e::recursive_2) {
      const auto next_combinations = return_value;
      top.combinations.insert(
        top.combinations.end(), next_combinations.begin(),
        next_combinations.end());
      return_value = top.combinations;
      call_stack.pop();
    }
  }
  return return_value;
}

template<typename T>
std::vector<std::vector<T>> get_combinations_any_recursive(
  const std::vector<T>& values, int k) {
  if (k == 0) {
    return std::vector<std::vector<T>>(1, std::vector<T>());
  } else if (values.empty()) {
    return std::vector<std::vector<T>>();
  }
  std::vector<std::vector<T>> combinations;
  const auto head_view = values | std::views::take(1);
  const auto tail_view = values | std::views::drop(1);
  const auto tail = std::vector<T>(tail_view.begin(), tail_view.end());
  const auto tail_combinations = get_combinations_any_recursive(tail, k - 1);
  for (const auto& tail_combination : tail_combinations) {
    std::vector<T> next(head_view.begin(), head_view.end());
    next.insert(next.end(), tail_combination.begin(), tail_combination.end());
    combinations.push_back(next);
  }
  const auto next_combinations = get_combinations_any_recursive(tail, k);
  combinations.insert(
    combinations.end(), next_combinations.begin(), next_combinations.end());
  return combinations;
}

int main(int argc, char** argv) {
  {
    const auto combinations = get_combinations_recursive("ABC", 2);
    std::cout << "recursive combinations:\n";
    for (const auto& combination : combinations) {
      std::cout << std::format("{}\n", combination);
    }
  }
  std::cout << '\n';
  {
    const auto combinations = get_combinations_recursive("ABC", 3);
    std::cout << "recursive combinations:\n";
    for (const auto& combination : combinations) {
      std::cout << std::format("{}\n", combination);
    }
  }
  std::cout << '\n';
  {
    const auto combinations = get_combinations_recursive("ABC", 1);
    std::cout << "recursive combinations:\n";
    for (const auto& combination : combinations) {
      std::cout << std::format("{}\n", combination);
    }
  }
  std::cout << '\n';
  {
    const auto combinations = get_combinations_recursive("ABCDE", 3);
    std::cout << "recursive combinations:\n";
    for (const auto& combination : combinations) {
      std::cout << std::format("{}\n", combination);
    }
  }
  std::cout << '\n';
  {
    const auto combinations =
      get_combinations_any_recursive(std::vector<int>{1, 2, 3, 4, 5}, 3);
    std::cout << "recursive combinations (any):\n";
    for (const auto& combination : combinations) {
      for (auto v : combination | std::views::take(combination.size() - 1)) {
        std::cout << std::format("{},", v);
      }
      std::cout << combination[combination.size() - 1] << '\n';
    }
  }
  std::cout << '\n';
  {
    const auto combinations = get_combinations_iterative_recursive("ABCDE", 3);
    std::cout << "iterative combinations:\n";
    for (const auto& combination : combinations) {
      std::cout << std::format("{}\n", combination);
    }
  }
  return 0;
}
