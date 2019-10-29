#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "Worm.h"

namespace Sep
{
  class Worm;

  //----------------------------------------------------------------------------
  // Player Class
  // class to model the players
  //
  class Player
  {
    private:
      int name_;
      std::vector<Worm *> worm_vector_;

    public:
      //------------------------------------------------------------------------
      // Default constructor
      //
      Player() : name_(PLAYER)
      {
      }

      //------------------------------------------------------------------------
      // Constructor
      //
      Player(const int &name);

      //------------------------------------------------------------------------
      // Copy constructor
      //
      Player(const Player&);

      //------------------------------------------------------------------------
      // Destructor
      //
      ~Player();

      //------------------------------------------------------------------------
      // Constants
      //
      static const int PLAYER;

      //------------------------------------------------------------------------
      // method to get the name of the player
      // @return 1 or 2
      //
      int getName() const;

      //------------------------------------------------------------------------
      // method to set the name of the player
      // @param name name of the player
      //
      void setName(int name);

      //------------------------------------------------------------------------
      // method to get a worm from the worm vector
      // @param counter index
      // @return pointer to this worm
      //
      Worm *getWormFromVector(int counter) const;

      //------------------------------------------------------------------------
      // method to set wor mto vector
      // @param worm worm to be set
      //
      void setWormToVector(Worm *worm);

      //------------------------------------------------------------------------
      // method to get worm from vector
      // @return vector of type worm pointer
      //
      std::vector<Worm *> getWormVector() const;

      //------------------------------------------------------------------------
      // method to set worm vector
      // @param worm_vector vector of worm pointer to be set
      //
      void setWormVector(std::vector<Worm*> worm_vector);
  };
}

#endif // PLAYER_H
