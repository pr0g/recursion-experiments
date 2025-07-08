#include <format>
#include <iostream>
#include <span>

std::vector<std::string> get_permutations(
  const std::string& chars, int indent = 0) {
  const auto prefix = std::string(indent, '.');
  std::cout << std::format("{}Start of get_permutations ({})\n", prefix, chars);
  if (chars.size() == 1) {
    std::cout << std::format(
      "{}When chars = ({}), base case returns\n", prefix, chars);
    return std::vector<std::string>(1, std::string(chars.begin(), chars.end()));
  }
  std::vector<std::string> permutations;
  const auto head = chars[0];
  const auto tail = chars.substr(1);
  const auto tail_permutations = get_permutations(tail, indent + 1);
  for (const auto tail_permutation : tail_permutations) {
    std::cout << std::format(
      "{}When chars = ({}), putting head {} in all places in {}\n", , chars,
      head, tail);
    for (int i = 0; i < tail_permutation.size() + 1; i++) {
      const auto new_permutation =
        tail_permutation.substr(0, i) + head + tail_permutation.substr(i);
      std::cout << std::format(
        "{}New permutation ({})\n", prefix, new_permutation);
      permutations.push_back(new_permutation);
    }
  }
  return permutations;
}

int main(int argc, char** argv) {
  const auto letters = std::string{"ABCD"};
  const auto permutations = get_permutations(letters);

  std::cout << '\n';

  std::cout << "recursive permutations:\n";
  for (const auto permutation : permutations) {
    std::cout << permutation << '\n';
  }

  return 0;
}
