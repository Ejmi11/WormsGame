#include "Map.h"

namespace Sep
{
  const int RETURN_SUCCESS = 0;
  const int WRONG_PARAMETER = 1;

  //----------------------------------------------------------------------------
  Map::Map(std::string name) : Command(name)
  {
  }

  //----------------------------------------------------------------------------
  Map::~Map()
  {
  }

  //----------------------------------------------------------------------------
  int Map::execute(Game& game, std::vector<std::string>& params)
  {
    if (params.size() == 1)
    {
      game.printMap();
    }
    else
    {
      return WRONG_PARAMETER;
    }
    return RETURN_SUCCESS;
  }
}
