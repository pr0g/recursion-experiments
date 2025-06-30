#include <format>
#include <iostream>

std::ostream_iterator<char> g_out(std::cout);

void count_down_and_up(int number) {
  std::format_to(g_out, "{}\n", number);
  if (number == 0) {
    std::format_to(g_out, "reached base case\n");
    return;
  }
  count_down_and_up(number - 1);
  std::format_to(g_out, "{} returning\n", number);
}

int main(int argc, char** argv) {
  count_down_and_up(10);
  return 0;
}
