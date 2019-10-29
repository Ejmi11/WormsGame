#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "Command.h"
#include "Game.h"

namespace Sep
{
  class Game;
  //----------------------------------------------------------------------------
  // Move Class
  // controlls move values and sets direciton and amount of steps
  // calls move method from game to make movement
  // inherits from class
  //
  class Move : public Command
  {
    private:
      //------------------------------------------------------------------------
      // Private copy constructor
      //
      Move(const Move &original);

      //------------------------------------------------------------------------
      // Private assignment operator
      //
      Move& operator=(const Move &original);

    public:
      //------------------------------------------------------------------------
      // Constructor
      //
      Move(std::string name);

      //------------------------------------------------------------------------
      // Destructor
      //
      virtual ~Move();

      //------------------------------------------------------------------------
      // Executes the Command Move.
      // @param board The board where action should be performed on
      // @param params Possible parameters needed for the execution
      // @return Integer representing the success of the action
      //
      virtual int execute(Game &board, std::vector<std::string> &params);
  };
}

#endif //MOVE_H_INCLUDED
