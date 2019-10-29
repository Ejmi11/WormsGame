#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "Command.h"
#include "Game.h"

namespace Sep
{
  class Game;
  //----------------------------------------------------------------------------
  // Map Class
  // class to model command map
  //
  class Map : public Command
  {
    private:
      //------------------------------------------------------------------------
      // Private copy constructor
      //
      Map(const Map &original);

      //------------------------------------------------------------------------
      // Private assignment operator
      //
      Map& operator=(const Map &original);

    public:
      //------------------------------------------------------------------------
      // Constructor
      //
      Map(std::string name);

      //------------------------------------------------------------------------
      // Destructor
      //
      virtual ~Map();

      //------------------------------------------------------------------------
      // Executes the Command Map.
      // @param board The board where action should be performed on
      // @param params Possible parameters neede for the execution
      // @return Integer representing the success of the action
      //
      virtual int execute(Game &board, std::vector<std::string> &params);
  };
}

#endif //MAP_H_INCLUDED
