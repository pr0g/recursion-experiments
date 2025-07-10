#include <format>
#include <iostream>

void shortest_with_base_case(bool make_recursive_call) {
  std::cout << std::format("called with ({})\n", make_recursive_call);
  if (!make_recursive_call) {
    std::cout << std::format("returning from base case\n");
    return;
  }
  shortest_with_base_case(false);
  std::cout << std::format("returning from recursive case\n");
}

int main(int argc, char** argv) {
  std::cout << "shortest with base case (false):\n";
  shortest_with_base_case(false);
  std::cout << std::format("\n---\n\n");
  std::cout << "shortest with base case (true):\n";
  shortest_with_base_case(true);
  return 0;
}
