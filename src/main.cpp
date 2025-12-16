#include <exception>
#include <iostream>
#include "Macro.h"

int main(int ac, char **av) {
  try {
    std::cout << "Gomoku game started with " << ac << " arguments."
              << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return FAILURE;
  }
  return SUCCESS;
}
