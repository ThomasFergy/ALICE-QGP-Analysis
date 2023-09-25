#include <TSystem.h>

#include <iostream>

// clang-format off
#include "../lib/Lib/Lib.h" // for syntax highlighting
// MACROS
R__ADD_INCLUDE_PATH(lib/Lib)
R__LOAD_LIBRARY(lib/Lib/Lib_cpp.so)
// clang-format on

void HelloWorld() {
  std::cout << "Hello World! " << std::endl;
  std::cout << Lib::add(1, 2) << std::endl;
  std::cout << Lib::multiply(9, 3) << std::endl;
}
