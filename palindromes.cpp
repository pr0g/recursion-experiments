#include <format>
#include <iostream>
#include <string_view>

bool palindrome_recursive(std::string_view phrase) {
  if (phrase.empty() || phrase.length() == 1) {
    return true;
  }
  bool match = phrase[0] == phrase[phrase.length() - 1];
  return match && palindrome_recursive(phrase.substr(1, phrase.length() - 2));
}

int main(int argc, char** argv) {
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

  return 0;
}
