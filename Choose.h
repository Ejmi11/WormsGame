#ifndef CHOOSE_H_INCLUDED
#define CHOOSE_H_INCLUDED

#include "Command.h"
#include "Game.h"

#include <iostream>
#include <string>
#include <vector>

namespace Sep
{
  class Game;
  class Worm;

  //----------------------------------------------------------------------------
  // Choose Class
  // class to model command choose
  //
  class Choose : public Command
  {
    private:
      //------------------------------------------------------------------------
      // Private copy constructor
      //
      Choose(const Choose &original);

      //------------------------------------------------------------------------
      // Private assignment operator
      //
      Choose& operator=(const Choose &original);

    public:
      //------------------------------------------------------------------------
      // Constructor
      //
      Choose(std::string name);

      //------------------------------------------------------------------------
      // Destructor
      //
      virtual ~Choose();

      //------------------------------------------------------------------------
      // Executes the Command Choose.
      // @param board The board where action should be performed on
      // @param params Possible parameters needed for the execution
      // @return always 0
      //
      virtual int execute(Game &board, std::vector<std::string> &params);
  };
}
#endif //CHOOSE_H_INCLUDED
