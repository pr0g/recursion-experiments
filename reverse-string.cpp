#include <format>
#include <iostream>
#include <string_view>

std::string reverse_string_iterative(std::string_view string) {
  std::string result = std::string(string);
  for (int i = 0, half_length = string.size() / 2; i < half_length; i++) {
    const char temp = result[i];
    const int last = string.size() - 1 - i;
    result[i] = result[last];
    result[last] = temp;
  }
  return result;
}

std::string reverse_string_recursive(std::string_view string) {
  if (string.empty()) {
    return "";
  }
  return reverse_string_recursive(string.substr(1, string.size()))
    .append(string.substr(0, 1));
}

int main(int argc, char** argv) {
  std::cout << std::format(
    "reversed string \"{}\" is \"{}\"\n", "hello",
    reverse_string_iterative("hello"));
  std::cout << std::format(
    "reversed string \"{}\" is \"{}\"\n", "something else",
    reverse_string_iterative("something else"));
  std::cout << "---\n";
  std::cout << std::format(
    "reversed string \"{}\" is \"{}\"\n", "hello",
    reverse_string_recursive("hello"));
  std::cout << std::format(
    "reversed string \"{}\" is \"{}\"\n", "something else",
    reverse_string_recursive("something else"));
  return 0;
}
