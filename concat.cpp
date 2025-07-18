#include <format>
#include <iostream>
#include <span>

std::string concat_recursive(const std::span<std::string> words) {
  if (words.size() == 1) {
    return words[0];
  }
  return words[0] + concat_recursive(words.subspan(1, words.size() - 1));
}

std::string concat_iterative(const std::span<std::string> words) {
  std::string result;
  for (const auto& word : words) {
    result.append(word);
  }
  return result;
}

std::string concat_iterative_recursive(const std::span<std::string> words) {
  enum class return_address_e { recursive };
  struct frame_t {
    std::optional<return_address_e> return_address;
    std::span<std::string> words;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.words = words});

  std::string return_value;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.words.size() == 1) {
        return_value = top.words[0];
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive;
      call_stack.push(
        frame_t{.words = top.words.subspan(1, top.words.size() - 1)});
    } else if (top.return_address == return_address_e::recursive) {
      return_value = top.words[0] + return_value;
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  {
    std::cout << std::format("recursive: \n");
    std::vector<std::string> words = {"hello", "lovely", "world"};
    const auto concatenated_words = concat_recursive(words);
    std::cout << std::format("{}\n", concatenated_words);
  }
  std::cout << '\n';
  {
    std::cout << std::format("iterative: \n");
    std::vector<std::string> words = {"hello", "lovely", "world"};
    const auto concatenated_words = concat_iterative(words);
    std::cout << std::format("{}\n", concatenated_words);
  }
  std::cout << '\n';
  {
    std::cout << std::format("iterative recursive: \n");
    std::vector<std::string> words = {"hello", "lovely", "world"};
    const auto concatenated_words = concat_iterative_recursive(words);
    std::cout << std::format("{}\n", concatenated_words);
  }

  return 0;
}
