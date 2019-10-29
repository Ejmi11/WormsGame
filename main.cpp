#include <iostream>
#include <string>

#include "Game.h"

namespace Sep
{
  const int RETURN_OK = 0;
  const int RETURN_ERROR_BAD_CONFIG = -1;
  const int RETURN_ERROR_MEMORY = -2;
  const int RETURN_ERROR_ARGUMENT = -3;
  const std::string ERROR_WRONG_ARGUMENTS =
    "[ERROR] usage: ./sworms <config-file>\n";
}

//------------------------------------------------------------------------------
// first program to start
// @param argc number of arguments given to program
// @param argv vector of arguments
// @return RETURN_OK if program terminated correctly
//
int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cout << Sep::ERROR_WRONG_ARGUMENTS;
    return Sep::RETURN_ERROR_ARGUMENT;
  }

  Sep::Game game;

  int return_value = Sep::RETURN_OK;
  return_value = game.runme(argv[1]);

  if (return_value == Sep::RETURN_ERROR_BAD_CONFIG)
  {
    return Sep::RETURN_ERROR_BAD_CONFIG;
  }
  else if (return_value == Sep::RETURN_ERROR_MEMORY)
  {
    return Sep::RETURN_ERROR_MEMORY;
  }

  return Sep::RETURN_OK;
}
