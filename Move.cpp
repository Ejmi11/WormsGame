#include "Move.h"

namespace Sep
{
  const int RETURN_SUCCESS = 0;
  const int WRONG_PARAMETER = 1;
  const int INVALID_PARAMETER = 4;

  //----------------------------------------------------------------------------
  Move::Move(std::string name) : Command(name)
  {
  }

  //----------------------------------------------------------------------------
  Move::~Move()
  {
  }

  //----------------------------------------------------------------------------
  int Move::execute(Game &game, std::vector<std::string> &params)
  {
    // if amount of parameters is correct
    if (params.size() == 3)
    {
      std::string left  = "l";
      std::string right = "r";

      // if first parameter after move isn't 'l' or 'r'
      if (params[1] != left && params[1] != right)
      {
        return INVALID_PARAMETER;
      }

      // if steps are more than 3 or less than 0
      if (params[2] > "3" || params[2] < "0")
      {
        return INVALID_PARAMETER;
      }

      int param3 = std::stoi(params[2]);
      int steps_direction = param3;

      // if move is left sets steps minus
      if (params[1] == left)
      {
        steps_direction = ( -1 ) * param3;
      }

      int x_coordinates =
        game.getActualPlayer()->getWormFromVector(0)->getXposition();
      int y_coordinates =
        game.getActualPlayer()->getWormFromVector(0)->getYposition();

      // calls move to make movement starting at position of current worm
      game.move(y_coordinates, x_coordinates, steps_direction);
    }
    else
    {
      return WRONG_PARAMETER;
    }
    return RETURN_SUCCESS;
  }
}
