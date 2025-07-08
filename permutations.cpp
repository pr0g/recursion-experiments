#include <format>
#include <iostream>
#include <span>

std::vector<std::string> get_permutations(
  const std::string& chars, int indent_length = 0) {
  const auto indent = std::string(indent_length, '.');
  std::cout << std::format("{}Start of get_permutations ({})\n", indent, chars);
  if (chars.size() == 1) {
    std::cout << std::format(
      "{}When chars = ({}), base case returns\n", indent, chars);
    return std::vector<std::string>(1, std::string(chars.begin(), chars.end()));
  }
  std::vector<std::string> permutations;
  const auto head = chars[0];
  const auto tail = chars.substr(1);
  const auto tail_permutations = get_permutations(tail, indent_length + 1);
  for (const auto tail_permutation : tail_permutations) {
    std::cout << std::format(
      "{}When chars = ({}), putting head {} in all places in {}\n", indent,
      chars, head, tail);
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

std::vector<std::string> get_permutations_with_repetitions(
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
    const auto next_permutations = get_permutations_with_repetitions(
      characters, *permutation_length - 1, new_prefix);
    results.insert(
      results.end(), next_permutations.begin(), next_permutations.end());
  }
  return results;
}

int main(int argc, char** argv) {
  {
    const auto letters = std::string{"ABCD"};
    const auto permutations = get_permutations(letters);

    std::cout << '\n';

    std::cout << "recursive permutations:\n";
    for (const auto permutation : permutations) {
      std::cout << permutation << '\n';
    }
  }

  std::cout << '\n';

  {
    const auto letters = std::string{"JPB123"};
    const auto permutations = get_permutations_with_repetitions(letters, 4);

    std::cout << '\n';

    std::cout << "recursive permutations with repetitions:\n";
    for (const auto permutation : permutations) {
      std::cout << permutation << '\n';
    }
  }

  return 0;
}
