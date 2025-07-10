#include <format>
#include <iostream>

int find_substring_iterative(
  std::string_view needle, std::string_view haystack) {
  for (int i = 0; i < haystack.length(); i++) {
    if (haystack.substr(i, needle.length()) == needle) {
      return i;
    }
  }
  return -1;
}

int find_substring_recursive(
  std::string_view needle, std::string_view haystack, int i = 0) {
  if (i >= haystack.length()) {
    return -1;
  }
  if (haystack.substr(i, needle.length()) == needle) {
    return i;
  } else {
    return find_substring_recursive(needle, haystack, i + 1);
  }
}

int main(int argc, char** argv) {
  std::string needle = "cat";
  std::string haystack = "My cat Ollie";
  std::cout << "find substring iterative:\n";
  const int offset_it =
    find_substring_iterative(needle.c_str(), haystack.c_str());
  std::cout << std::format(
    "Found needle \"{}\" in haystack \"{}\" at offset {}\n", needle, haystack,
    offset_it);
  std::cout << '\n';
  std::cout << "find substring recursive:\n";
  const int offset_rec =
    find_substring_recursive(needle.c_str(), haystack.c_str());
  std::cout << std::format(
    "Found needle \"{}\" in haystack \"{}\" at offset {}\n", needle, haystack,
    offset_rec);
}
