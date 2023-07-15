#include "std/string.hpp"

extern "C" int _start() {
  std::string test { "Hello World!" };
  test.at(3);
  test.size();
  return 0;
}
