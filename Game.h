#ifndef SEP_GAME_H
#define SEP_GAME_H

#include <fstream>
#include <string>
#include <vector>

#include "Chest.h"
#include "Command.h"
#include "Field.h"
#include "Player.h"
#include "Random.h"
#include "Worm.h"

namespace Sep
{
  class Chest;
  class Command;
  class Player;
  class Worm;

  //----------------------------------------------------------------------------
  // Game class
  // class used to steer and connect all game-relevant parts
  //
  class Game
  {
    private:
      int board_width_;
      int board_height_;
      std::vector<std::vector<Field::FieldType> > map_;
      bool running_state_;
      Sep::Worm *actual_worm_;

    public:
      //------------------------------------------------------------------------
      // Constructor
      //
      Game();

      //------------------------------------------------------------------------
      // Destructor
      //
      ~Game();

      //------------------------------------------------------------------------
      // method to ignore comments, extra spaces and after .that we save the
      // size content and map content to their prospective variables. We also
      // check the validity of string 'MAP:' and 'SIZE: '
      // @param 2d vector string type file_input_vec
      // @param string size_content
      // @param 2d vector string &map_content
      // @return RETURN_SUCCESS
      // @return RETURN_INVALID_CONFIG
      //
      int ignoreKomentSpacesEnters(std::vector<std::string> file_input_vec,
        std::string &size, std::vector<std::string> &map_content);

      //------------------------------------------------------------------------
      // method to get the parameters from SIZE: and convert them
      // from string type to int type.
      // @param string size_content
      // @param int &real_height
      // @param int &real_width
      // @return RETURN_SUCCESS
      // @return RETURN_INVALID_CONFIG
      //
      int sizeParamGet(std::string size_content, int &real_height,
        int &real_width);

      //------------------------------------------------------------------------
      // method to check if the content of the map is valid and set
      // board_width_, board_height_, map_ to their right values.
      // @param vector string map_content
      // @param int real_height
      // @param int real_width
      // @return RETURN_SUCCESS
      // @return RETURN_INVALID_CONFIG
      //
      int mapContentChecker(std::vector<std::string> map_content,
        int real_height, int real_width);

      //------------------------------------------------------------------------
      // method to read and parse the input file and after that to set the
      // specific private variables like board_width_, board_height_, map_.
      // @param string cfg_file
      // @return RETURN_SUCCESS
      // @return RETURN_INVALID_CONFIG
      //
      int loadConfig(std::string cfg_file);

      //------------------------------------------------------------------------
      // method to add worms on the specific field coordinates if possible
      // @param int row
      // @param int col
      // @return RETURN_SUCCESS
      // @return RETURN_INVALID_CONFIG
      //
      int addWorm(int row, int col, int player);

      //------------------------------------------------------------------------
      // method used to print upper and lower boarder of the map
      //
      void printMapHelper();

      //------------------------------------------------------------------------
      // method used to print the actual game map
      //
      void printMap();

      //------------------------------------------------------------------------
      // method to simulate the right-side movement of the worm
      // @param int row
      // @param int col
      // @param int steps
      // @return (just return from void function)
      //
      void rightMovement(int row, int col, int steps);

      //------------------------------------------------------------------------
      // method to simulate the left-side movement of the worm
      // @param int row
      // @param int col
      // @param int steps
      // @return (just return from void function)
      //
      void leftMovement(int row, int col, int steps);

      //------------------------------------------------------------------------
      // method to simulate the movement of the worm on the field
      // @param int row
      // @param int col
      // @param int steps
      //
      void move(int row, int col, int steps);

      //------------------------------------------------------------------------
      // method to transform from FieldType to a char type
      // (EARTH -> 'E')
      // @param field_type
      // @return water
      // @return earth
      // @return air
      // @return worm
      // @return RETURN_SUCCESS
      //
      char getCharacter(Field::FieldType field_type);

      //------------------------------------------------------------------------
      // Vector of commands
      //
      typedef std::vector<Command *> CmdList;
      CmdList commands_list_;

      //------------------------------------------------------------------------
      // Active player variable
      //
      int player_active_;

      //------------------------------------------------------------------------
      // Vector of Player objects
      //
      typedef std::vector<Player *> Players;
      Players players_list_;


      //------------------------------------------------------------------------
      // Player pointer to player who is taking a turn
      //
      Player *actual_player_;

      //------------------------------------------------------------------------
      // method to set player who is taking the next turn
      // @param actual_player player who is taking the next turn
      //
      void setActualPlayer(Player *actual_player);

      //------------------------------------------------------------------------
      // method to get the player who is taking the turn
      // @return pointer to this player
      //
      Player *getActualPlayer() const;


      //------------------------------------------------------------------------
      // Vector of Worms objects
      //
      typedef std::vector<Worm *> Worms;
      Worms worms_list_;

      //------------------------------------------------------------------------
      // method to parse the input
      // @param command_parameter parameters of the input
      // @param base_command the base command
      // @return RETURN_SUCCESS if parsed successfully
      //
      int parseInput(std::vector<std::string> &command_parameter,
        std::string &base_command);

      //------------------------------------------------------------------------
      // method to split a string
      // @param string_to_parse string to be parsed
      // @return vector containing splitted string
      //
      std::vector<std::string> customSplittString(const std::string
        &string_to_parse);

      //------------------------------------------------------------------------
      // method to set runme
      // @param runme_state state to be set
      //
      void setStateRunning(bool runme_state);

      //------------------------------------------------------------------------
      // method to generate random x coordinates
      // @return vector of x coordinates
      //
      std::vector<int> randomXCoordVector();

      //------------------------------------------------------------------------
      // method to initialize the worm field for the players
      // @param player1 pointer to player 1
      // @param player2 pointer to player 2
      //
      void wormsFieldBegin(Player *player1, Player *player2);

      //------------------------------------------------------------------------
      // method to active runme and set all necessary attributes
      // @param cfg_file name of config file
      // @return SUCCESS if runme successfully started
      //
      int runme(std::string cfg_file);

      //------------------------------------------------------------------------
      // method to add new players
      // @param player to be added
      //
      void addPlayer(Sep::Player *player);

      //------------------------------------------------------------------------
      // method to get active player
      // @return active player
      //
      int getActivePlayer() const;

      //------------------------------------------------------------------------
      // method to get player
      // @param active_player number of player
      // @return pointer to player
      //
      Sep::Player *getPlayer(int active_player) const;

      //------------------------------------------------------------------------
      // method to set active player
      // @param active_player number of player that should be active
      //
      void setActivePlayer(int active_player);

      //------------------------------------------------------------------------
      // method to get worm from vector
      // @param active_worm index
      // @return Worm pointer
      //
      Sep::Worm *getWorm(int active_worm) const;

      //------------------------------------------------------------------------
      // method to print a list of worms
      //
      void printWormList();

      //------------------------------------------------------------------------
      // method to get width of board
      // @return board width
      //
      int getBoardWidth();

      //------------------------------------------------------------------------
      // method to get height of board
      // @return board height
      //
      int getBoardHeight();

      //------------------------------------------------------------------------
      // method to get map
      // @return map
      //
      std::vector<std::vector<Field::FieldType> > getMap();

      //------------------------------------------------------------------------
      // method to get actual worm
      // @return Worm pointer
      //
      Sep::Worm *getActualWorm() const;

      //------------------------------------------------------------------------
      // method to set actual worm
      // @param actual_worm worm to be set as actual
      //
      void setActualWorm(Sep::Worm *actual_worm);

      //------------------------------------------------------------------------
      // method to apply gravity to worms
      //
      void gameGravity();

      //------------------------------------------------------------------------
      // method to get the type of a certain field
      // @param row of the map
      // @param col of the map
      // @return field type
      //
      Field::FieldType getFieldType(int row, int col) const;

      //------------------------------------------------------------------------
      // method to set type of the field
      // @param type type to be set
      // @param row of the field
      // @param col of the field
      //
      void setFieldType(Field::FieldType type, int row, int col);

      //------------------------------------------------------------------------
      // method to search for a certain worm
      // @param row row of the worm
      // @param col of the worm
      // @return Worm pointer if worm exists, otherwise nullptr
      //
      Sep::Worm *searchWorm(int row, int col);

      //------------------------------------------------------------------------
      // Random instance
      //
      Random random_;

      //------------------------------------------------------------------------
      // Vector of chests
      //
      typedef std::vector<Chest *> Chests;
      Chests chest_list_;

      //------------------------------------------------------------------------
      // method to search for a certain chest
      // @param row row of the chest
      // @param col of the chest
      // @return Chest pointer if chest exists, otherwise nullptr
      //
      Sep::Chest *searchChest(int row, int col);

      //------------------------------------------------------------------------
      // method to apply gravity to all chests in a column
      // @param row of the chest
      // @param col of the chest
      //
      void makeAllChestsFallDown(int row, int col);
  };
}

#endif //SEP_GAME_H
