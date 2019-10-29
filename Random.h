#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <vector>
#include <string>

namespace Sep
{
  //------------------------------------------------------------------------------
  // Random class
  // generates random numbers and worm names
  //
  class Random final
  {
  private:
    std::mt19937 mersenne_twister_;
    const std::vector<std::string> worm_names_;

    //--------------------------------------------------------------------------
    // returns an integer suitable to seed the random number generator
    //
    static unsigned int getSeed();

    //--------------------------------------------------------------------------
    // returns a vector of possible worm names
    //
    static std::vector<std::string> getWormNames();

  public:
    //--------------------------------------------------------------------------
    // Constructs a new Random instance
    //
    Random();

    //--------------------------------------------------------------------------
    // deleted copy constructor
    //
    Random(const Random& other) = delete;

    //--------------------------------------------------------------------------
    // deleted assignment operator
    //
    Random& operator=(const Random& rhs) = delete;

    //--------------------------------------------------------------------------
    // returns a random integer between lower and upper (inclusive)
    //
    int getRandomInt(int lower, int upper);

    //--------------------------------------------------------------------------
    // returns a random name for a worm
    //
    std::string getWormName();
  };
}

#endif
