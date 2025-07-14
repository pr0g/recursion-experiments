#include <format>
#include <iostream>
#include <stack>

int fibonacci_iterative_1(int n) {
  if (n <= 1) {
    return n;
  }
  int prev = 0;
  int curr = 1;
  for (int i = 1; i < n; i++) {
    int next = prev + curr;
    prev = curr;
    curr = next;
  }
  return curr;
}

int fibonacci_iterative_2(int n) {
  if (n <= 1) {
    return n;
  }
  int prev = 0;
  int curr = 1;
  for (int i = 1; i < n; i++) {
    prev = std::exchange(curr, prev + curr);
  }
  return curr;
}

// 1. What is the base case?
// 2. What argument is passed to the recursive function call?
// 3. How does the argument become closer to the base case?

int fibonacci_recursive_1(int n) {
  if (n <= 1) {
    return n;
  }
  return fibonacci_recursive_1(n - 1) + fibonacci_recursive_1(n - 2);
}

int fibonacci_recursive_2(int n) {
  if (n <= 1) {
    return n;
  }
  int minus1 = fibonacci_recursive_2(n - 1);
  int minus2 = fibonacci_recursive_2(n - 2);
  return minus1 + minus2;
}

int fibonacci_iterative_recursive(const int n) {
  enum class return_address_e { before, recursive1, recursive2 };
  struct frame_t {
    return_address_e return_address;
    int n;
    int minus1;
    int minus2;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.return_address = return_address_e::before, .n = n});
  int return_value = 0;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      if (top.n <= 1) {
        return_value = top.n;
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive1;
      call_stack.push(
        frame_t{.return_address = return_address_e::before, .n = top.n - 1});
    } else if (top.return_address == return_address_e::recursive1) {
      top.return_address = return_address_e::recursive2;
      top.minus1 = return_value;
      call_stack.push(
        frame_t{.return_address = return_address_e::before, .n = top.n - 2});
    } else if (top.return_address == return_address_e::recursive2) {
      top.minus2 = return_value;
      return_value = top.minus1 + top.minus2;
      call_stack.pop();
    }
  }
  return return_value;
}

int fibonacci_recursive_memoized_internal(
  int n, std::unordered_map<int, int>& cache) {
  if (n <= 1) {
    return n;
  }
  if (auto it = cache.find(n); it != cache.end()) {
    return it->second;
  }
  const int minus1 = fibonacci_recursive_memoized_internal(n - 1, cache);
  const int minus2 = fibonacci_recursive_memoized_internal(n - 2, cache);
  cache.insert({n, minus1 + minus2});
  return minus1 + minus2;
}

int fibonacci_recursive_memoized(int n) {
  std::unordered_map<int, int> cache;
  return fibonacci_recursive_memoized_internal(n, cache);
}

int fibonacci_iterative_recursive_memoized(const int n) {
  enum class return_address_e { before, recursive1, recursive2 };
  struct frame_t {
    return_address_e return_address;
    int n;
    int minus1;
    int minus2;
  };
  std::unordered_map<int, int> cache;
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.return_address = return_address_e::before, .n = n});
  int return_value = 0;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      if (top.n <= 1) {
        return_value = top.n;
        call_stack.pop();
        continue;
      }
      if (auto it = cache.find(top.n); it != cache.end()) {
        return_value = it->second;
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive1;
      call_stack.push(
        frame_t{.return_address = return_address_e::before, .n = top.n - 1});
    } else if (top.return_address == return_address_e::recursive1) {
      top.return_address = return_address_e::recursive2;
      top.minus1 = return_value;
      call_stack.push(
        frame_t{.return_address = return_address_e::before, .n = top.n - 2});
    } else if (top.return_address == return_address_e::recursive2) {
      top.minus2 = return_value;
      return_value = top.minus1 + top.minus2;
      cache.insert({top.n, return_value});
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  const int count = 20;
  std::cout << "fibonacci iterative 1:\n";
  for (int i = 0; i <= count; i++) {
    std::cout << std::format("{} is {}\n", i, fibonacci_iterative_1(i));
  }
  std::cout << '\n';
  std::cout << "fibonacci iterative 2:\n";
  for (int i = 0; i <= count; i++) {
    std::cout << std::format("{} is {}\n", i, fibonacci_iterative_2(i));
  }
  std::cout << '\n';
  std::cout << "fibonacci recursive:\n";
  for (int i = 0; i <= count; i++) {
    std::cout << std::format("{} is {}\n", i, fibonacci_recursive_1(i));
  }
  std::cout << '\n';
  std::cout << "fibonacci iterative recursive:\n";
  for (int i = 0; i <= count; i++) {
    std::cout << std::format("{} is {}\n", i, fibonacci_iterative_recursive(i));
  }
  std::cout << '\n';
  std::cout << "fibonacci recursive memoized:\n";
  for (int i = 0; i <= count; i++) {
    std::cout << std::format("{} is {}\n", i, fibonacci_recursive_memoized(i));
  }
  std::cout << '\n';
  std::cout << "fibonacci iterative recursive memoized:\n";
  for (int i = 0; i <= count; i++) {
    std::cout << std::format(
      "{} is {}\n", i, fibonacci_iterative_recursive_memoized(i));
  }
  return 0;
}
