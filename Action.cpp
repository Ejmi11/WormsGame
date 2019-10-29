#include "Action.h"

namespace Sep
{
  const int RETURN_SUCCESS    = 0;
  const int WRONG_PARAMETER   = 1;
  const int INVALID_TARGET    = 3;
  const int INVALID_PARAMETER = 4;

  //----------------------------------------------------------------------------
  Action::Action(std::string name) : Command(name)
  {
  }

  //----------------------------------------------------------------------------
  Action::~Action()
  {
  }

  //----------------------------------------------------------------------------
  int Action::execute(Game &game, std::vector<std::string> &params)
  {
    Worm *worm = game.getActualWorm();
    std::string param;

    x_coordinates =
      game.getActualPlayer()->getWormFromVector(0)->getXposition();
    y_coordinates =
      game.getActualPlayer()->getWormFromVector(0)->getYposition();
    std::string weapon = worm->getNameOfCurrentWeapon();

    // returns without action for current worm
    if (params.size() == 2)
    {
      if (params[1] == "idle")
      {
        return RETURN_SUCCESS;
      }
    }

    // attack if weapon is gun first field(not air) next to current worm
    if (weapon == "gun")
    {
      if (params.size() != 2)
      {
        return WRONG_PARAMETER;
      }

      if (correctParameter(params[1]) == false)
      {
        return INVALID_PARAMETER;
      }

      param                = params[1];

      //returns value for the direction of the attack
      attack_direction_row = attackDirectionRow(param);
      attack_direction_col = attackDirectionCol(param);

      //calls the method to attack with gun
      action_done          = actionGun(game,
                                       attack_direction_row,
                                       attack_direction_col,
                                       y_coordinates,
                                       x_coordinates);
    }

    // attack if weapon is bazooka first field (not air) next to current worm
    // and fields l,r,d,u around target field
    else if (weapon == "bazooka")
    {
      if (params.size() != 2)
      {
        return WRONG_PARAMETER;
      }

      if (correctParameter(params[1]) == false)
      {
        return INVALID_PARAMETER;
      }

      param                = params[1];

      //returns value for the direction of the attack
      attack_direction_row = attackDirectionRow(param);
      attack_direction_col = attackDirectionCol(param);

      //calls the method to attack with bazooka
      action_done          = actionBazooka(game,
                                           attack_direction_row,
                                           attack_direction_col,
                                           y_coordinates,
                                           x_coordinates);

      //decrements ammunition by one if shot was fired
      if (action_done == 1)
      {
        ammunition = worm->getBazooka();
        worm->setBazooka(--ammunition);
      }
    }

    // attack if weapon is blowtorch 5 fields next to current worm
    else if (weapon == "blowtorch")
    {
      if (params.size() != 2)
      {
        return WRONG_PARAMETER;
      }

      if (correctParameter(params[1]) == false)
      {
        return INVALID_PARAMETER;
      }

      param                = params[1];

      //returns value for the direction of the attack
      attack_direction_row = attackDirectionRow(param);
      attack_direction_col = attackDirectionCol(param);

      //calls the method to attack with blowtorch
      action_done          = actionBlowtorch(game,
                                             attack_direction_row,
                                             attack_direction_col,
                                             y_coordinates,
                                             x_coordinates);

      //decrements ammunition by one if shot was fired
      if (action_done == 1)
      {
        ammunition = worm->getBlowtorch();
        worm->setBlowtorch(--ammunition);
      }
    }

    // action [row] [col] -> teleport
    else if (weapon == "teleport")
    {
      if (params.size() != 3)
      {
        return WRONG_PARAMETER;
      }

      param = params[1];

      //if parameter is no digit Error
      for (int counter = 0; param[counter] != '\0'; counter++)
      {
        if (std::isdigit(param[counter]) != 1)
        {
          return INVALID_PARAMETER;
        }
      }

      param = params[2];

      //if parameter is no digit Error
      for (int counter = 0; param[counter] != '\0'; counter++)
      {
        if (std::isdigit(param[counter]) != 1)
        {
          return INVALID_PARAMETER;
        }
      }

      //sets parameter to digit
      int teleport_row = std::stoi(params[1]);
      int teleport_col = std::stoi(params[2]);

      // Error if position is out of field
      if (teleport_row > game.getBoardHeight() || teleport_row < 0
          || teleport_col > game.getBoardWidth() || teleport_col < 0)
      {
        return INVALID_PARAMETER;
      }

      //calls the method to teleport
      action_done = actionTeleport(game, worm, teleport_row, teleport_col);

      //decrements ammunition by one if teleported
      if (action_done == 1)
      {
        ammunition = worm->getTeleport();
        worm->setTeleport(--ammunition);
      }
    }

    // action [col] with airstrike
    else if (weapon == "airstrike")
    {
      if (params.size() != 2)
      {
        return WRONG_PARAMETER;
      }

      param = params[1];

      //if parameter is no digit Error
      for (int counter = 0; param[counter] != '\0'; counter++)
      {
        if (std::isdigit(param[counter]) != 1)
        {
          return INVALID_PARAMETER;
        }
      }

      //sets parameter to digit
      int airstrike_col = std::stoi(params[1]);

      //calls the method to attack with airstrike
      action_done = actionAirstrike(game, airstrike_col);

      //decrements ammunition by one if shot was fired
      if (action_done == 1)
      {
        ammunition = worm->getAirstrike();
        worm->setAirstrike(--ammunition);
      }
    }

    // action attacks with melee all fields around current worm
    else if (weapon == "melee")
    {
      if (params.size() != 1)
      {
        return WRONG_PARAMETER;
      }

      //calls the method to attack with melee
      action_done = actionMelee(game, y_coordinates, x_coordinates);

      //decrements ammunition by one if shot was fired
      if (action_done == 1)
      {
        ammunition = worm->getMelee();
        worm->setMelee(--ammunition);
      }
    }
    return RETURN_SUCCESS;
  }

  //----------------------------------------------------------------------------
  int Action::actionGun(Game &game, int attack_direction_row,
                        int attack_direction_col, int gun_row, int gun_col)
  {
    damage = 25;
    int direction_row = gun_row + attack_direction_row;
    int direction_col = gun_col + attack_direction_col;

    while (1)
    {
      // if end of x end of y
      if (direction_row >= game.getBoardHeight() || direction_row < 0
          || direction_col >= game.getBoardWidth() || direction_col < 0)
      {
        std::cout << "Shot missed..." << std::endl;
        action_done = 0;
        break;
      }

      // sets field to AIR if field is Earth
      if (game.getFieldType(direction_row, direction_col) == Field::EARTH)
      {
        std::cout << "Shot hit Earth at position ("
                  << direction_row << ", " << direction_col << ")"
                  << std::endl;

        game.setFieldType(Field::AIR, direction_row, direction_col);

        //checks gravity above current field
        gravityAbove(game, direction_row - 1, direction_col);

        action_done = 1;
        break;
      }

      // sets field to AIR if field is chest
      else if (game.getFieldType(direction_row, direction_col)
               == Field::CHEST)
      {
        std::cout << "Shot hit Chest at position ("
                  << direction_row << ", " << direction_col << ")"
                  << std::endl;

        //deletes chest if it got hit
        Chest *chest = game.searchChest(direction_row, direction_col);
        chest->setToDelete(true);
        game.setFieldType(Field::AIR, direction_row, direction_col);

        //checks gravity above current field
        gravityAbove(game, direction_row - 1, direction_col);

        action_done = 1;
        break;
      }

      //if field type is worm
      else if (game.getFieldType(direction_row, direction_col) == Field::WORM
               || game.getFieldType(direction_row,
                                    direction_col) == Field::WORM2)
      {
        Worm *attacked_worm =
          game.searchWorm(direction_row, direction_col);

        std::cout << "Shot hit Worm at position ("
                  << direction_row << ", " << direction_col << ")"
                  << std::endl;

        //resets lifepoints or sends death message
        attackDamage(attacked_worm, damage);

        if (attacked_worm->getWormDead() == true)
        {
          game.setFieldType(Field::AIR, direction_row, direction_col);

          //checks gravity above current field
          gravityAbove(game, direction_row - 1, direction_col);
        }
        action_done = 1;
        break;
      }

      // sets direction to the next position in map
      direction_row = direction_row + attack_direction_row;
      direction_col = direction_col + attack_direction_col;
    }
    return action_done;
  }

  //----------------------------------------------------------------------------
  int Action::actionBazooka(Game &game, int attack_direction_row,
                            int attack_direction_col, int bazooka_row,
                            int bazooka_col)
  {
    damage = 40;
    int direction_row = bazooka_row + attack_direction_row;
    int direction_col = bazooka_col + attack_direction_col;

    while (1)
    {
      //if end of x end of y
      if (direction_row >= game.getBoardHeight() || direction_row < 0
          || direction_col >= game.getBoardWidth() || direction_col < 0)
      {
        std::cout << "Shot missed..." << std::endl;
        action_done = 0;
        break;
      }

      // sets field to AIR if field is Earth
      if (game.getFieldType(direction_row, direction_col) == Field::EARTH)
      {
        std::cout << "Shot hit Earth at position ("
                  << direction_row << ", " << direction_col << ")"
                  << std::endl;

        game.setFieldType(Field::AIR, direction_row, direction_col);

        //checks gravity above current field
        gravityAbove(game, direction_row - 1, direction_col);
        bazookaCrossAttacking(game, direction_row, direction_col, damage);

        action_done = 1;
        break;
      }

      // sets field to AIR if field is chest
      else if (game.getFieldType(direction_row, direction_col)
               == Field::CHEST)
      {
        std::cout << "Shot hit Chest at position ("
                  << direction_row << ", " << direction_col << ")"
                  << std::endl;

        //deletes chest if it got hit
        Chest *chest = game.searchChest(direction_row, direction_col);
        chest->setToDelete(true);
        game.setFieldType(Field::AIR, direction_row, direction_col);

        //checks gravity above current field
        gravityAbove(game, direction_row - 1, direction_col);

        //calls second method for cross of attacking
        bazookaCrossAttacking(game, direction_row, direction_col, damage);

        action_done = 1;
        break;
      }

      //if attacked field is worm
      else if (game.getFieldType(direction_row, direction_col) == Field::WORM
               || game.getFieldType(direction_row,
                                    direction_col) == Field::WORM2)
      {
        Worm *attacked_worm = game.searchWorm(direction_row, direction_col);
        std::cout << "Shot hit Worm at position ("
                  << direction_row << ", " << direction_col << ")"
                  << std::endl;

        //resets lifepoints or sends death message
        attackDamage(attacked_worm, damage);

        if (attacked_worm->getWormDead() == true)
        {
          game.setFieldType(Field::AIR, direction_row, direction_col);

          //checks gravity above dead worm
          gravityAbove(game, direction_row - 1, direction_col);
        }

        //calls second method for cross of attacking
        bazookaCrossAttacking(game, direction_row, direction_col, damage);

        action_done = 1;
        break;
      }

      //if shot hit water
      else if (game.getFieldType(direction_row, direction_col)
               == Field::WATER)
      {
        //calls second method for cross of attacking
        bazookaCrossAttacking(game, direction_row, direction_col, damage);
        action_done = 1;
        break;
      }

      // sets direction to the next position in map
      direction_row = direction_row + attack_direction_row;
      direction_col = direction_col + attack_direction_col;
    }
    return action_done;
  }

  //----------------------------------------------------------------------------
  int Action::actionBlowtorch(Game &game, int attack_direction_row,
                              int attack_direction_col, int blowtorch_row,
                              int blowtorch_col)
  {
    damage = 35;
    int counter       = 0;
    int direction_row = blowtorch_row + attack_direction_row;
    int direction_col = blowtorch_col + attack_direction_col;

    for (int count = 0; count < 5; count++)
    {
      // sets field to AIR if field is Earth
      if (game.getFieldType(direction_row, direction_col) == Field::EARTH)
      {
        std::cout << "Torch hit Earth at position ("
                  << direction_row << ", " << direction_col << ")"
                  << std::endl;
        game.setFieldType(Field::AIR, direction_row, direction_col);

        action_done = 1;
      }

      // sets field to AIR if field is chest
      else if (game.getFieldType(direction_row, direction_col)
               == Field::CHEST)
      {
        std::cout << "Torch hit Chest at position ("
                  << direction_row << ", " << direction_col << ")"
                  << std::endl;

        //deletes chest if it got hit
        Chest *chest = game.searchChest(direction_row, direction_col);
        chest->setToDelete(true);
        game.setFieldType(Field::AIR, direction_row, direction_col);

        action_done = 1;
      }

      //if torch hits worm
      else if (game.getFieldType(direction_row, direction_col) == Field::WORM
               || game.getFieldType(direction_row,
                                    direction_col) == Field::WORM2)
      {
        Worm *attacked_worm = game.searchWorm(direction_row, direction_col);
        std::cout << "Torch hit Worm at position ("
                  << direction_row << ", " << direction_col << ")"
                  << std::endl;

        //resets lifepoints or sends death message
        attackDamage(attacked_worm, damage);

        //if attacked worm died
        if (attacked_worm->getWormDead() == true)
        {
          game.setFieldType(Field::AIR, direction_row, direction_col);

          //checks gravity above dead worm
          gravityAbove(game, direction_row - 1, direction_col);
        }
        action_done = 1;
      }

      //it out of map
      if (direction_row + attack_direction_row == game.getBoardHeight())
      {
        break;
      }

      // sets direction to the next position in map
      direction_row = direction_row + attack_direction_row;
      direction_col = direction_col + attack_direction_col;
    }

    direction_row = blowtorch_row + attack_direction_row;
    direction_col = blowtorch_col + attack_direction_col;

    //checks gravity above all attacked fields
    for (int count = 0; count < counter; count++)
    {
      gravityAbove(game, direction_row - 1, direction_col);
      direction_row = direction_row + attack_direction_row;
      direction_col = direction_col + attack_direction_col;
    }

    return action_done;
  }

  //----------------------------------------------------------------------------
  int Action::actionTeleport(Game &game, Worm *worm, int teleport_row,
                             int teleport_col)
  {
    // Error if Field is not of type air or chest
    if (game.getFieldType(teleport_row, teleport_col) != Field::AIR
        && game.getFieldType(teleport_row, teleport_col) != Field::CHEST)
    {
      return INVALID_TARGET;
    }

    // sets actual worm to new coordinates
    game.getActualPlayer()->getWormFromVector(0)->setXposition(teleport_col);
    game.getActualPlayer()->getWormFromVector(0)->setYposition(teleport_row);

    // field for teleport is of type chest
    if (game.getFieldType(teleport_row, teleport_col) == Field::CHEST)
    {
      //collects chest
      Sep::Chest *chest = game.searchChest(teleport_row, teleport_col);
      game.getActualPlayer()->getWormFromVector(0)->openChest(game, chest);

      int player1 = 1;
      if (game.getActualPlayer()->getName() == player1)
      {
        game.setFieldType(Field::WORM, teleport_row, teleport_col);
      }
      else
      {
        game.setFieldType(Field::WORM2, teleport_row, teleport_col);
      }

      // gravity for previous position
      game.setFieldType(Field::AIR, y_coordinates, x_coordinates);
      gravityAbove(game, y_coordinates - 1, x_coordinates);

      action_done = 1;
    }

    // field for teleport is of type air
    else if (game.getFieldType(teleport_row, teleport_col) == Field::AIR)
    {
      //checks if worm can fall after teleport to new position
      checkGravityAfterTeleport(game, worm, teleport_row, teleport_col);

      // gravity for previous position
      game.setFieldType(Field::AIR, y_coordinates, x_coordinates);
      gravityAbove(game, y_coordinates - 1, x_coordinates);

      action_done = 1;
    }
    return action_done;
  }

  //----------------------------------------------------------------------------
  int Action::actionAirstrike(Game &game, int airstrike_col)
  {
    damage = 20;


    if (airstrike_col >= game.getBoardWidth() || direction_col < 0)
    {
      std::cout << "Shot missed..." << std::endl;
      action_done = 0;
    }
    else
    {
      for (int counter = 0; counter <= game.getBoardHeight(); counter++)
      {
        if (counter == game.getBoardHeight())
        {
          std::cout << "Shot missed..." << std::endl;
          action_done = 0;
          break;
        }

        // sets field to AIR if field is Earth
        if (game.getFieldType(counter, airstrike_col) == Field::EARTH)
        {
          std::cout << "Shot hit Earth at position ("
                    << counter << ", " << airstrike_col << ")"
                    << std::endl;
          game.setFieldType(Field::AIR, counter, airstrike_col);

          action_done = 1;
          break;
        }
        // sets field to AIR if field is chest
        else if (game.getFieldType(counter, airstrike_col) == Field::CHEST)
        {
          std::cout << "Shot hit Chest at position ("
                    << counter << ", " << airstrike_col << ")"
                    << std::endl;

          //deletes chest if it got hit
          Chest *chest = game.searchChest(direction_row, direction_col);
          chest->setToDelete(true);
          game.setFieldType(Field::AIR, counter, airstrike_col);

          action_done = 1;
          break;
        }

        //if airstrike hits worm
        else if (game.getFieldType(counter, airstrike_col) == Field::WORM
                 || game.getFieldType(counter, airstrike_col) == Field::WORM2)
        {
          Worm *attacked_worm = game.searchWorm(counter, airstrike_col);
          std::cout << "Shot hit Worm at position ("
                    << counter << ", " << airstrike_col << ")"
                    << std::endl;

          //resets lifepoints or sends death message
          attackDamage(attacked_worm, damage);

          //if worm died
          if (attacked_worm->getWormDead() == true)
          {
            game.setFieldType(Field::AIR, counter, airstrike_col);
          }

          action_done = 1;
          break;
        }
      }
    }
    return action_done;
  }

  //----------------------------------------------------------------------------
  int Action::actionMelee(Game &game, int melee_row, int melee_col)
  {
    //checks for every target field of melee if worm is there and if so attacks
    if (game.getFieldType(( melee_row - 1 ), melee_col) == Field::WORM
        || game.getFieldType(( melee_row - 1 ), melee_col) == Field::WORM2)
    {
      meleeAttacking(game, melee_row - 1, melee_col);
      action_done = 1;
    }

    if (game.getFieldType(( melee_row - 1 ), ( melee_col + 1 )) == Field::WORM
        || game.getFieldType(melee_row - 1, melee_col + 1) == Field::WORM2)
    {
      meleeAttacking(game, melee_row - 1, melee_col + 1);
      action_done = 1;
    }

    if (game.getFieldType(melee_row, ( melee_col + 1 )) == Field::WORM
        || game.getFieldType(melee_row, ( melee_col + 1 )) == Field::WORM2)
    {
      meleeAttacking(game, melee_row, melee_col + 1);
      action_done = 1;
    }

    if (game.getFieldType(( melee_row + 1 ), ( melee_col + 1 )) == Field::WORM
        || game.getFieldType(( melee_row + 1 ),
                             ( melee_col + 1 )) == Field::WORM2)
    {
      meleeAttacking(game, melee_row + 1, melee_col + 1);
      action_done = 1;
    }

    if (game.getFieldType(( melee_row + 1 ), melee_col) == Field::WORM
        || game.getFieldType(( melee_row + 1 ), melee_col) == Field::WORM2)
    {
      meleeAttacking(game, melee_row + 1, melee_col);
      action_done = 1;
    }

    if (game.getFieldType(( melee_row + 1 ), ( melee_col - 1 )) == Field::WORM
        || game.getFieldType(( melee_row + 1 ),
                             ( melee_col - 1 )) == Field::WORM2)
    {
      meleeAttacking(game, melee_row + 1, melee_col - 1);
      action_done = 1;
    }

    if (game.getFieldType(melee_row, ( melee_col - 1 )) == Field::WORM
        || game.getFieldType(melee_row, ( melee_col - 1 )) == Field::WORM2)
    {
      meleeAttacking(game, melee_row, melee_col - 1);
      action_done = 1;
    }

    if (game.getFieldType(( melee_row - 1 ), ( melee_col - 1 )) == Field::WORM
        || game.getFieldType(( melee_row - 1 ),
                             ( melee_col - 1 )) == Field::WORM2)
    {
      meleeAttacking(game, melee_row - 1, melee_col - 1);
      action_done = 1;
    }
    return action_done;
  }

  //----------------------------------------------------------------------------
  void Action::checkGravityAfterTeleport(Game &game, Worm *worm,
                                         int row, int col)
  {
    int temp_row = row;

    while (game.getFieldType(( row + 1 ), col) == Field::AIR)
    {
      row++;
    }

    //if falls on water
    if (game.getFieldType(( row + 1 ), col) == Field::WATER)
    {
      std::cout << worm->getName() << " (" << worm->getId()
                << ") drowned."
                << std::endl;
      worm_dead = true;
      worm->setWormDead(worm_dead);
      game.setFieldType(Field::AIR, temp_row, col);
    }

    //if falls on chest
    else if (game.getFieldType(( row + 1 ), col) == Field::CHEST)
    {
      //collects chest
      Sep::Chest *chest = game.searchChest(row + 1, col);
      game.getActualPlayer()->getWormFromVector(0)->openChest(game, chest);
      row++;
      game.setFieldType(Field::AIR, temp_row, col);

      int player1 = 1;
      if (game.getActualPlayer()->getName() == player1)
      {
        game.setFieldType(Field::WORM, row, col);
      }
      else
      {
        game.setFieldType(Field::WORM2, row, col);
      }

      worm->setXposition(col);
      worm->setYposition(row);
      fallDamage(worm, ( temp_row - row ));
    }

    // if falls on earth or on worm
    else
    {
      game.setFieldType(Field::AIR, temp_row, col);
      int player1 = 1;
      if (game.getActualPlayer()->getName() == player1)
      {
        game.setFieldType(Field::WORM, row, col);
      }
      else
      {
        game.setFieldType(Field::WORM2, row, col);
      }

      worm->setXposition(col);
      worm->setYposition(row);
      fallDamage(worm, ( temp_row - row ));
    }
  }

  //----------------------------------------------------------------------------
  void Action::attackDamage(Worm *attacked_worm, int damage)
  {
    old_lifepoints = attacked_worm->getLifePoints();
    new_lifepoints = old_lifepoints - damage;

    if (new_lifepoints < 1)
    {
      worm_dead = true;
      attacked_worm->setWormDead(worm_dead);
      std::cout << attacked_worm->getName() << " ("
                << attacked_worm->getId() << ") died."
                << std::endl;
    }
    else
    {
      attacked_worm->setLifePoints(new_lifepoints);
      std::cout << attacked_worm->getName() << " ("
                << attacked_worm->getId() << ") took "
                << damage << "hp damage"
                << std::endl;
    }
  }

  //----------------------------------------------------------------------------
  void Action::fallDamage(Worm *worm, int rows_fallen)
  {
    int damage_fallen = ( rows_fallen-- ) * 10;

    old_lifepoints = worm->getLifePoints();
    new_lifepoints = old_lifepoints - damage_fallen;

    if (new_lifepoints < 1)
    {
      worm_dead = true;
      worm->setWormDead(worm_dead);
      std::cout << worm->getName() << " (" << worm->getId()
                << ") fell into his death."
                << std::endl;
    }
    else if (damage_fallen > 0)
    {
      worm->setLifePoints(new_lifepoints);
      std::cout << worm->getName() << " ("
                << worm->getId() << ") took "
                << damage << "hp fall damage"
                << std::endl;
    }
  }

  //----------------------------------------------------------------------------
  void Action::meleeAttacking(Game &game, int row, int col)
  {
    int   damage        = 50;
    Worm *attacked_worm = game.searchWorm(row, col);
    std::cout << "Attack hit Worm at position ("
              << row << ", " << col << ")"
              << std::endl;

    //resets lifepoints or sends death message
    attackDamage(attacked_worm, damage);

    if (attacked_worm->getWormDead() == true)
    {
      game.setFieldType(Field::AIR, row, col);

      //checks gravity above dead worm
      gravityAbove(game, row - 1, col);
    }
  }

  //----------------------------------------------------------------------------
  int Action::bazookaCrossAttacking(Game &game, int row, int col, int damage)
  {
    int temp_row = row;
    int temp_col = col;

    //sets row or col value to next attack field
    for (int counter = 0; counter < 4; counter++)
    {
      if (counter == 0)
      {
        temp_row--;
      }
      else if (counter == 1)
      {
        temp_col++;
      }
      else if (counter == 2)
      {
        temp_row++;
      }
      else if (counter == 3)
      {
        temp_col--;
      }

      if (temp_row == game.getBoardHeight())
      {
        temp_row = row;
        temp_col = col;
        continue;
      }

      //if hits chest
      if (game.getFieldType(temp_row, temp_col) == Field::CHEST)
      {
        std::cout << "Shot hit Chest at position ("
                  << temp_row << ", " << temp_col << ")"
                  << std::endl;

        //deletes chest if it got hit
        Chest *chest = game.searchChest(temp_row, temp_col);
        chest->setToDelete(true);
        game.setFieldType(Field::AIR, temp_row, temp_col);

        action_done = 1;
      }

      //if hits earth
      else if (game.getFieldType(temp_row, temp_col) == Field::EARTH)
      {
        std::cout << "Shot hit Earth at position ("
                  << temp_row << ", " << temp_col << ")"
                  << std::endl;
        game.setFieldType(Field::AIR, temp_row, temp_col);

        action_done = 1;
      }

      //if hits worm
      else if (game.getFieldType(temp_row, temp_col) == Field::WORM
               || game.getFieldType(temp_row, temp_col) == Field::WORM2)
      {
        Worm *attacked_worm = game.searchWorm(temp_row, temp_col);
        std::cout << "Shot hit Worm at position ("
                  << temp_row << ", " << temp_col << ")"
                  << std::endl;

        //resets lifepoints or sends death message
        attackDamage(attacked_worm, damage);

        //if worm died
        if (attacked_worm->getWormDead() == true)
        {
          game.setFieldType(Field::AIR, temp_row, temp_col);
        }
        action_done = 1;
      }

      temp_row = row;
      temp_col = col;
    }

    //checks gravity for attacked fields
    for (int counter = 0; counter < 4; counter++)
    {
      if (counter == 0)
      {
        temp_row--;
      }
      else if (counter == 1)
      {
        temp_col++;
      }
      else if (counter == 2)
      {
        temp_row++;
      }
      else if (counter == 3)
      {
        temp_col--;
      }

      gravityAbove(game, temp_row - 1, temp_col);

      temp_row = row;
      temp_col = col;
    }

    return action_done;
  }

  //----------------------------------------------------------------------------
  void Action::gravityAbove(Game &game, int row, int col)
  {
    // gravity if field above is worm
    if (game.getFieldType(row, col) == Field::WORM
        || game.getFieldType(row, col) == Field::WORM2)
    {
      //as long as above field is another worm
      for (int counter = row; game.getFieldType(counter, col) == Field::WORM
           || game.getFieldType(counter, col) == Field::WORM2; counter--)
      {
        Worm *worm = game.searchWorm(counter, col);
        int temp_row = row;

        //as long field under founded worm is air he falls
        while (game.getFieldType(( row + 1 ), col) == Field::AIR)
        {
          row++;
          if (row + 1 == game.getBoardHeight())
          {
            break;
          }
        }

        //if worm fell out of map
        if (( row + 1 ) == game.getBoardHeight())
        {
          worm_dead = true;
          worm->setWormDead(worm_dead);
          std::cout << worm->getName() << " (" << worm->getId()
                    << ") fell out of the map."
                    << std::endl;
          game.setFieldType(Field::AIR, temp_row, col);
          break;
        }

        //if worm fell on earth or worm
        if (game.getFieldType(( row + 1 ), col) == Field::EARTH
            || game.getFieldType(( row + 1 ), col) == Field::WORM
            || game.getFieldType(( row + 1 ), col) == Field::WORM2)
        {
          game.setFieldType(Field::AIR, temp_row, col);
          game.setFieldType(Field::WORM, row, col);
          worm->setXposition(col);
          worm->setYposition(row);
          fallDamage(worm, ( temp_row - row ));
        }

        //if worm fell on water he drowned
        else if (game.getFieldType(( row + 1 ), col) == Field::WATER)
        {
          worm_dead = true;
          worm->setWormDead(worm_dead);
          std::cout << worm->getName() << " (" << worm->getId()
                    << ") drowned." << std::endl;
          game.setFieldType(Field::AIR, temp_row, col);
        }
      }
    }

    // gravity if field above is chest
    if (game.getFieldType(row, col) == Field::CHEST)
    {
      game.makeAllChestsFallDown(row, col);
    }
  }

  //----------------------------------------------------------------------------
  int Action::attackDirectionRow(std::string param)
  {
    //sets value according to direction, upsided = -1, downsided = 1
    if (param == "u")
    {
      attack_direction_row = -1;
    }
    else if (param == "ru")
    {
      attack_direction_row = -1;
    }
    else if (param == "r")
    {
      attack_direction_row = 0;
    }
    else if (param == "rd")
    {
      attack_direction_row = 1;
    }
    else if (param == "d")
    {
      attack_direction_row = 1;
    }
    else if (param == "ld")
    {
      attack_direction_row = 1;
    }
    else if (param == "l")
    {
      attack_direction_row = 0;
    }
    else if (param == "lu")
    {
      attack_direction_row = -1;
    }

    return attack_direction_row;
  }

  //----------------------------------------------------------------------------
  int Action::attackDirectionCol(std::string param)
  {
    //sets value according to direction, leftsided = -1, rightsided = 1
    if (param == "u")
    {
      attack_direction_col = 0;
    }
    else if (param == "ru")
    {
      attack_direction_col = 1;
    }
    else if (param == "r")
    {
      attack_direction_col = 1;
    }
    else if (param == "rd")
    {
      attack_direction_col = 1;
    }
    else if (param == "d")
    {
      attack_direction_col = 0;
    }
    else if (param == "ld")
    {
      attack_direction_col = -1;
    }
    else if (param == "l")
    {
      attack_direction_col = -1;
    }
    else if (param == "lu")
    {
      attack_direction_col = -1;
    }

    return attack_direction_col;
  }

  //----------------------------------------------------------------------------
  bool Action::correctParameter(std::string param)
  {
    if (param == "l")
    {
      return true;
    }
    if (param == "r")
    {
      return true;
    }
    if (param == "d")
    {
      return true;
    }
    if (param == "u")
    {
      return true;
    }
    if (param == "ld")
    {
      return true;
    }
    if (param == "rd")
    {
      return true;
    }
    if (param == "lu")
    {
      return true;
    }
    if (param == "ru")
    {
      return true;
    }

    return false;
  }
}
