#include <format>
#include <iostream>

void say_hello_iterative(int n) {
  for (int i = 0; i < n; i++) {
    std::cout << std::format("hello {}\n", i + 1);
  }
}

void say_hello_recursive(int n, int i = 0) {
  if (i < n) {
    std::cout << std::format("hello {}\n", i + 1);
    say_hello_recursive(n, i + 1);
  }
}

int find_substring_iterative(const char* needle, const char* haystack) {
  int i = 0;
  const int haystack_length = strlen(haystack);
  const int needle_length = strlen(needle);
  while (i < haystack_length) {
    if (strncmp(haystack + i, needle, needle_length) == 0) {
      return i;
    }
    i++;
  }
  return -1;
}

int find_substring_recursive(
  const char* needle, const char* haystack, int i = 0) {
  if (i >= strlen(haystack)) {
    return -1;
  }
  const int needle_length = strlen(needle);
  if (strncmp(haystack + i, needle, needle_length) == 0) {
    return i;
  } else {
    return find_substring_recursive(needle, haystack, i + 1);
  }
}

int main(int argc, char** argv) {
  say_hello_iterative(5);
  std::cout << "---\n";
  say_hello_recursive(5);

  std::cout << '\n';

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
  return 0;
}
