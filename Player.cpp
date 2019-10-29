#include "Player.h"

namespace Sep
{
  //----------------------------------------------------------------------------
  const int Player::PLAYER = 1;

  //----------------------------------------------------------------------------
  Player::Player(const int &name) : name_(name)
  {
  }

  //----------------------------------------------------------------------------
  Player::Player(const Player &src) : name_(src.name_)
  {
  }

  //----------------------------------------------------------------------------
  Player::~Player()
  {
  }

  //----------------------------------------------------------------------------
  int Player::getName() const
  {
    return name_;
  }

  //----------------------------------------------------------------------------
  void Player::setName(int name)
  {
    name_ = name;
  }

  //----------------------------------------------------------------------------
  Worm *Player::getWormFromVector(int counter) const
  {
    return worm_vector_[counter];
  }

  //----------------------------------------------------------------------------
  void Player::setWormToVector(Worm *worm)
  {
    worm_vector_.push_back(worm);
  }

  //----------------------------------------------------------------------------
  std::vector<Worm *> Player::getWormVector() const
  {
    return worm_vector_;
  }

  //----------------------------------------------------------------------------
  void Player::setWormVector(std::vector<Worm *> worm_vector)
  {
    worm_vector_ = worm_vector;
  }
}
