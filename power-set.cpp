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
  const auto head = characters[0];
  const auto tail = characters.substr(1);
  // get the sets that don't include the head
  const auto tail_power_set = get_power_set_recursive(tail);
  std::vector<std::string> power_set;
  // get the sets that include the head
  for (const auto& tail_set : tail_power_set) {
    power_set.push_back(head + tail_set);
  }
  power_set.insert(
    power_set.end(), tail_power_set.begin(), tail_power_set.end());
  return power_set;
}

std::vector<std::string> get_power_set_iterative_recursive(
  const std::string& characters) {
  enum class return_address_e { recursive };
  struct frame_t {
    std::optional<return_address_e> return_address;
    std::string characters;
    std::string head;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.characters = characters});
  std::vector<std::string> return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.characters.empty()) {
        return_value = std::vector<std::string>(1, "");
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive;
      top.head = top.characters[0];
      call_stack.push(frame_t{.characters = top.characters.substr(1)});
    } else if (top.return_address == return_address_e::recursive) {
      const auto tail_power_set = return_value;
      std::vector<std::string> power_set;
      // get the sets that include the head
      for (const auto& tail_set : tail_power_set) {
        power_set.push_back(top.head + tail_set);
      }
      power_set.insert(
        power_set.end(), tail_power_set.begin(), tail_power_set.end());
      return_value = power_set;
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  {
    std::cout << "recursive power set:\n";
    const auto power_set = get_power_set_recursive("ABC");
    for (const auto& set : power_set) {
      std::cout << set << '\n';
    }
  }
  {
    std::cout << "iterative power set:\n";
    const auto power_set = get_power_set_iterative_recursive("ABC");
    for (const auto& set : power_set) {
      std::cout << set << '\n';
    }
  }
  return 0;
}
