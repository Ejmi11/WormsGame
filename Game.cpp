#include <algorithm>
#include <climits>
#include <exception>
#include <fstream> // streams "text" files in and out
#include <iostream>
#include <iterator>
#include <string.h>
#include <vector>

#include "Action.h"
#include "Choose.h"
#include "Command.h"
#include "Game.h"
#include "Help.h"
#include "Map.h"
#include "Move.h"
#include "Quit.h"
#include "State.h"

namespace Sep
{
  const int RETURN_SUCCESS = 0;
  const int RETURN_INVALID_CONFIG = -1;
  const int RETURN_INVALID_TARGET = -2;
  const int RETURN_NO_MEMORY = -2;
  const int DROWNED_WORM = -3;
  const std::string ERROR_INVALID_CONFIG = "[ERROR] invalid config file!\n";
  const std::string ERROR_INVALID_TARGET = "[ERROR] invalid target!\n";
  const std::string ERROR_INVALID_PARAMETER = "[ERROR] invalid parameter!\n";
  const std::string ERROR_NO_AMMUNITION = "[ERROR] no ammunition\n";
  const std::string ERROR_WRONG_PARAMETER_COUNT =
    "[ERROR] wrong parameter count!\n";
  const std::string WARNING_CANT_MOVE_FURTHER =
    "[WARNING] can't move further\n";
  const std::string OUT_OF_MEMORY = "[ERROR] memory error!\n";
}

//------------------------------------------------------------------------------
Sep::Game::Game() : board_width_(0), board_height_(0)
{
  bool enough_memory = true;
  actual_worm_ = nullptr;
  map_.resize((size_t) board_height_,
    std::vector<Field::FieldType>((size_t) board_width_));

  try
  {
    commands_list_.push_back(new Quit("quit"));
    commands_list_.push_back(new Move("move"));
    commands_list_.push_back(new Choose("choose"));
    commands_list_.push_back(new Action("action"));
    commands_list_.push_back(new Map("map"));
    commands_list_.push_back(new Help("help"));
    commands_list_.push_back(new State("state"));

    players_list_.push_back(new Player(1));
    players_list_.push_back(new Player(2));
  }
  catch (std::bad_alloc &bad_memory)
  {
    std::cout << OUT_OF_MEMORY;
    enough_memory = false;
  }

  if (enough_memory)
  {
    setActivePlayer(1); // i set the first active player as player 1
    int worms_nr = 6;
    int start_life_points = 100;

    for (int counter = 0; counter < worms_nr; counter++)
    {
      int worm_id = counter + 1;

      try
      {
        worms_list_.push_back(new Worm("", start_life_points, 0, 0, worm_id));
      }
      catch (std::bad_alloc &bad_memory)
      {
        std::cout << OUT_OF_MEMORY;
        enough_memory = false;

        // release all allocated memory
        for (CmdList::iterator cmd = commands_list_.begin();
             cmd != commands_list_.end();cmd++)
        {
          delete (*cmd);
          *cmd = nullptr;
        }
        commands_list_.clear();

        for (Players::iterator player_iterator = players_list_.begin();
             player_iterator != players_list_.end(); player_iterator++)
        {
          delete (*player_iterator);
          *player_iterator = nullptr;
        }
        players_list_.clear();
      }
    }
  }
}

//------------------------------------------------------------------------------
Sep::Game::~Game()
{
  for (CmdList::iterator cmd = commands_list_.begin();
       cmd != commands_list_.end();cmd++)
  {
    delete (*cmd);
    *cmd = nullptr;
  }
  commands_list_.clear();

  for (Players::iterator player_iterator = players_list_.begin();
       player_iterator != players_list_.end(); player_iterator++)
  {
    delete (*player_iterator);
    *player_iterator = nullptr;
  }
  players_list_.clear();

  for (Worms::iterator worm_iterator = worms_list_.begin();
       worm_iterator != worms_list_.end(); worm_iterator++)
  {
    delete (*worm_iterator);
    *worm_iterator = nullptr;
  }
  worms_list_.clear();

  for (Chests::iterator chest_iterator = chest_list_.begin();
       chest_iterator != chest_list_.end(); chest_iterator++)
  {
    delete (*chest_iterator);
    *chest_iterator = nullptr;
  }
  chest_list_.clear();
}

//------------------------------------------------------------------------------
int Sep::Game::ignoreKomentSpacesEnters(
  std::vector<std::string>  file_input_vec,
  std::string              &size_content,
  std::vector<std::string> &map_content)
{
  std::string                        size_checker = "SIZE: ";
  std::string                        map_checker  = "MAP:";
  std::vector<std::string>::iterator it;
  size_t                             white_space_counter = 0;
  std::vector<std::string>           temp_main_inputs;

  for (it = file_input_vec.begin(); it != file_input_vec.end(); it++)
  {
    std::string temp_string;
    for (size_t counter = 0; counter < (*it).size(); counter++)
    {
      if ((*it)[counter] == ' ')
      {
        white_space_counter++;
      }
      else
      {
        temp_string =
          (*it).substr(white_space_counter, (*it).size());
        white_space_counter = 0;
        break;
      }
    }

    if (!temp_string.empty() && temp_string[0] != '#')
    {
      temp_main_inputs.push_back(temp_string);
    }
  }

  size_t      size_position = 0;
  size_t      map_position  = 1;
  size_t      size_char_nr  = 6;
  size_t      map_char_nr   = 4;
  std::string temp_size;
  std::string temp_map;

  for (size_t position = 0; position < temp_main_inputs.size(); position++)
  {
    if (position == size_position)
    {
      temp_size = temp_main_inputs[position].substr(0, size_char_nr);

      if (temp_size == size_checker)
      {
        size_content = temp_main_inputs[position].substr(size_char_nr,
                                                         temp_main_inputs[
                                                           position].size());
      }
      else
      {
        std::cout << ERROR_INVALID_CONFIG;
        return RETURN_INVALID_CONFIG;
      }
    }
    else if (position == map_position)
    {
      size_t character_nr = 0;
      for (size_t counter = 0;
           counter < temp_main_inputs[position].size();
           counter++)
      {
        if (temp_main_inputs[position][counter] != ' ')
        {
          character_nr++;
        }
      }

      temp_map = temp_main_inputs[position].substr(0, map_char_nr);

      if (temp_map != map_checker || character_nr > map_char_nr)
      {
        std::cout << ERROR_INVALID_CONFIG;
        return RETURN_INVALID_CONFIG;
      }
    }
    else
    {
      map_content.push_back(temp_main_inputs[position]);
    }
  }
  return RETURN_SUCCESS;
}

//------------------------------------------------------------------------------
int Sep::Game::sizeParamGet(std::string size_content,
                            int         &real_height,
                            int         &real_width)
{
  std::vector<char> widths;
  std::vector<char> heights;
  int               first_word   = 1;
  int               second_word  = 2;
  int               word_counter = 0;
  int               spaces       = 1;
  int               zero         = 48;
  int               nine         = 57;

  for (size_t count = 0; count < size_content.size(); count++)
  {
    if (size_content[count] != ' ')
    {
      if (size_content[count] < zero || size_content[count] > nine)
      {
        std::cout << ERROR_INVALID_CONFIG;
        return RETURN_INVALID_CONFIG;
      }

      if (spaces)
      {
        word_counter++;
      }

      spaces = 0;

      if (word_counter == first_word)
      {
        widths.push_back(size_content[count]);
      }
      else if (word_counter == second_word)
      {
        heights.push_back(size_content[count]);
      }
    }
    else
    {
      spaces = 1;
    }
  }

  if (word_counter != second_word)
  {
    std::cout << ERROR_INVALID_CONFIG;
    return RETURN_INVALID_CONFIG;
  }

  // change from vector<char> to string
  std::string wid(widths.begin(), widths.end());
  std::string hig(heights.begin(), heights.end());

  // change from string to int
  real_width  = stoi(wid);
  real_height = stoi(hig);

  // width and height must be >=10 , <=80
  int ten    = 10;
  int eighty = 80;
  if (real_width < ten || real_width > eighty)
  {
    std::cout << ERROR_INVALID_CONFIG;
    return RETURN_INVALID_CONFIG;
  }

  if (real_height < ten || real_height > eighty)
  {
    std::cout << ERROR_INVALID_CONFIG;
    return RETURN_INVALID_CONFIG;
  }

  return RETURN_SUCCESS;
}

//------------------------------------------------------------------------------
int Sep::Game::mapContentChecker(std::vector<std::string> map_content,
                                 int real_height, int real_width)
{
  char   air            = 'A';
  char   earth          = 'E';
  char   water          = 'W';
  size_t first_position = 0;
  size_t last_position  = map_content.size() - 1;
  int    width_counter  = 0;
  int    height_counter = 0;

  for (size_t counte1 = 0; counte1 < map_content.size(); counte1++)
  {
    for (size_t counte2 = 0; counte2 < map_content[counte1].size(); counte2++)
    {
      if (map_content[counte1][counte2] != air &&
          map_content[counte1][counte2] != earth && counte1 < last_position)
      {
        std::cout << ERROR_INVALID_CONFIG;
        return RETURN_INVALID_CONFIG;
      }
      if (map_content[first_position][counte2] != air &&
          counte1 == first_position)
      {
        std::cout << ERROR_INVALID_CONFIG;
        return RETURN_INVALID_CONFIG;
      }
      if (map_content[last_position][counte2] != earth &&
          map_content[last_position][counte2] != water
          && counte1 == last_position)
      {
        std::cout << ERROR_INVALID_CONFIG;
        return RETURN_INVALID_CONFIG;
      }

      width_counter++;
    }
    if (real_width != width_counter)
    {
      std::cout << ERROR_INVALID_CONFIG;
      return RETURN_INVALID_CONFIG;
    }

    width_counter = 0;

    height_counter++;
  }

  if (real_height != height_counter)
  {
    std::cout << ERROR_INVALID_CONFIG;
    return RETURN_INVALID_CONFIG;
  }

  // setting the board_width_ and board_height_ and the map_
  board_width_  = real_width;
  board_height_ = real_height;

  // resetting the size of the map_ vector
  map_.resize((size_t) board_height_,
              std::vector<Field::FieldType>((size_t) board_width_));

  for (size_t counte1 = 0; counte1 < map_content.size(); counte1++)
  {
    for (size_t counte2 = 0; counte2 < map_content[counte1].size(); counte2++)
    {
      if (map_content[counte1][counte2] != ' ')
      {
        if (map_content[counte1][counte2] == air)
        {
          map_[counte1][counte2] = Field::AIR;
        }

        if (map_content[counte1][counte2] == earth)
        {
          map_[counte1][counte2] = Field::EARTH;
        }

        if (map_content[counte1][counte2] == water)
        {
          map_[counte1][counte2] = Field::WATER;
        }
      }
    }
  }

  return RETURN_SUCCESS;
}

//------------------------------------------------------------------------------
int Sep::Game::loadConfig(std::string cfg_file)
{
  std::ifstream infile;

  infile.open(cfg_file);
  if (!infile)
  {
    std::cout << ERROR_INVALID_CONFIG;
    return RETURN_INVALID_CONFIG;
  }

  std::string                        input_string;
  std::vector<std::string>           file_input_vec;
  std::vector<std::string>::iterator it;
  std::string                        head_file = "#SWORMS";
  std::string                        size_in_map;

  while (!infile.eof())
  {
    getline(infile, input_string);
    file_input_vec.push_back(input_string);
  }

  infile.close();

  // check #SWORMS
  if (file_input_vec.front() != head_file)
  {
    std::cout << ERROR_INVALID_CONFIG;
    return RETURN_INVALID_CONFIG;
  }

  // Ignore the comments starting with #comment and empty spaces
  int                      valid = 0;
  std::string              size_content;
  std::vector<std::string> map_content;
  valid = ignoreKomentSpacesEnters(file_input_vec, size_content, map_content);
  if (valid == RETURN_INVALID_CONFIG)
  {
    return RETURN_INVALID_CONFIG;
  }

  // SIZE: 40 11
  int real_width  = 0;
  int real_height = 0;

  valid = sizeParamGet(size_content, real_height, real_width);
  if (valid == RETURN_INVALID_CONFIG)
  {
    return RETURN_INVALID_CONFIG;
  }

  valid = mapContentChecker(map_content, real_height, real_width);
  if (valid == RETURN_INVALID_CONFIG)
  {
    return RETURN_INVALID_CONFIG;
  }

  return RETURN_SUCCESS;
}

//------------------------------------------------------------------------------
int Sep::Game::addWorm(int row, int col, int player)
{
  int              player2   = 2;
  Field::FieldType worm_type = Field::WORM;

  if (player == player2)
  {
    worm_type = Field::WORM2;
  }

  if (map_[row][col] == Field::AIR)
  {
    map_[row][col] = worm_type;
  }
  else
  {
    std::cout << ERROR_INVALID_TARGET;
    return RETURN_INVALID_TARGET;
  }

  for (; map_[row + 1][col] == Field::AIR; row++)
  {
    map_[row][col]     = Field::AIR;
    map_[row + 1][col] = worm_type;
  }

  if (map_[row + 1][col] == Field::WATER)
  {
    map_[row][col] = Field::AIR;
    return DROWNED_WORM;
  }

  // return the actual row of the worm on the map
  return row;
}

//------------------------------------------------------------------------------
void Sep::Game::printMapHelper()
{
  int max_count_nr = 9;
  int count_nr     = 0;

  for (int counter = 0; counter < board_width_; counter++)
  {
    if (count_nr > max_count_nr)
    {
      count_nr = 0;
    }

    if (counter == 0)
    {
      std::cout << "  " << count_nr;
    }
    else if (counter == board_width_ - 1)
    {
      std::cout << count_nr << std::endl;
    }
    else
    {
      std::cout << count_nr;
    }

    count_nr++;
  }
}

//------------------------------------------------------------------------------
void Sep::Game::printMap()
{
  int count_nr = 0;
  int nr_ten   = 10;

  std::cout << "Current Map:" << std::endl;

  printMapHelper();

  for (int counter1 = 0; counter1 < board_height_; counter1++)
  {
    for (int counter2 = 0; counter2 < board_width_; counter2++)
    {
      char field_var = getCharacter(map_[counter1][counter2]);

      if (counter2 == 0 && count_nr < nr_ten)
      {
        std::cout << " " << count_nr << field_var;
      }
      else if (counter2 == board_width_ - 1 && count_nr < nr_ten)
      {
        std::cout << field_var << " " << count_nr << std::endl;
      }
      else if (counter2 == 0 && count_nr >= nr_ten)
      {
        std::cout << count_nr << field_var;
      }
      else if (counter2 == board_width_ - 1 && count_nr >= nr_ten)
      {
        std::cout << field_var << count_nr << std::endl;
      }
      else
      {
        std::cout << field_var;
      }
    }
    count_nr++;
  }
  printMapHelper();
}

//------------------------------------------------------------------------------
void Sep::Game::rightMovement(int row, int col, int steps)
{
  Field::FieldType worm_type = Field::WORM;
  int              player2   = 2;

  if (getActualPlayer()->getName() == player2)
  {
    worm_type = Field::WORM2;
  }

  int destination = col + steps;

  for (int counter = col; counter < destination; counter++)
  {
    if (map_[row][counter + 1] == Field::EARTH) // up movement earth
    {
      if (row - 1 < 0) // check if position is 0 for real_row
      {
        // set x position
        getActualPlayer()->getWormFromVector(0)->setXposition(counter);
        // set y position
        getActualPlayer()->getWormFromVector(0)->setYposition(row);

        std::cout << WARNING_CANT_MOVE_FURTHER;
        return;
      }
      // check if double earth (one up the other)
      if (map_[row - 1][counter + 1] != Field::AIR)
      {
        Field::FieldType type = map_[row - 1][counter + 1];
	      // double earth or worm
        if (type == Field::EARTH || type == Field::WORM || type == Field::WORM2)
        {
          // set x position
          getActualPlayer()->getWormFromVector(0)->setXposition(counter);
          // set y position
          getActualPlayer()->getWormFromVector(0)->setYposition(row);

          std::cout << WARNING_CANT_MOVE_FURTHER;
          return;
        }
        else if (map_[row - 1][counter + 1] == Field::CHEST)
        {
          Sep::Chest *chest = searchChest(row - 1, counter + 1);
          getActualPlayer()->getWormFromVector(0)->openChest(*this, chest);
          // set x position
          getActualPlayer()->getWormFromVector(0)->setXposition(counter + 1);
          // set y position
          getActualPlayer()->getWormFromVector(0)->setYposition(row - 1);
        }
      }

      map_[row][counter]         = Field::AIR;
      map_[row - 1][counter + 1] = worm_type; // Field::WORM;

      // set x position
      getActualPlayer()->getWormFromVector(0)->setXposition(counter + 1);
      // set y position
      getActualPlayer()->getWormFromVector(0)->setYposition(row - 1);

      row--;
    }
    // up movement Worms
    else if (map_[row][counter + 1] == Field::WORM ||
             map_[row][counter + 1] == Field::WORM2)
    {
      int temp_row = row;
      while (map_[row - 1][counter + 1] == Field::WORM ||
             map_[row - 1][counter + 1] == Field::WORM2)
      {
        if (( row - 1 ) == 0)
        {
          // set x position
          getActualPlayer()->getWormFromVector(0)->setXposition(counter);
          // set y position
          getActualPlayer()->getWormFromVector(0)->setYposition(row);

          std::cout << WARNING_CANT_MOVE_FURTHER;
          return;
        }
        row--;
      }
      map_[temp_row][counter]    = Field::AIR;
      map_[row - 1][counter + 1] = worm_type; // Field::WORM;

      // set x position
      getActualPlayer()->getWormFromVector(0)->setXposition(counter + 1);
      // set y position
      getActualPlayer()->getWormFromVector(0)->setYposition(row - 1);

      row--;
    }
    // down movement and right movements
    else if (map_[row][counter + 1] == Field::AIR)
    {
      int fall_right    = 1;
      int return_signal = 0;
      map_ = getActualPlayer()->getWormFromVector(0)->gravity(*this, worm_type,
                                                              fall_right, map_,
                                                              return_signal);

      row = getActualPlayer()->getWormFromVector(0)->getYposition();

      if (return_signal)
      {
        return;
      }
    }
    else if (map_[row][counter + 1] == Field::CHEST) // chest on the same level
    {
      Sep::Chest *chest = searchChest(row, counter + 1);
      getActualPlayer()->getWormFromVector(0)->openChest(*this, chest);
      // set x position
      getActualPlayer()->getWormFromVector(0)->setXposition(counter + 1);
      // set y position
      getActualPlayer()->getWormFromVector(0)->setYposition(row);
      setFieldType(Field::AIR, row, counter);
    }
  }
}

//------------------------------------------------------------------------------
void Sep::Game::leftMovement(int row, int col, int steps)
{
  Field::FieldType worm_type = Field::WORM;
  int              player2   = 2;

  if (getActualPlayer()->getName() == player2)
  {
    worm_type = Field::WORM2;
  }

  int destination = col + steps;

  for (int counter = col; counter > destination; counter--)
  {
    if (map_[row][counter - 1] == Field::EARTH) // up movement earth
    {
      if (row - 1 < 0) // check if position is 0 for real_row
      {
        // set x position
        getActualPlayer()->getWormFromVector(0)->setXposition(counter);
        // set y position
        getActualPlayer()->getWormFromVector(0)->setYposition(row);

        std::cout << WARNING_CANT_MOVE_FURTHER;
        return;
      }

      // Field::FieldType type = map_[row - 1][counter - 1];
      if (map_[row - 1][counter - 1] != Field::AIR)
      {
        // double earth or worm
        if (map_[row - 1][counter - 1] == Field::EARTH
            || map_[row - 1][counter - 1] == Field::WORM
            || map_[row - 1][counter - 1] == Field::WORM2)
        {
          // set x position
          getActualPlayer()->getWormFromVector(0)->setXposition(counter);
          // set y position
          getActualPlayer()->getWormFromVector(0)->setYposition(row);

          std::cout << WARNING_CANT_MOVE_FURTHER;
          return;
        }
        else if (map_[row - 1][counter - 1] == Field::CHEST)
        {
          Sep::Chest *chest = searchChest(row - 1, counter - 1);
          getActualPlayer()->getWormFromVector(0)->openChest(*this, chest);
          // set x position
          getActualPlayer()->getWormFromVector(0)->setXposition(counter - 1);
          // set y position
          getActualPlayer()->getWormFromVector(0)->setYposition(row - 1);
        }
      }

      map_[row][counter]         = Field::AIR;
      map_[row - 1][counter - 1] = worm_type;

      // set x position
      getActualPlayer()->getWormFromVector(0)->setXposition(counter - 1);
      // set y position
      getActualPlayer()->getWormFromVector(0)->setYposition(row - 1);

      row--;
    }
    // up movement Worms
    else if (map_[row][counter - 1] == Field::WORM ||
             map_[row][counter - 1] == Field::WORM2)
    {
      int temp_row = row;
      while (map_[row - 1][counter - 1] == Field::WORM ||
             map_[row - 1][counter - 1] == Field::WORM2)
      {
        if (( row - 1 ) == 0)
        {
          // set x position
          getActualPlayer()->getWormFromVector(0)->setXposition(counter);
          // set y position
          getActualPlayer()->getWormFromVector(0)->setYposition(row);

          std::cout << WARNING_CANT_MOVE_FURTHER;
          return;
        }
        row--;
      }
      map_[temp_row][counter]    = Field::AIR;
      map_[row - 1][counter - 1] = worm_type; // Field::WORM;

      // set x position
      getActualPlayer()->getWormFromVector(0)->setXposition(counter - 1);
      // set y position
      getActualPlayer()->getWormFromVector(0)->setYposition(row - 1);

      row--;
    }
    // down movement and left movements
    else if (map_[row][counter - 1] == Field::AIR)
    {
      int return_signal = 0;
      int fall_left     = -1;
      map_ = getActualPlayer()->getWormFromVector(0)->gravity(*this, worm_type,
                                                              fall_left, map_,
                                                              return_signal);


      row = getActualPlayer()->getWormFromVector(0)->getYposition();

      if (return_signal)
      {
        return;
      }
    }
    // chest on the same level
    else if (map_[row][counter - 1] == Field::CHEST)
    {
      Sep::Chest *chest = searchChest(row, counter - 1);
      getActualPlayer()->getWormFromVector(0)->openChest(*this, chest);

      // set x position
      getActualPlayer()->getWormFromVector(0)->setXposition(counter - 1);
      // set y position
      getActualPlayer()->getWormFromVector(0)->setYposition(row);
      setFieldType(Field::AIR, row, counter);
    }
  }
}

//------------------------------------------------------------------------------
void Sep::Game::move(int row, int col, int steps)
{
  if (map_[row][col] != Field::WORM && map_[row][col] != Field::WORM2)
  {
    std::cout << ERROR_INVALID_TARGET;
    return;
  }

  int max_right_steps = 3;
  int max_left_steps  = -3;

  if (steps > max_right_steps)
  {
    steps = max_right_steps;
  }

  if (steps < max_left_steps)
  {
    steps = max_left_steps;
  }

  // movement
  if (steps > 0) // right side
  {
    rightMovement(row, col, steps);
  }
  else if (steps < 0) // left side
  {
    leftMovement(row, col, steps);
  }
  else // if steps == 0 do nothing
  {
    return;
  }
}

//------------------------------------------------------------------------------
char Sep::Game::getCharacter(Field::FieldType field_type)
{
  char water = 'W';
  char earth = 'E';
  char air   = ' ';
  char worm  = '~';
  char worm2 = '*';
  char chest = '#';

  if (field_type == Field::WATER)
  {
    return water;
  }
  if (field_type == Field::EARTH)
  {
    return earth;
  }
  if (field_type == Field::AIR)
  {
    return air;
  }
  if (field_type == Field::WORM)
  {
    return worm;
  }
  if (field_type == Field::WORM2)
  {
    return worm2;
  }
  if (field_type == Field::CHEST)
  {
    return chest;
  }

  // to escape the warning
  return RETURN_SUCCESS;
}

//------------------------------------------------------------------------------
void Sep::Game::setStateRunning(bool runme_state)
{
  running_state_ = runme_state;
}

// -----------------------------------------------------------------------------
std::vector<std::string> Sep::Game::customSplittString(
  const std::string &string_to_parse)
{
  std::vector<std::string> vectorToReturn;

  typedef std::string::size_type size_of_str;
  size_of_str index_ = 0;
  for (; index_ != string_to_parse.size();) // processing the string
  {
    for (;
         isspace(string_to_parse[index_]) && index_ != string_to_parse.size();
         ++index_)
    {
    }
    // next word end
    size_of_str index_next_ = index_;
    // no found characters in orig index_next_, current index_next_ is a space
    for (;
         !isspace(string_to_parse[index_next_]) &&
         index_next_ != string_to_parse.size();
         index_next_++)
    {
    }
    // if in case no space char words are found push it to vectorToReturn
    if (index_ != index_next_)
    {
      // from the startpoint string_to_parse at given index,
      // we save index_next_ - index substrings
      vectorToReturn.push_back(string_to_parse.substr(index_,
                                                    index_next_ - index_));
      index_ = index_next_;
    }
  }
  return vectorToReturn;
}

//------------------------------------------------------------------------------
int Sep::Game::parseInput(std::vector<std::string> &command_parameter,
                          std::string              &base_command)
{
  int return_val = RETURN_SUCCESS;
  // string holding everything from command to paramas
  // std::string commandPlusParamms;
  // string holding everything from command to paramas
  std::string commandPlusParamms;

  // read the whole string and save it
  // getline(std::cin, commandPlusParamms);
  // read the whole string and save it
  getline(std::cin, commandPlusParamms);
  if (std::cin.eof())
  {
    return_val = 2;
    return return_val;
  }

  if (commandPlusParamms == "")
  {
    return_val = 1;
    return return_val;
  }

  // command case insensitive
  size_t big_A = 65;
  size_t big_Z = 90;
  size_t toLow = 32;
  for (size_t counter = 0; counter < commandPlusParamms.length(); counter++)
  {
    if (static_cast<size_t>( commandPlusParamms.at(counter)) <= big_Z
        && static_cast<size_t>( commandPlusParamms.at(counter)) >= big_A)
    {
      commandPlusParamms.replace(counter, 1, 1,
                                 static_cast<char>( commandPlusParamms.at(
                                                      counter)) + toLow);
    }
  }

  // parse the string and save it to vector
  command_parameter = customSplittString(commandPlusParamms);
  // the command saved in the base_command and in the command_parameter[0]
  base_command = command_parameter[0];

  return return_val;
}

//------------------------------------------------------------------------------
std::vector<int> Sep::Game::randomXCoordVector()
{
  // random x position vector
  std::vector<int> x_positions;

  int  start_position   = 0;
  int  check_position_c = 0;
  int  position_x       = 0;
  int  max_worm_nr      = 5; // start count from 0
  bool flag             = false;

  while (1)
  {
    position_x = random_.getRandomInt(start_position,
                                      board_width_ - 1);

    for (int iter = 0; iter < check_position_c; iter++)
    {
      if (x_positions[iter] == position_x)
      {
        flag = true;
      }
    }
    if (flag)
    {
      flag = false;
      continue;
    }

    x_positions.push_back(position_x);

    if (check_position_c == max_worm_nr)
    {
      break;
    }
    check_position_c++;
  }
  return x_positions;
}

//------------------------------------------------------------------------------
void Sep::Game::wormsFieldBegin(Player *player1, Player *player2)
{
  Player *player_one = player1;
  Player *player_two = player2;

  // random x position vector
  std::vector<int>         x_positions;
  std::vector<std::string> worm_names;

  int         start_position   = 0;
  int         check_position_c = 0;
  std::string worm_name        = "";
  int         max_worm_nr      = 5; // start count from 0
  bool        flag             = false;

  while (1)
  {
    if (flag == false)
    {
      worm_name = random_.getWormName();
    }

    int position_x = random_.getRandomInt(start_position,
                                          board_width_ - 1);

    for (int iter = 0; iter < check_position_c; iter++)
    {
      if (x_positions[iter] == position_x)
      {
        flag = true;
        break;
      }
      else
      {
        flag = false;
      }
    }
    if (flag)
    {
      // flag = false;
      continue;
    }

    x_positions.push_back(position_x);
    worm_names.push_back(worm_name);

    if (check_position_c == max_worm_nr)
    {
      break;
    }
    check_position_c++;
  }

  // setting the worms in their beginning random position
  int count_dead = 0;

  for (size_t counter = 0; counter < worms_list_.size(); counter++)
  {
    int position_y = 0;
    // set x position and names of worms
    worms_list_[counter]->setXposition(x_positions[counter]);
    worms_list_[counter]->setName(worm_names[counter]);

    int row_or_status = addWorm(position_y, x_positions[counter],
                                worms_list_[counter]->getWormType());

    if (row_or_status == DROWNED_WORM)
    {
      count_dead++;
      worms_list_[counter]->setWormDead(true);
    }
    else
    {
      // set y position of worms
      worms_list_[counter]->setYposition(row_or_status);
    }

    // set worms to their prospective players
    size_t change_player = 3;
    if (counter < change_player)
    {
      if (worms_list_[counter]->getWormDead() == false)
      {
        player_one->setWormToVector(worms_list_[counter]);
      }
    }
    else
    {
      if (worms_list_[counter]->getWormDead() == false)
      {
        player_two->setWormToVector(worms_list_[counter]);
      }
    }
  }

  for (size_t counter = 0; counter < worms_list_.size(); counter++)
  {
    std::cout << "spawning " << worms_list_[counter]->getName()
              << " (" << worms_list_[counter]->getId()
              << ") at (0, " << worms_list_[counter]->getXposition() << ")"
              << std::endl;

    if (worms_list_[counter]->getWormDead() == true)
    {
      std::cout << worms_list_[counter]->getName() << " (" <<
        worms_list_[counter]->getId() << ") drowned." << std::endl;
    }
  }
}

//------------------------------------------------------------------------------
int Sep::Game::runme(std::string cfg_file)
{
  int ret_val = 0;

  ret_val = this->loadConfig(cfg_file);
  if (ret_val == RETURN_INVALID_CONFIG)
  {
    return RETURN_INVALID_CONFIG;
  }

  running_state_ = true;
  bool                     is_command_known;
  int                      return_value_execute;
  std::string              actual_command;
  std::vector<std::string> params;

  Player *player_one = nullptr;
  Player *player_two = nullptr;

  try
  {
    player_one = new Player(1);
    player_two = new Player(2);
  }
  catch (std::bad_alloc &bad_memory)
  {
    std::cout << OUT_OF_MEMORY;
    return RETURN_NO_MEMORY;
  }

  wormsFieldBegin(player_one, player_two);

  if (player_one->getWormVector().size() == 0
      && player_two->getWormVector().size() == 0)
  {
    std::cout << "END: Draw" << std::endl;
    delete player_one;
    delete player_two;
    return RETURN_SUCCESS;
  }
  if (player_one->getWormVector().size() == 0)
  {
    std::cout << "END: Player 2 win!" << std::endl;
    delete player_one;
    delete player_two;
    return RETURN_SUCCESS;
  }
  if (player_two->getWormVector().size() == 0)
  {
    std::cout << "END: Player 1 win!" << std::endl;
    delete player_one;
    delete player_two;
    return RETURN_SUCCESS;
  }

  // setting the starting player
  setActualPlayer(player_one);
  player_one->getWormFromVector(0)->setCurrentWeapon("gun", Worm::GUN);
  bool to_print = true;

  // starting with player1
  int player_nr = 1;

  std::string move_command   = "move";
  std::string choose_command = "choose";
  std::string action_command = "action";

  bool move_once = false;

  while (running_state_ == true)
  {
    if (to_print)
    {
      printMap();
      std::cout << "Player " << getActualPlayer()->getName() << " Worm "
                << getActualPlayer()->getWormFromVector(0)->getName() << " ("
                << getActualPlayer()->getWormFromVector(0)->getId() << ") at ("
                << getActualPlayer()->getWormFromVector(0)->getYposition() <<
        ", "
                << getActualPlayer()->getWormFromVector(0)->getXposition()
                << ") ready" << std::endl;
      to_print = false;
    }

    std::cout << "sep> ";
    is_command_known = true;
    actual_command   = "";
    params.clear();

    // delete chest objects and check gravity of chest
    for (size_t counter = 0; counter < chest_list_.size(); counter++)
    {
      if (chest_list_[counter]->getToDelete() == true)
      {
        delete chest_list_[counter];
        chest_list_.erase(chest_list_.begin() + counter);
      }
      else
      {
        chest_list_[counter]->makeChestFall(*this);
      }
    }

    int check = parseInput(params, actual_command);

    int just_newLine = 1;
    int ctrlD        = 2;
    if (check == just_newLine)
    {
      continue;
    }
    else if (check == ctrlD)
    {
      break;
    }

    if (move_once && ( actual_command == move_command ))
    {
      std::cout << "[ERROR] command currently not allowed!" << std::endl;
      continue;
    }

    // setting the actual worm
    setActualWorm(getActualPlayer()->getWormFromVector(0));

    std::transform(params[0].begin(),
                   params[0].end(),
                   params[0].begin(),
                   ::tolower);

    for (CmdList::iterator iterator_of_command_list = commands_list_.begin();
         iterator_of_command_list != commands_list_.end();
         iterator_of_command_list++)
    {
      if (actual_command == (*iterator_of_command_list)->getName())
      {
        return_value_execute = (*iterator_of_command_list)->execute(*this,
                                                                    params);
        switch (return_value_execute)
        {
          case 1:
            std::cout << ERROR_WRONG_PARAMETER_COUNT;
            break;
          case 2:
            std::cout << ERROR_NO_AMMUNITION;
            break;
          case 3:
            std::cout << ERROR_INVALID_TARGET;
            break;
          case 4:
            std::cout << ERROR_INVALID_PARAMETER;
            break;
        }
        is_command_known = false;
      }
    }

    if (return_value_execute == 1 || return_value_execute == 3 ||
        return_value_execute == 4)
    {
      continue;
    }

    if (return_value_execute == 2)
    {
      // set to default weapon
      getActualPlayer()->getWormFromVector(0)
      ->setCurrentWeapon("gun", Worm::GUN);
    }

    if (is_command_known == true)
    {
      std::cout << "[ERROR] unknown command!" << std::endl;
      continue;
    }

    if (actual_command == move_command)
    {
      move_once = true;
    }

    // the gravity in play
    gameGravity();

    // print map after move
    if (actual_command == move_command)
    {
      printMap();
    }

    // turn changes only with specific commands
    if (actual_command == action_command ||
        actual_player_->getWormFromVector(0)->getWormDead())
    {
      std::vector<Worm *> player_worms = getActualPlayer()->getWormVector();

      int player1 = 1;
      int player2 = 2;

      if (player_worms[0]->getWormDead())
      {
        player_worms.erase(player_worms.begin());
        getActualPlayer()->setWormVector(player_worms);
      }
      else
      {
        // plush the first element to the last place of the vector
        player_worms.push_back(player_worms[0]);
        // than erase the first place
        player_worms.erase(player_worms.begin());
        // update player worms vector
        getActualPlayer()->setWormVector(player_worms);
      }

      // switch players
      if (player_nr == player1)
      {
        setActualPlayer(player_two);
        player_nr = player2;
      }
      else
      {
        setActualPlayer(player_one);
        player_nr = player1;
      }

      // action->dead->worms for the other player
      player_worms = getActualPlayer()->getWormVector();
      for (size_t counter = 0; counter < player_worms.size(); counter++)
      {
        if (player_worms[0]->getWormDead())
        {
          player_worms.erase(player_worms.begin());
          getActualPlayer()->setWormVector(player_worms);
        }
      }

      if (player_one->getWormVector().size() == 0
          && player_two->getWormVector().size() == 0)
      {
        printMap();
        std::cout << "END: Draw" << std::endl;
        break;
      }
      if (player_one->getWormVector().size() == 0)
      {
        printMap();
        std::cout << "END: Player 2 win!" << std::endl;
        break;
      }
      if (player_two->getWormVector().size() == 0)
      {
        printMap();
        std::cout << "END: Player 1 win!" << std::endl;
        break;
      }

      to_print  = true;
      move_once = false;

      try
      {
        // create new chest object
        chest_list_.push_back(new Chest(random_.getRandomInt(0,
						   board_width_ - 1),
                                                   random_.getRandomInt(0, 4)));
      }
      catch (std::bad_alloc &bad_memory)
      {
        std::cout << OUT_OF_MEMORY;
        return RETURN_NO_MEMORY;
      }

      // get created chest object and add it to field
      Chest *chest = chest_list_.back();
      chest->addChest(*this);
      getActualPlayer()->getWormFromVector(0)
      ->setCurrentWeapon("gun", Worm::GUN);
    }
  }

  delete player_one;
  delete player_two;

  player_one = nullptr;
  player_two = nullptr;

  return RETURN_SUCCESS;
}

//------------------------------------------------------------------------------
void Sep::Game::addPlayer(Sep::Player *player)
{
  players_list_.push_back(player);
}

//------------------------------------------------------------------------------
int Sep::Game::getActivePlayer() const
{
  return player_active_;
}

//------------------------------------------------------------------------------
Sep::Player *Sep::Game::getPlayer(int active_player) const
{
  return players_list_.at(active_player);
}

//------------------------------------------------------------------------------
void Sep::Game::setActivePlayer(int active_player)
{
  player_active_ = active_player;
}

//------------------------------------------------------------------------------
Sep::Worm *Sep::Game::getWorm(int active_worm) const
{
  return worms_list_.at(active_worm);
}

//------------------------------------------------------------------------------
void Sep::Game::printWormList()
{
  for (Worms::iterator wi = worms_list_.begin();
       wi != worms_list_.end(); wi++)
  {
    std::cout << (*wi)->getName() << std::endl;
  }
}

//------------------------------------------------------------------------------
int Sep::Game::getBoardWidth()
{
  return board_width_;
}

//------------------------------------------------------------------------------
int Sep::Game::getBoardHeight()
{
  return board_height_;
}

//------------------------------------------------------------------------------
std::vector<std::vector<Sep::Field::FieldType> > Sep::Game::getMap()
{
  return map_;
}

//------------------------------------------------------------------------------
Sep::Worm *Sep::Game::getActualWorm() const
{
  return actual_worm_;
}

//------------------------------------------------------------------------------
void Sep::Game::setActualWorm(Sep::Worm *actual_worm)
{
  actual_worm_ = actual_worm;
}

//------------------------------------------------------------------------------
void Sep::Game::setActualPlayer(Player *actual_player)
{
  actual_player_ = actual_player;
}

//------------------------------------------------------------------------------
Sep::Player *Sep::Game::getActualPlayer() const
{
  return actual_player_;
}

//------------------------------------------------------------------------------
void Sep::Game::gameGravity()
{
  int gravity_fall = 0;
  int repeat = 3;

  for (int iter = 0; iter < repeat; iter++)
  {
    for (size_t counter = 0; counter < worms_list_.size(); counter++)
    {
      Field::FieldType worm_type = Field::WORM;
      int              player2   = 2;
      if (worms_list_[counter]->getWormType() == player2)
      {
        worm_type = Field::WORM2;
      }

      // not needed
      int return_signal = 0;

      map_ = worms_list_[counter]->gravity(*this, worm_type, gravity_fall, map_,
                                           return_signal);
    }
  }
}

//------------------------------------------------------------------------------
Sep::Field::FieldType Sep::Game::getFieldType(int row, int col) const
{
  return map_[row][col];
}

//------------------------------------------------------------------------------
void Sep::Game::setFieldType(Field::FieldType type, int row, int col)
{
  map_[row][col] = type;
}

//------------------------------------------------------------------------------
Sep::Worm *Sep::Game::searchWorm(int row, int col)
{
  for (unsigned iterator = 0; iterator < worms_list_.size(); iterator++)
  {
    Sep::Worm *worm = getWorm(iterator);
    if (worm->getXposition() == col && worm->getYposition() == row)
    {
      return worm;
    }
  }
  return nullptr;
}

//------------------------------------------------------------------------------
Sep::Chest *Sep::Game::searchChest(int row, int col)
{
  for (unsigned iterator = 0; iterator < chest_list_.size(); iterator++)
  {
    Sep::Chest *chest = chest_list_[iterator];
    if (chest->getChestRow() == row && chest->getChestCol() == col
        && chest->getToDelete() == false)
    {
      return chest;
    }
  }
  return nullptr;
}

//------------------------------------------------------------------------------
void Sep::Game::makeAllChestsFallDown(int row, int col)
{
  // gravity check if field above is chest
  if (getFieldType(row, col) == Field::CHEST)
  {
    int temp_height = row;
    while (getFieldType(temp_height, col) == Field::CHEST)
    {
      Chest *chest = searchChest(temp_height, col);
      chest->makeChestFall(*this);
      temp_height--;
    }

    // recursive function called in case there is still an AIR field beneath
    // the chest nearest to the ground
    if (getFieldType(row + 1, col) == Field::AIR)
    {
      makeAllChestsFallDown(row + 2, col);
    }
  }
}
