#include "Worm.h"

namespace Sep
{
  //----------------------------------------------------------------------------
  const std::string Worm::WORM = "unknown";

  //----------------------------------------------------------------------------
  Worm::Worm(const std::string &name, int lifePoint, int x_position,
    int y_position, int id)
    : worm_name_(name), id_(id), lifepoints_(lifePoint),
      x_position_(x_position), y_position_(y_position)
  {
    // init the worm_type aka "worm of player"
    int worm_player_check = 4;
    int first_player = 1;
    int second_player = 2;

    if (id < worm_player_check)
    {
      worms_type_ = first_player;
    }
    else
    {
      worms_type_ = second_player;
    }

    // init the munitions of every weapon
    gun_ = true;
    bazooka_ = 1;
    teleport_ = 1;
    blowtorch_ = 1;
    melee_ = 1;
    airstrike_ = 1;

    // set worm to alive
    worm_dead_ = false;
  }

  //----------------------------------------------------------------------------
  Worm::Worm(const Worm &src)
    : worm_name_(src.worm_name_), id_(src.id_), lifepoints_(src.lifepoints_),
      x_position_(src.x_position_), y_position_(src.y_position_)
  {
  }

  //----------------------------------------------------------------------------
  Worm::~Worm()
  {
  }

  //----------------------------------------------------------------------------
  std::string Worm::getName() const
  {
    return worm_name_;
  }

  //----------------------------------------------------------------------------
  void Worm::setName(std::string name)
  {
    worm_name_ = name;
  }

  //----------------------------------------------------------------------------
  int Worm::getId() const
  {
    return id_;
  }

  //----------------------------------------------------------------------------
  void Worm::setId(int id)
  {
    id_ = id;
  }

  //----------------------------------------------------------------------------
  int Worm::getXposition() const
  {
    return x_position_;
  }

  //----------------------------------------------------------------------------
  void Worm::setXposition(int x_position)
  {
    x_position_ = x_position;
  }

  //----------------------------------------------------------------------------
  int Worm::getYposition() const
  {
    return y_position_;
  }

  //----------------------------------------------------------------------------
  void Worm::setYposition(int y_position)
  {
    y_position_ = y_position;
  }

  //----------------------------------------------------------------------------
  bool Worm::getGun() const
  {
    return gun_;
  }

  //----------------------------------------------------------------------------
  void Worm::setGun(bool gun)
  {
    gun_ = gun;
  }

  //----------------------------------------------------------------------------
  int Worm::getBazooka() const
  {
    return bazooka_;
  }

  //----------------------------------------------------------------------------
  void Worm::setBazooka(int bazooka)
  {
    bazooka_ = bazooka;
  }

  //----------------------------------------------------------------------------
  int Worm::getTeleport() const
  {
    return teleport_;
  }

  //----------------------------------------------------------------------------
  void Worm::setTeleport(int teleport)
  {
    teleport_ = teleport;
  }

  //----------------------------------------------------------------------------
  int Worm::getBlowtorch() const
  {
    return blowtorch_;
  }

  //----------------------------------------------------------------------------
  void Worm::setBlowtorch(int blowtorch)
  {
    blowtorch_ = blowtorch;
  }

  //----------------------------------------------------------------------------
  int Worm::getMelee() const
  {
    return melee_;
  }

  //----------------------------------------------------------------------------
  void Worm::setMelee(int melee)
  {
    melee_ = melee;
  }

  //----------------------------------------------------------------------------
  int Worm::getAirstrike() const
  {
    return airstrike_;
  }

  //----------------------------------------------------------------------------
  void Worm::setAirstrike(int airstrike)
  {
    airstrike_ = airstrike;
  }

  //----------------------------------------------------------------------------
  int Worm::getWormType() const
  {
    return worms_type_;
  }

  //----------------------------------------------------------------------------
  bool Worm::getWormDead() const
  {
    return worm_dead_;
  }

  //----------------------------------------------------------------------------
  void Worm::setWormDead(bool worm_dead)
  {
    worm_dead_ = worm_dead;
  }

  //----------------------------------------------------------------------------
  std::vector<std::vector<Field::FieldType> > Worm::gravity(Game &game,
    Field::FieldType type, int fall_nr,
    std::vector<std::vector<Field::FieldType> > field_map, int &return_signal)
  {
    // if worm is dead ignore it
    if (worm_dead_)
    {
      return field_map;
    }

    int row = y_position_;
    int col = x_position_;

    // fall_nr : -1 left +1 right 0 just down
    int fall_counter = 0;
    int temp_row = row;
    while (field_map[row + 1][col + fall_nr] == Field::AIR)
    {
      fall_counter++;
      row++;

      if (row + 1 == static_cast<int> (field_map.size()))
      {
        std::cout << getName() << " (" << getId() << ") fell out of the map."
          << std::endl;
        field_map[temp_row][col] = Field::AIR;
        setWormDead(true);
        return field_map;
      }
    }

    // if falls on water
    if (field_map[row + 1][col + fall_nr] == Field::WATER)
    {
      std::cout << getName() << " (" << getId() << ") drowned." << std::endl;
      field_map[temp_row][col] = Field::AIR;
      worm_dead_ = true;
      return_signal = 1;
      return field_map;
    }

    int allowed_fall_height = 1;
    int damage_nr = 10;
    int fall_damage = 0;
    if (fall_counter > allowed_fall_height)
    {
      fall_damage = ( fall_counter - allowed_fall_height ) * damage_nr;
      lifepoints_ = lifepoints_ - fall_damage;
      if (lifepoints_ <= 0)
      {
        // in case it goes negative value
        lifepoints_ = 0;
        worm_dead_  = true;
        std::cout << worm_name_ << " (" << id_ << ") fell into his death."
                  << std::endl;
        field_map[temp_row][col] = Field::AIR;

        return_signal = 1;
        return field_map;
      }
      else
      {
        std::cout << worm_name_ << " (" << id_ << ") took " << fall_damage
                  << "hp fall damage" << std::endl;
      }
    }

    // if falls on earth or on worm
    if (field_map[row + 1][col + fall_nr] != Field::WATER)
    {
      field_map[temp_row][col]      = Field::AIR;
      field_map[row][col + fall_nr] = type;

      // set x position
      x_position_ = col + fall_nr;
      // set y position
      y_position_ = row;
    }

    // if falls on chest
    if (field_map[row + 1][col + fall_nr] == Field::CHEST)
    {
      Chest *chest = game.searchChest(row + 1, col + fall_nr);
      if (chest != nullptr)
      {
        openChest(game, chest);
      }
      field_map[row + 1][col + fall_nr] = Field::AIR;
      field_map[temp_row][col]          = Field::AIR;
      field_map[row][col + fall_nr]     = type;

      // set x position
      x_position_ = col + fall_nr;
      // set y position
      y_position_ = row;
    }
    return field_map;
  }

  //----------------------------------------------------------------------------
  std::string Worm::getWormTypeSymbol(int player_id) const
  {
    if (player_id == 1)
    {
      return "~";
    }
    else
    {
      return "*";
    }
  }

  //----------------------------------------------------------------------------
  std::string Worm::getNameOfCurrentWeapon() const
  {
    return current_weapon_name_;
  }

  //----------------------------------------------------------------------------
  void Worm::setCurrentWeapon(std::string weapon_name, Weapon weapon_number)
  {
    current_weapon_name_ = weapon_name;

    for (unsigned iterator = 0;iterator < active_weapon_.size();iterator++)
    {
      active_weapon_[iterator] = false;
    }
    active_weapon_[weapon_number] = true;
  }

  //----------------------------------------------------------------------------
  int Worm::getLifePoints() const
  {
    return lifepoints_;
  }

  //----------------------------------------------------------------------------
  void Worm::openChest(Game &game, Chest *chest)
  {
    if (chest->getToDelete() == true)
    {
      return;
    }

    switch (chest->getChestContent())
    {
      case BAZOOKA:
        bazooka_++;
        std::cout << worm_name_ << " (" << id_ << ") picked up 1 of bazooka"
                  << std::endl;
        break;
      case TELEPORT:
        teleport_++;
        std::cout << worm_name_ << " (" << id_ << ") picked up 1 of teleport"
                  << std::endl;
        break;
      case BLOWTORCH:
        blowtorch_++;
        std::cout << worm_name_ << " (" << id_ << ") picked up 1 of blowtorch"
                  << std::endl;
        break;
      case MELEE:
        melee_++;
        std::cout << worm_name_ << " (" << id_ << ") picked up 1 of melee"
                  << std::endl;
        break;
      case AIRSTRIKE:
        airstrike_++;
        std::cout << worm_name_ << " (" << id_ << ") picked up 1 of airstrike"
                  << std::endl;
        break;
    }
    x_position_ = chest->getChestCol();
    y_position_ = chest->getChestRow();
    chest->destroyChest(game);

    // check if there are still other chests above
    int current_row = chest->getChestRow();
    int current_col = chest->getChestCol();

    Chest *other_chests = game.searchChest(current_row - 1, current_col);
    if (other_chests != nullptr)
    {
      openChest(game, other_chests);
    }
  }

  //----------------------------------------------------------------------------
  void Worm::setLifePoints(int new_lifepoints)
  {
    lifepoints_ = new_lifepoints;
  }
}
