#include "Help.h"

namespace Sep
{
  const int RETURN_SUCCESS  = 0;
  const int WRONG_PARAMETER = 1;

  //----------------------------------------------------------------------------
  Help::Help(std::string name) : Command(name)
  {
  }

  //----------------------------------------------------------------------------
  Help::~Help()
  {
  }

  //----------------------------------------------------------------------------
  int Help::execute(Game &game, std::vector<std::string> &params)
  {
    if (params.size() == 1)
    {
      std::cout << "Available Commands:\n"
        "  move [left/right] [0..3]\n"
        "    Move your worm.\n"
        "  choose [weapon]\n"
        "    Select your weapon.\n"
        "  action [l/r/d/u/ld/rd/lu/ru]\n"
        "    Shoots current weapon.\n"
        "    Activates blowtorch.\n"
        "  action\n"
        "    Activates melee attack.\n"
        "  action [row] [col]\n"
        "    Teleports to coordinate.\n"
        "  action [col]\n"
        "    Airstrike.\n"
        "  action idle\n"
        "    Do nothing.\n"
        "  quit\n"
        "    End the game.\n"
        "  help\n"
        "    Display this help.\n"
        "  state\n"
        "    Print the current game state." << std::endl;
    }
    else
    {
      return WRONG_PARAMETER;
    }
    return RETURN_SUCCESS;
  }
}
