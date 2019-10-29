#ifndef WORM_H
#define WORM_H

#include <array>
#include <iostream>
#include <string>

#include "Chest.h"
#include "Field.h"
#include "Random.h"

namespace Sep
{
  class Chest;

  //----------------------------------------------------------------------------
  // Worm Class
  // class to model worms
  //
  class Worm
  {
    private:
      std::string worm_name_;
      int id_;
      int lifepoints_;
      int x_position_;
      int y_position_;

      // first player or second player
      int worms_type_;

      // array (to make it easier?) to determine which weapon is selected
      // gun with index 0 is the default weapon
      std::array<bool, 6> active_weapon_;
      std::string current_weapon_name_;

      // different weapon munitions
      bool gun_;
      int bazooka_;
      int teleport_;
      int blowtorch_;
      int melee_;
      int airstrike_;

      // status
      bool worm_dead_;


    public:
      enum Weapon
      {
        GUN = 0, BAZOOKA = 1, TELEPORT = 2, BLOWTORCH = 3, MELEE = 4,
        AIRSTRIKE = 5
      };

      // -----------------------------------------------------------------------
      // default constructor
      Worm() : worm_name_(WORM), id_(0), lifepoints_(100), x_position_(0),
        y_position_(0)
      {
      }

      //------------------------------------------------------------------------
      // Constructor
      //
      Worm(const std::string &name, int lifePoint, int x_position,
        int y_position, int id);

      //------------------------------------------------------------------------
      // Copy constructor
      //
      Worm(const Worm&);

      //------------------------------------------------------------------------
      // Destructor
      ~Worm();

      //------------------------------------------------------------------------
      // Constants
      //
      static const std::string WORM;

      //------------------------------------------------------------------------
      // Getter Methods
      //
      std::string getName() const;

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setName(std::string name);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getId() const;

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setId(int id);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getXposition() const;

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setXposition(int x_position);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getYposition() const;

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setYposition(int y_position);



      //------------------------------------------------------------------------
      // Getter Methods
      //
      bool getGun() const;

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setGun(bool gun);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getBazooka() const;

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setBazooka(int bazooka);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getTeleport() const;

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setTeleport(int teleport);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getBlowtorch() const;

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setBlowtorch(int blowtorch_);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getMelee() const;

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setMelee(int melee_);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getAirstrike() const;

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setAirstrike(int airstrike_);


      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getWormType() const;


      //------------------------------------------------------------------------
      // Getter Methods
      //
      bool getWormDead() const;

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setWormDead(bool worm_dead);

      //------------------------------------------------------------------------
      // Gravity Method
      //
      std::vector<std::vector<Field::FieldType> > gravity(Game &game,
        Field::FieldType type, int fall_nr,
        std::vector<std::vector<Field::FieldType> > field_map,
        int &return_signal);

      //------------------------------------------------------------------------
      // method to get the symbol characterizing to which team the worm belongs
      // @param player_id id of the player
      // @return ~ if player 1 or * if player 2
      //
      std::string getWormTypeSymbol(int player_id) const;

      //------------------------------------------------------------------------
      // method to get the name of the current weapon
      // @return name of the current weapon
      //
      std::string getNameOfCurrentWeapon() const;

      //------------------------------------------------------------------------
      // method to set the current weapon
      // @param weapon_name name of the weapon
      // @param weapon_number number of the weapon in the hierarchy
      //
      void setCurrentWeapon(std::string weapon_name, Weapon weapon_number);

      //------------------------------------------------------------------------
      // method to get the lifepoints of the worm
      //
      int getLifePoints() const;

      //------------------------------------------------------------------------
      // method to open chest
      // @param chest object
      //
      void openChest(Game &game, Chest *chest);

      //------------------------------------------------------------------------
      // method to set the lifepoints of the worm
      // @param new_lifepoints new lifepoints of the worm
      //
      void setLifePoints(int new_lifepoints);
  };
}

#endif //WORM_H
