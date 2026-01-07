#include <exception>
#include <iostream>
#include "Commands.hpp"
#include "Macro.h"

// int main(void) {
//   try {
//     Commands commands;
//     commands.run();
//   } catch (const std::exception &e) {
//     std::cerr << "ERROR " << e.what() << std::endl;
//     return FAILURE;
//   }
//   return SUCCESS;
// }

int main(void) {
  Commands commands;
  commands.run();
  
  return SUCCESS;
}
