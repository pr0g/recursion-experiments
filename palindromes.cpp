#include <format>
#include <iostream>
#include <optional>
#include <string_view>

bool palindrome_iterative(std::string_view phrase) {
  const auto half_length = phrase.length() / 2;
  for (int i = 0; i < half_length; i++) {
    if (phrase[i] != phrase[phrase.length() - 1 - i]) {
      return false;
    }
  }
  return true;
}

bool palindrome_recursive(std::string_view phrase) {
  if (phrase.empty() || phrase.length() == 1) {
    return true;
  }
  bool match = phrase[0] == phrase[phrase.length() - 1];
  return match && palindrome_recursive(phrase.substr(1, phrase.length() - 2));
}

bool palindrome_iterative_recursive(std::string_view phrase) {
  enum class return_address_e { recursive };
  struct frame_t {
    std::optional<return_address_e> return_address;
    std::string_view phrase;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.phrase = phrase});
  bool return_value = false;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.phrase.empty() || top.phrase.length() == 1) {
        return_value = true;
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive;
      call_stack.push(
        frame_t{.phrase = top.phrase.substr(1, top.phrase.length() - 2)});
    } else if (top.return_address == return_address_e::recursive) {
      const bool match = top.phrase[0] == top.phrase[top.phrase.length() - 1];
      return_value = match && return_value;
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  std::cout << "palindromes recursive:\n";
  std::cout << std::format(
    "{} {} a palindrome\n", "tacocat",
    palindrome_recursive("tacocat") ? "is" : "is not");
  std::cout << std::format(
    "{} {} a palindrome\n", "racecar",
    palindrome_recursive("racecar") ? "is" : "is not");
  std::cout << std::format(
    "{} {} a palindrome\n", "zophie",
    palindrome_recursive("zophie") ? "is" : "is not");
  std::cout << std::format(
    "{} {} a palindrome\n", "amanaplanacanalpanama",
    palindrome_recursive("amanaplanacanalpanama") ? "is" : "is not");
  std::cout << '\n';
  std::cout << "palindromes iterative:\n";
  std::cout << std::format(
    "{} {} a palindrome\n", "tacocat",
    palindrome_iterative("tacocat") ? "is" : "is not");
  std::cout << std::format(
    "{} {} a palindrome\n", "racecar",
    palindrome_iterative("racecar") ? "is" : "is not");
  std::cout << std::format(
    "{} {} a palindrome\n", "zophie",
    palindrome_iterative("zophie") ? "is" : "is not");
  std::cout << std::format(
    "{} {} a palindrome\n", "amanaplanacanalpanama",
    palindrome_iterative("amanaplanacanalpanama") ? "is" : "is not");
  std::cout << '\n';
  std::cout << "palindromes iterative recursive:\n";
  std::cout << std::format(
    "{} {} a palindrome\n", "tacocat",
    palindrome_iterative_recursive("tacocat") ? "is" : "is not");
  std::cout << std::format(
    "{} {} a palindrome\n", "racecar",
    palindrome_iterative_recursive("racecar") ? "is" : "is not");
  std::cout << std::format(
    "{} {} a palindrome\n", "zophie",
    palindrome_iterative_recursive("zophie") ? "is" : "is not");
  std::cout << std::format(
    "{} {} a palindrome\n", "amanaplanacanalpanama",
    palindrome_iterative_recursive("amanaplanacanalpanama") ? "is" : "is not");
  std::cout << std::format(
    "{} {} a palindrome\n", "abcdecba",
    palindrome_iterative_recursive("abcdecba") ? "is" : "is not");

  return 0;
}
