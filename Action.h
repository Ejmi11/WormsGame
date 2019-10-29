#ifndef ACTION_H_INCLUDED
#define ACTION_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "Chest.h"
#include "Command.h"
#include "Game.h"
#include "Worm.h"

namespace Sep
{
  class Worm;
  class Chest;
  class Game;
  class Player;

  //----------------------------------------------------------------------------
  // Action Class
  // executes all action commands
  // regarding targeting, attacking, damaging, falling and dieing
  // inerits from Command
  //
  class Action : public Command
  {
    private:
      //------------------------------------------------------------------------
      // Private copy constructor
      //
      Action(const Action &original);

      //------------------------------------------------------------------------
      // Private assignment operator
      //
      Action& operator=(const Action &original);

      int damage;
      int old_lifepoints;
      int new_lifepoints;
      int action_done;
      int ammunition;
      int attack_direction_row;
      int attack_direction_col;
      int direction_row;
      int direction_col;
      std::string param;
      int x_coordinates;
      int y_coordinates;
      bool worm_dead = false;


    public:
      //------------------------------------------------------------------------
      // Constructor
      //
      Action(std::string name);

      //------------------------------------------------------------------------
      // Destructor
      //
      virtual ~Action();

      //------------------------------------------------------------------------
      // Executes the Command Action.
      // @param board The board where action should be performed on
      // @param params Possible parameters needed for the execution
      // @return Integer representing the success of the action
      //
      virtual int execute(Game &board, std::vector<std::string> &params);

      //------------------------------------------------------------------------
      // Action [direction] with gun shooting at first target next to it
      // @param board The board where action should be performed on
      // @param attack_direction_row in which direction attacks go
      // @param attack_direction_row in which direction attacks go
      // @param gun_row first possible field of attack
      // @param gun_col first possible field of attack
      // @return Integer representing if action happened
      //
      int actionGun(Game &board,
                    int   attack_direction_row,
                    int   attack_direction_col,
                    int   gun_row,
                    int   gun_col);

      //------------------------------------------------------------------------
      // Action [direction] with bazooka shooting at first target next to it
      // and 4 fields (u,r,d,l) around it
      // @param board The board where action should be performed on
      // @param attack_direction_row in which direction attacks go
      // @param attack_direction_row in which direction attacks go
      // @param bazooka_row first possible field of attack
      // @param bazooka_col first possible field of attack
      // @return Integer representing if action happened
      //
      int actionBazooka(Game &board,
                        int   attack_direction_row,
                        int   attack_direction_col,
                        int   bazooka_row,
                        int   bazooka_col);

      //------------------------------------------------------------------------
      // Action [direction] with blowtorch torchs 5 fields next to position
      // @param board The board where action should be performed on
      // @param attack_direction_row in which direction attacks go
      // @param attack_direction_row in which direction attacks go
      // @param blowtorch_row first field of attack
      // @param blowtorch_col first field of attack
      // @return Integer representing if action happened
      //
      int actionBlowtorch(Game &board,
                          int   attack_direction_row,
                          int   attack_direction_col,
                          int   blowtorch_row,
                          int   blowtorch_col);

      //------------------------------------------------------------------------
      // Action [row][col] with teleport teleports worm to choosen position
      // @param board The board where action should be performed on
      // @param worm the currently playing worm
      // @param teleport_row row position of teleport target
      // @param teleport_col col position of teleport target
      // @return Integer representing if action happened
      //
      int actionTeleport(Game &board,
                         Worm *worm,
                         int   teleport_row,
                         int   teleport_col);

      //------------------------------------------------------------------------
      // Action [col] with airstrike shooting at the highest target in column
      // @param board The board where action should be performed on
      // @param airstrike_col column where airstrike should happen
      // @return Integer representing if action happened
      //
      int actionAirstrike(Game &board, int airstrike_col);

      //------------------------------------------------------------------------
      // Action with melee attacking all fields around playing worm
      // @param board The board where action should be performed on
      // @param melee_row row position from where attacks goes around
      // @param melee_col col position from where attacks goes around
      // @return Integer representing if action happened
      //
      int actionMelee(Game &board, int melee_row, int melee_col);

      //------------------------------------------------------------------------
      // checks if Worm falls through teleport
      // @param board The board where action should be performed on
      // @param worm the currently playing worm
      // @param row row position after teleport
      // @param col col position after teleport
      //
      void checkGravityAfterTeleport(Game &board, Worm *worm, int row, int col);

      //------------------------------------------------------------------------
      // reduces lifepoints of attacked worm
      // @param worm the currently attacked worm
      // @param damage corresponding damage of current attack
      //
      void attackDamage(Worm *attacked_worm, int damage);

      //------------------------------------------------------------------------
      // in case of damage through falling reduces lifepoints
      // @param worm the worm which felt down
      // @param row_fallen integer of the fallen rows
      //
      void fallDamage(Worm *worm, int rows_fallen);

      //------------------------------------------------------------------------
      // additionally method for action melee that executes attack
      // @param board The board where action should be performed on
      // @param row row position in the middle of attack radius
      // @param col col position in the middle of attack radius
      //
      void meleeAttacking(Game &board, int row, int col);

      //------------------------------------------------------------------------
      // additionally method for bazooka to attack corresponding fields around
      // @param board The board where action should be performed on
      // @param row row position of first target in the middle
      // @param col col position of first target in the middle
      // @param damage integer of damage points
      // @return Integer representing if action happened
      //
      int bazookaCrossAttacking(Game &board, int row, int col, int damage);

      //------------------------------------------------------------------------
      // method to checks if fields above attacked field can fall
      // @param board The board where action should be performed on
      // @param row row position of field above attacked field
      // @param col col position of field above attacked field
      //
      void gravityAbove(Game &board, int row, int col);

      //------------------------------------------------------------------------
      // method to get a value of direction row for the attack
      // @param param parameter from command for row
      // @return integer depending which direction attack should go in row
      //
      int attackDirectionRow(std::string param);

      //------------------------------------------------------------------------
      // method to get a value of direction col for the attack
      // @param param parameter from command for col
      // @return integer depending which direction attack should go in col
      //
      int attackDirectionCol(std::string param);

      //------------------------------------------------------------------------
      // method to check if parameter is one of allowed directions
      // @param param parameter from command for direction
      // @return true if parameter is allowed direction
      //
      bool correctParameter(std::string param);
  };
}
#endif //ACTION_H_INCLUDED
