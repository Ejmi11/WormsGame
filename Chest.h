#ifndef CHEST_H
#define CHEST_H

#include "Game.h"

namespace Sep
{
  class Game;

  //----------------------------------------------------------------------------
  // Chest Class
  // class to model the chests
  //
  class Chest
  {
    public:
      //------------------------------------------------------------------------
      // Constructs a new Chest instance
      // @param game object
      // @param weapon_number of the weapon
      // @param col(umn) where the weapon should spawn in the first row
      //
      Chest(int col, int weapon_number);

      //------------------------------------------------------------------------
      // Destructor
      //
      ~Chest();

      //------------------------------------------------------------------------
      // Deleted copy constructor
      //
      Chest(const Chest &original) = delete;

      //------------------------------------------------------------------------
      // Deleted assignment operator
      //
      Chest& operator=(const Chest &original) = delete;

      //------------------------------------------------------------------------
      // method to add chest and apply gravity afterwards
      // @param game object
      //
      void addChest(Game &game);

      //------------------------------------------------------------------------
      // method to get the content of the chest
      // @return the number of the weapon
      //
      int getChestContent() const;

      //------------------------------------------------------------------------
      // method to get the row
      // @return row of the chest
      //
      int getChestRow() const;

      //------------------------------------------------------------------------
      // method to get the col
      // @return col of the chest
      //
      int getChestCol() const;

      //------------------------------------------------------------------------
      // method to destroy chest
      // @param game object
      //
      void destroyChest(Game &game);

      //------------------------------------------------------------------------
      // method to make chest fall down according to gravity rules and check
      // ground
      // @param game object
      //
      void makeChestFall(Game &game);

      //------------------------------------------------------------------------
      // method to get the to_delete_ attribute
      // @return to_delete_
      //
      bool getToDelete() const;

      //------------------------------------------------------------------------
      // method to set the to_delete_ attribute
      // @param to_delete value to set
      //
      void setToDelete(bool to_delete);

    private:
      int chest_col_;
      int chest_row_;
      int weapon_number_;
      bool to_delete_;
  };
}

#endif // ifndef CHEST_H
