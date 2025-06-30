#include <format>
#include <iostream>

std::ostream_iterator<char> g_out(std::cout);

void c() {
  std::format_to(g_out, "c() was called\n");
  std::format_to(g_out, "c() is returning\n");
}

void b() {
  std::format_to(g_out, "b() was called\n");
  c();
  std::format_to(g_out, "b() is returning\n");
}

void a() {
  std::format_to(g_out, "a() was called\n");
  b();
  std::format_to(g_out, "a() is returning\n");
}

int main(int argc, char** argv) {
  a();
}
