#include <format>
#include <iostream>
#include <span>

std::vector<std::string> get_permutations_recursive(
  const std::string& characters, int indent_length = 0) {
  const auto indent = std::string(indent_length, '.');
  std::cout << std::format(
    "{}Start of get_permutations_recursive ({})\n", indent, characters);
  if (characters.size() == 1) {
    std::cout << std::format(
      "{}When characters = ({}), base case returns\n", indent, characters);
    return std::vector<std::string>(
      1, std::string(characters.begin(), characters.end()));
  }
  const auto head = characters[0];
  const auto tail = characters.substr(1);
  const auto tail_permutations =
    get_permutations_recursive(tail, indent_length + 1);
  std::vector<std::string> permutations;
  for (const auto tail_permutation : tail_permutations) {
    std::cout << std::format(
      "{}When characters = ({}), putting head {} in all places in {}\n", indent,
      characters, head, tail);
    for (int i = 0; i < tail_permutation.size() + 1; i++) {
      const auto new_permutation =
        tail_permutation.substr(0, i) + head + tail_permutation.substr(i);
      std::cout << std::format(
        "{}New permutation ({})\n", indent, new_permutation);
      permutations.push_back(new_permutation);
    }
  }
  return permutations;
}

std::vector<std::string> get_permutations_iterative_recursive(
  const std::string& characters, int indent_length = 0) {
  enum class return_address_e { before, recursive };
  struct frame_t {
    return_address_e return_address;
    std::string characters;
    int indent_length;
    char head;
    std::string tail;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{
      .return_address = return_address_e::before,
      .characters = characters,
      .indent_length = indent_length});
  std::vector<std::string> return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      const auto indent = std::string(top.indent_length, '.');
      std::cout << std::format(
        "{}Start of get_permutations_recursive ({})\n", indent, top.characters);
      if (top.characters.size() == 1) {
        std::cout << std::format(
          "{}When characters = ({}), base case returns\n", indent,
          top.characters);
        return_value = std::vector<std::string>(
          1, std::string(top.characters.begin(), top.characters.end()));
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive;
      top.head = top.characters[0];
      top.tail = top.characters.substr(1);
      call_stack.push(
        frame_t{
          .return_address = return_address_e::before,
          .characters = top.tail,
          .tail = top.tail,
          .indent_length = top.indent_length + 1});
    } else if (top.return_address == return_address_e::recursive) {
      std::vector<std::string> tail_permutations = return_value;
      const auto indent = std::string(top.indent_length, '.');
      std::vector<std::string> permutations;
      for (const auto tail_permutation : tail_permutations) {
        std::cout << std::format(
          "{}When characters = ({}), putting head {} in all places in {}\n",
          indent, top.characters, top.head, top.tail);
        for (int i = 0; i < tail_permutation.size() + 1; i++) {
          const auto new_permutation = tail_permutation.substr(0, i) + top.head
                                     + tail_permutation.substr(i);
          std::cout << std::format(
            "{}New permutation ({})\n", indent, new_permutation);
          permutations.push_back(new_permutation);
        }
      }
      return_value = permutations;
      call_stack.pop();
    }
  }
  return return_value;
}

std::vector<std::string> get_permutations_with_repetitions_recursive(
  const std::string& characters,
  std::optional<int> permutation_length = std::nullopt,
  const std::string& prefix = "") {
  if (permutation_length == std::nullopt) {
    permutation_length = characters.length();
  }
  if (permutation_length == 0) {
    return std::vector<std::string>{prefix};
  }
  const auto indent = std::string(prefix.length(), '.');
  std::cout << std::format(
    "{}Start, args=(\"{}\"), {}, {}\n", indent, characters, *permutation_length,
    prefix);
  std::vector<std::string> results;
  std::cout << std::format(
    "{}Adding each char to prefix \"{}\"\n", indent, prefix);
  for (const auto character : characters) {
    std::string new_prefix = prefix + character;
    const auto next_permutations = get_permutations_with_repetitions_recursive(
      characters, *permutation_length - 1, new_prefix);
    results.insert(
      results.end(), next_permutations.begin(), next_permutations.end());
  }
  return results;
}

int main(int argc, char** argv) {
  {
    const auto letters = std::string{"ABCD"};
    {
      const auto permutations_recursive = get_permutations_recursive(letters);
      std::cout << "\nrecursive permutations:\n";
      for (const auto permutation : permutations_recursive) {
        std::cout << permutation << '\n';
      }
    }
    std::cout << "\n";
    {
      const auto permutations_iterative =
        get_permutations_iterative_recursive(letters);
      std::cout << "\niterative permutations:\n";
      for (const auto permutation : permutations_iterative) {
        std::cout << permutation << '\n';
      }
    }
  }

  std::cout << '\n';

  {
    const auto letters = std::string{"JPB123"};
    const auto permutations =
      get_permutations_with_repetitions_recursive(letters, 4);

    std::cout << '\n';

    std::cout << "recursive permutations with repetitions:\n";
    for (const auto permutation : permutations) {
      std::cout << permutation << '\n';
    }
  }

  return 0;
}
