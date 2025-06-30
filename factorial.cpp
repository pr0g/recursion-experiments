#include <format>
#include <iostream>
#include <stack>

std::ostream_iterator<char> g_out(std::cout);

int factorial_iterative(int number) {
  int fact = 1;
  for (int index = number; index > 0; index--) {
    fact *= number;
    number--;
  }
  return fact;
}

// 1. What is the base case?
// 2. What argument is passed to the recursive function call?
// 3. How does the argument become closer to the base case?

// half of the call happens before the recursive call, half takes place
// after the recursive call returns

// first half is factorial_recursive(number - 1)
// second half is <number> * <return value> (e.g. 5 * 24)

int factorial_recursive(int number) {
  if (number <= 1) {
    return 1;
  }
  return number * factorial_recursive(number - 1);
}

int factorial_recursive_iterative(int n) {
  enum class return_address_e { before, recursive };
  struct frame_t {
    return_address_e return_address;
    int n;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.return_address = return_address_e::before, .n = n});

  int return_value = -1;
  while (call_stack.size() > 0) {
    // body of the factorial function
    int n = call_stack.top().n;
    return_address_e return_address = call_stack.top().return_address;

    if (return_address == return_address_e::before) {
      if (n <= 1) {
        return_value = 1;
        call_stack.pop();
        continue;
      } else {
        call_stack.top().return_address = return_address_e::recursive;
        // call the factorial function
        call_stack.push(
          frame_t{.return_address = return_address_e::before, .n = n - 1});
        continue;
      }
    } else if (return_address == return_address_e::recursive) {
      return_value = n * return_value;
      call_stack.pop();
      continue;
    }
  }

  return return_value;
}

int main(int argc, char** argv) {
  std::format_to(g_out, "{}! (it) is {}\n", 5, factorial_iterative(5));
  std::format_to(g_out, "{}! (rec) is {}\n", 5, factorial_recursive(5));

  std::format_to(
    g_out, "{}! (rec_it) is {}\n", 5, factorial_recursive_iterative(5));

  // for (int i = 0; i < 6; i++) {
  //   std::format_to(
  //     g_out, "{}! (rec_it) is {}\n", i, factorial_recursive_iterative(i));
  // }
  return 0;
}
