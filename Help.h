#ifndef HELP_H_INCLUDED
#define HELP_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "Command.h"
#include "Game.h"

namespace Sep
{
  class Game;

  //----------------------------------------------------------------------------
  // Help Class
  // prints overview about allowed commands including format of command
  // inherits from Command
  //
  class Help : public Command
  {
    private:
      //------------------------------------------------------------------------
      // Private copy constructor
      //
      Help(const Help &original);

      //------------------------------------------------------------------------
      // Private assignment operator
      //
      Help& operator=(const Help &original);

    public:
      //------------------------------------------------------------------------
      // Constructor
      //
      Help(std::string name);

      //------------------------------------------------------------------------
      // Destructor
      //
      virtual ~Help();

      //------------------------------------------------------------------------
      // Executes the Command Help.
      // @param board The board where action should be performed on
      // @param params Possible parameters needed for the execution
      // @return Integer representing the success of the action
      //
      virtual int execute(Game &board, std::vector<std::string> &params);
  };
}
#endif //HELP_H_INCLUDED
