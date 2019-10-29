#include "Quit.h"

namespace Sep
{
  const int RETURN_SUCCESS = 0;
  const int WRONG_PARAMETER = 1;

  //----------------------------------------------------------------------------
  Quit::Quit(std::string name) : Command(name)
  {
  }

  //----------------------------------------------------------------------------
  Quit::~Quit()
  {
  }

  //----------------------------------------------------------------------------
  int Quit::execute(Game &game, std::vector<std::string> &params)
  {
    if (params.size() == 1)
    {
      game.setStateRunning(false);
    }
    else
    {
      return WRONG_PARAMETER;
    }
    return RETURN_SUCCESS;
  }
}
