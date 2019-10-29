#include "Choose.h"

namespace Sep
{
  //----------------------------------------------------------------------------
  Choose::Choose(std::string name) : Command(name)
  {
  }

  //----------------------------------------------------------------------------
  Choose::~Choose()
  {
  }

  //----------------------------------------------------------------------------
  int Choose::execute(Game &game, std::vector<std::string> &params)
  {
    std::string weapon_name = params[1];
    Worm *worm = game.getActualWorm();
    int ammunition = 0;

    // sets current weapon and gets according ammunition
    if (params.size() == 2)
    {
      if (weapon_name == "gun")
      {
        worm->setCurrentWeapon("gun", Worm::GUN);
        ammunition = worm->getGun();
      }
      else if (weapon_name == "bazooka")
      {
        worm->setCurrentWeapon("bazooka", Worm::BAZOOKA);
        ammunition = worm->getBazooka();
      }
      else if (weapon_name == "teleport")
      {
        worm->setCurrentWeapon("teleport", Worm::TELEPORT);
        ammunition = worm->getTeleport();
      }
      else if (weapon_name == "blowtorch")
      {
        worm->setCurrentWeapon("blowtorch", Worm::BLOWTORCH);
        ammunition = worm->getBlowtorch();
      }
      else if (weapon_name == "melee")
      {
        worm->setCurrentWeapon("melee", Worm::MELEE);
        ammunition = worm->getMelee();
      }
      else if (weapon_name == "airstrike")
      {
        worm->setCurrentWeapon("airstrike", Worm::AIRSTRIKE);
        ammunition = worm->getAirstrike();
      }
      else
      {
        std::cout << "[ERROR] invalid parameter!" << std::endl;
        return 0;
      }

      if (ammunition == 0)
      {
        return 2;
      }

      std::cout << "Chose weapon " << worm->getNameOfCurrentWeapon()
                << " Ammunition: ";

      // prints infinity for gun else ammunition of weapon
      if (worm->getNameOfCurrentWeapon() == "gun")
      {
        std::cout << "inf" << std::endl;
      }
      else
      {
        std::cout << ammunition << std::endl;
      }
    }
    else
    {
      std::cout << "[ERROR] wrong parameter count!" << std::endl;
    }
    return 0;
  }
}
