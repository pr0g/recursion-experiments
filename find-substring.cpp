#include <format>
#include <iostream>

int find_substring_iterative(
  std::string_view needle, std::string_view haystack) {
  for (int offset = 0; offset < haystack.length(); offset++) {
    if (haystack.substr(offset, needle.length()) == needle) {
      return offset;
    }
  }
  return -1;
}

int find_substring_recursive(
  std::string_view needle, std::string_view haystack, int offset = 0) {
  if (offset >= haystack.length()) {
    return -1;
  }
  if (haystack.substr(offset, needle.length()) == needle) {
    return offset;
  } else {
    return find_substring_recursive(needle, haystack, offset + 1);
  }
}

int find_substring_iterative_recursive(
  std::string_view needle, std::string_view haystack, int offset = 0) {
  enum class return_address_e { recursive };
  struct frame_t {
    std::optional<return_address_e> return_address;
    int offset;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{ .offset = offset});
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.offset >= haystack.length()) {
        // return immediately (tail call)
        return -1;
      }
      top.return_address = return_address_e::recursive;
      if (haystack.substr(top.offset, needle.length()) == needle) {
        // return immediately (tail call)
        return top.offset;
      } else {
        call_stack.push(
          frame_t{
            
            .offset = top.offset + 1});
      }
    } else if (top.return_address == return_address_e::recursive) {
      // noop
    }
  }
  return -1;
}

int main(int argc, char** argv) {
  std::string needle = "cat";
  std::string haystack = "My cat Ollie";
  {
    std::cout << "find substring iterative:\n";
    const int offset =
      find_substring_iterative(needle.c_str(), haystack.c_str());
    std::cout << std::format(
      "Found needle \"{}\" in haystack \"{}\" at offset {}\n", needle, haystack,
      offset);
  }
  std::cout << '\n';
  {
    std::cout << "find substring recursive:\n";
    const int offset =
      find_substring_recursive(needle.c_str(), haystack.c_str());
    std::cout << std::format(
      "Found needle \"{}\" in haystack \"{}\" at offset {}\n", needle, haystack,
      offset);
  }
  std::cout << '\n';
  {
    std::cout << "find substring iterative recursive:\n";
    const int offset =
      find_substring_iterative_recursive(needle.c_str(), haystack.c_str());
    std::cout << std::format(
      "Found needle \"{}\" in haystack \"{}\" at offset {}\n", needle, haystack,
      offset);
  }
}
