#include <format>
#include <iostream>

int ackermann_recursive(int m, int n, int indentation = 0) {
  std::string indent;
  std::generate_n(std::back_inserter(indent), indentation, [] { return ' '; });
  std::cout << std::format("{} ackermann({}, {})\n", indent, m, n);
  if (m == 0) {
    return n + 1;
  } else if (m > 0 && n == 0) {
    return ackermann_recursive(m - 1, 1, indentation + 1);
  } else if (m > 0 && n > 0) {
    return ackermann_recursive(
      m - 1, ackermann_recursive(m, n - 1, indentation + 1), indentation + 1);
  }
  return 0;
}

int ackermann_iterative_recursive(int m, int n, int indentation = 0) {
  enum class return_address_e { before, recursive_1, recursive_2, recursive_3 };
  struct frame_t {
    return_address_e return_address;
    int m;
    int n;
    int indentation;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{
      .return_address = return_address_e::before,
      .m = m,
      .n = n,
      .indentation = indentation});
  int return_value = 0;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      std::string indent;
      std::generate_n(
        std::back_inserter(indent), top.indentation, [] { return ' '; });
      std::cout << std::format("{} ackermann({}, {})\n", indent, top.m, top.n);
      if (top.m == 0) {
        return_value = top.n + 1;
        call_stack.pop();
      } else if (top.m > 0 && top.n == 0) {
        top.return_address = return_address_e::recursive_1;
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .m = top.m - 1,
            .n = 1,
            .indentation = top.indentation + 1});
      } else if (top.m > 0 && top.n > 0) {
        top.return_address = return_address_e::recursive_2;
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .m = top.m,
            .n = top.n - 1,
            .indentation = top.indentation + 1});
      }
    } else if (top.return_address == return_address_e::recursive_1) {
      call_stack.pop();
    } else if (top.return_address == return_address_e::recursive_2) {
      top.return_address = return_address_e::recursive_3;
      call_stack.push(
        frame_t{
          .return_address = return_address_e::before,
          .m = top.m - 1,
          .n = return_value,
          .indentation = top.indentation + 1});
    } else if (top.return_address == return_address_e::recursive_3) {
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  {
    const int result1 = ackermann_recursive(1, 1);
    std::cout << std::format("\nAckermann (1, 1) - {}\n", result1);

    std::cout << '\n';

    const int result2 = ackermann_recursive(2, 3);
    std::cout << std::format("\nAckermann (2, 3) - {}\n", result2);

    std::cout << '\n';

    const int result3 = ackermann_recursive(3, 3);
    std::cout << std::format("\nAckermann (3, 3) - {}\n", result3);
  }

  std::cout << '\n';

  {
    const int result1 = ackermann_iterative_recursive(1, 1);
    std::cout << std::format("\nAckermann (1, 1) - {}\n", result1);

    std::cout << '\n';

    const int result2 = ackermann_iterative_recursive(2, 3);
    std::cout << std::format("\nAckermann (2, 3) - {}\n", result2);

    std::cout << '\n';

    const int result3 = ackermann_iterative_recursive(3, 3);
    std::cout << std::format("\nAckermann (3, 3) - {}\n", result3);
  }
  return 0;
}
