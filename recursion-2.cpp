#include <format>
#include <iostream>

std::ostream_iterator<char> g_out(std::cout);

void shortest_with_base_case(bool make_recursive_call) {
  std::format_to(
    g_out, "shortest_with_base_case({}) called\n", make_recursive_call);
  if (!make_recursive_call) {
    std::format_to(g_out, "returning from base case\n");
    return;
  }
  shortest_with_base_case(false);
  std::format_to(g_out, "returning from recursive case\n");
}

int main(int argc, char** argv) {
  shortest_with_base_case(false);
  std::format_to(g_out, "---\n");
  shortest_with_base_case(true);
  return 0;
}
