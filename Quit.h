#ifndef QUIT_H_INCLUDED
#define QUIT_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "Command.h"
#include "Game.h"

namespace Sep
{
  class Game;

  //----------------------------------------------------------------------------
  // Quit Class
  // ends the game without further output, inherits from Command
  //
  class Quit : public Command
  {
    private:
      //------------------------------------------------------------------------
      // Private copy constructor
      //
      Quit(const Quit &original);

      //------------------------------------------------------------------------
      // Private assignment operator
      //
      Quit& operator=(const Quit &original);

    public:
      //------------------------------------------------------------------------
      // Constructor
      //
      Quit(std::string name);

      //------------------------------------------------------------------------
      // Destructor
      //
      virtual ~Quit();

      //------------------------------------------------------------------------
      // Executes the Command Quit.
      // @param board The board where action should be performed on
      // @param params Possible parameters needed for the execution
      // @return Integer representing the success of the action
      //
      virtual int execute(Game &board, std::vector<std::string> &params);
  };
}

#endif //QUIT_H_INCLUDED
