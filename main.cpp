#include <iostream>

#include "vm.hpp"

int main(int argc, char** /*argv*/)
{
  if (argc < 2) {
    std::cout << "Repl!\n";
    return 0;
  }
  else {
    std::cout << "Read file!\n";
  }

  return 0;
}
