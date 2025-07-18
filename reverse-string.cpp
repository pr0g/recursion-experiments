#include <format>
#include <iostream>
#include <optional>
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

// broken up return
std::string reverse_string_recursive_2(std::string_view string) {
  if (string.empty()) {
    return "";
  }
  auto result = reverse_string_recursive_2(string.substr(1, string.size()));
  result.append(string.substr(0, 1));
  return result;
}

std::string reverse_string_iterative_recursive(std::string_view string) {
  enum class return_address_e { recursive };
  struct frame_t {
    std::string_view string;
    std::optional<return_address_e> return_address;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.string = string});

  std::string return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.string.empty()) {
        return_value = "";
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive;
      call_stack.push(
        frame_t{

          .string = top.string.substr(1, top.string.size())});
    } else if (top.return_address == return_address_e::recursive) {
      return_value = return_value + std::string(top.string.substr(0, 1));
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  std::cout << "reverse string iterative:\n";
  std::cout << std::format(
    "\"{}\" is \"{}\"\n", "hello", reverse_string_iterative("hello"));
  std::cout << std::format(
    "\"{}\" is \"{}\"\n", "something else",
    reverse_string_iterative("something else"));
  std::cout << '\n';
  std::cout << "reverse string recursive:\n";
  std::cout << std::format(
    "\"{}\" is \"{}\"\n", "hello", reverse_string_recursive("hello"));
  std::cout << std::format(
    "\"{}\" is \"{}\"\n", "something else",
    reverse_string_recursive("something else"));
  std::cout << '\n';
  std::cout << "reverse string iterative recursive:\n";
  std::cout << std::format(
    "\"{}\" is \"{}\"\n", "hello", reverse_string_iterative_recursive("hello"));
  std::cout << std::format(
    "\"{}\" is \"{}\"\n", "something else",
    reverse_string_iterative_recursive("something else"));
  return 0;
}
