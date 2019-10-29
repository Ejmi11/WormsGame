#include "Chest.h"

namespace Sep
{
  // ---------------------------------------------------------------------------
  Chest::Chest(int col, int weapon_number)
    : chest_col_(col), chest_row_(0), weapon_number_(weapon_number + 1),
      to_delete_(false)
  {
  }

  // ---------------------------------------------------------------------------
  Chest::~Chest()
  {
  }

  // ---------------------------------------------------------------------------
  void Chest::addChest(Game &game)
  {
    // check if there is place in first row
    if (game.getFieldType(chest_row_, chest_col_) != Field::AIR)
    {
      to_delete_ = true;
      return;
    }
    makeChestFall(game);
  }

  // ---------------------------------------------------------------------------
  int Chest::getChestContent() const
  {
    return weapon_number_;
  }

  // ---------------------------------------------------------------------------
  int Chest::getChestRow() const
  {
    return chest_row_;
  }

  // ---------------------------------------------------------------------------
  int Chest::getChestCol() const
  {
    return chest_col_;
  }

  // ---------------------------------------------------------------------------
  void Chest::destroyChest(Game &game)
  {
    if (game.getFieldType(chest_row_, chest_col_) == Field::CHEST)
    {
      game.setFieldType(Field::AIR, chest_row_, chest_col_);
    }
    to_delete_ = true;
  }

  // ---------------------------------------------------------------------------
  void Chest::makeChestFall(Game &game)
  {
    if (to_delete_ == true)
    {
      return;
    }

    int temp_row  = chest_row_ + 1;
    int saved_row = chest_row_;
    int saved_col = chest_col_;

    // apply gravity
    while (game.getFieldType(temp_row, chest_col_) == Field::AIR)
    {
      chest_row_++;
      temp_row++;
    }
    chest_row_++;

    // check ground
    Field::FieldType type = game.getFieldType(chest_row_, chest_col_);
    if (type == Field::EARTH)
    {
      chest_row_--;
      game.setFieldType(Field::AIR, saved_row, saved_col);
      game.setFieldType(Field::CHEST, chest_row_, chest_col_);
    }
    else if (type == Field::WORM || type == Field::WORM2)
    {
      Worm *worm = game.searchWorm(chest_row_, chest_col_);
      worm->openChest(game, this);
      game.setFieldType(Field::AIR, saved_row, saved_col);

      if (game.getFieldType(chest_row_, chest_col_) == Field::WORM)
      {
        game.setFieldType(Field::WORM, chest_row_, chest_col_);
      }
      else
      {
        game.setFieldType(Field::WORM2, chest_row_, chest_col_);
      }
    }
    else if (type == Field::CHEST)
    {
      chest_row_--;
      game.setFieldType(Field::AIR, saved_row, saved_col);
      game.setFieldType(Field::CHEST, chest_row_, chest_col_);
    }
    else
    {
      to_delete_ = true; // chest falls into the water
      game.setFieldType(Field::AIR, saved_row, saved_col);
    }
  }

  // ---------------------------------------------------------------------------
  bool Chest::getToDelete() const
  {
    return to_delete_;
  }

  // ---------------------------------------------------------------------------
  void Chest::setToDelete(bool to_delete)
  {
    to_delete_ = to_delete;
  }
}
