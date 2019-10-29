#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "Command.h"
#include "Game.h"

namespace Sep
{
  class Game;
  class Worm;

  //----------------------------------------------------------------------------
  // State Class
  // shows information about worms, weapons, players
  //
  class State : public Command
  {
    private:
      //------------------------------------------------------------------------
      // Private copy constructor
      //
      State(const State &original);

      //------------------------------------------------------------------------
      // Private assignment operator
      //
      State& operator=(const State &original);

    public:
      //------------------------------------------------------------------------
      // Constructor
      //
      State(std::string name);

      // -----------------------------------------------------------------------
      // Destructor
      //
      virtual ~State();

      //------------------------------------------------------------------------
      // Executes the Command State.
      // @param board The board where action should be performed on
      // @param params Possible parameters needed for the execution
      // @return Integer representing the success of the action
      //
      virtual int execute(Game &board, std::vector<std::string> &params);

      //------------------------------------------------------------------------
      // method that prints all available weapons of a worm
      // @param active_worm the worm whose available weapons should be printed
      //
      void stateWeaponsPrinter(const Worm *active_worm) const;

      //------------------------------------------------------------------------
      // method that prints all living worms in the vector worm_list_ starting
      // from index from to index to
      // @param from and to
      //
      void stateWormsPrinter(Game &game, const int from, const int to) const;
  };
}

#endif //STATE_H_INCLUDED
