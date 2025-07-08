#include <format>
#include <iostream>
#include <vector>

// A B C
// A B
// A C
// A
// B C
// B
// C

std::vector<std::string> get_power_set_recursive(
  const std::string& characters) {
  if (characters.empty()) {
    return std::vector<std::string>(1, "");
  }
  std::vector<std::string> power_set;
  const auto head = characters[0];
  const auto tail = characters.substr(1);
  // get the sets that don't include the head
  const auto tail_power_set = get_power_set_recursive(tail);
  // get the sets that include the head
  for (const auto& tail_set : tail_power_set) {
    power_set.push_back(head + tail_set);
  }
  power_set.insert(
    power_set.end(), tail_power_set.begin(), tail_power_set.end());
  return power_set;
}

int main(int argc, char** argv) {
  std::cout << "recursive power set:\n";
  const auto power_set = get_power_set_recursive("ABC");
  for (const auto& set : power_set) {
    std::cout << set << '\n';
  }
  return 0;
}
