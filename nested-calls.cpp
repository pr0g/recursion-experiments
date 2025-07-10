#include <format>
#include <iostream>

void c() {
  std::cout << std::format("c() was called\n");
  std::cout << std::format("c() is returning\n");
}

void b() {
  std::cout << std::format("b() was called\n");
  c();
  std::cout << std::format("b() is returning\n");
}

void a() {
  std::cout << std::format("a() was called\n");
  b();
  std::cout << std::format("a() is returning\n");
}

int main(int argc, char** argv) {
  a();
}
