#include "State.h"

namespace Sep
{
  const int RETURN_SUCCESS = 0;
  const int WRONG_PARAMETER = 1;

  //----------------------------------------------------------------------------
  State::State(std::string name) : Command(name)
  {
  }

  //----------------------------------------------------------------------------
  State::~State()
  {
  }

  //----------------------------------------------------------------------------
  int State::execute(Game &game, std::vector<std::string> &params)
  {
    if (params.size() != 1)
    {
      return WRONG_PARAMETER;
    }

    Worm *active_worm = game.getActualWorm();
    int number_of_worms_per_player = 3;
    int maximum_worms = 6;

    std::cout << "current player: "
              << game.getActualPlayer()->getName() << std::endl;

    std::cout << "current worm: " << active_worm->getName() << " ("
              << active_worm->getId() << ") " << std::endl << std::endl;

    std::cout << "weapons of current worm:" << std::endl;
    stateWeaponsPrinter(active_worm);

    std::cout << "worms of player 1" << std::endl;
    stateWormsPrinter(game, 0, number_of_worms_per_player);

    std::cout << "worms of player 2" << std::endl;
    stateWormsPrinter(game, number_of_worms_per_player, maximum_worms);

    return RETURN_SUCCESS;
  }

  //----------------------------------------------------------------------------
  void State::stateWeaponsPrinter(const Worm *active_worm) const
  {
    std::cout << "  gun (ammunition: inf)"
      << ((active_worm->getNameOfCurrentWeapon() == "gun") ? " *" : "" )
      << std::endl;

    // print all weapons if ammunition is available
    if (active_worm->getBazooka())
    {
      std::cout << "  bazooka (ammunition: " << active_worm->getBazooka()
        << ")"
        << ((active_worm->getNameOfCurrentWeapon() == "bazooka") ? " *" : "" )
        << std::endl;
    }

    if (active_worm->getTeleport())
    {
      std::cout << "  teleport (ammunition: " << active_worm->getTeleport()
        << ")"
        << ((active_worm->getNameOfCurrentWeapon() == "teleport") ? " *" : "" )
        << std::endl;
    }

    if (active_worm->getBlowtorch())
    {
      std::cout << "  blowtorch (ammunition: " << active_worm->getBlowtorch()
        << ")"
        << ((active_worm->getNameOfCurrentWeapon() == "blowtorch") ? " *" : "" )
        << std::endl;
    }

    if (active_worm->getMelee())
    {
      std::cout << "  melee (ammunition: " << active_worm->getMelee() << ")"
        << ((active_worm->getNameOfCurrentWeapon() == "melee" ) ? " *" : "" )
        << std::endl;
    }

    if (active_worm->getAirstrike())
    {
      std::cout << "  airstrike (ammunition: " << active_worm->getAirstrike()
        << ")"
        << ((active_worm->getNameOfCurrentWeapon() == "airstrike") ? " *" : "" )
        << std::endl << std::endl;
    }
  }

  //----------------------------------------------------------------------------
  void State::stateWormsPrinter(Game &game, const int from, const int to) const
  {
    for (int iterator = from; iterator < to; iterator++)
    {
      Worm *worm = game.getWorm(iterator);

      // print only worms that are alive
      if (worm->getWormDead() == false)
      {
        std::cout << "  " << worm->getName() << " (" << worm->getId() << ") "
                  << worm->getLifePoints() << " HP (" << worm->getYposition()
                  << ", " << worm->getXposition() << ") "
                  << worm->getWormTypeSymbol(worm->getWormType()) << std::endl;
      }
    }
  }
}
