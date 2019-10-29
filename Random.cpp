#include "Random.h"

#include <sstream>
#include <iostream>
#include <cstdlib>
#include <fstream>

using Sep::Random;

unsigned int Random::getSeed()
{
  bool seed_valid = false;
  char *rand_seed_from_env = getenv("RAND_SEED");
  unsigned int seed = 0;
  if (rand_seed_from_env)
  {
    std::stringstream rand_seed_sstream(rand_seed_from_env);
    rand_seed_sstream >> seed;
    if (!rand_seed_sstream.eof() || rand_seed_sstream.bad())
    {
      seed_valid = false;
      std::cerr << "[WARN] could not parse seed for RNG from environment\n";
    }
    else
    {
      seed_valid = true;
    }
  }
  if (!seed_valid)
  {
    std::random_device rand_dev;
    seed = rand_dev();
  }
  return seed;
}

std::vector<std::string> Random::getWormNames()
{
  std::ifstream file("worm_names.txt");
  std::vector<std::string> result;
  if (!file.is_open())
  {
    result.push_back("unknown");
    return result;
  }
  std::string line;
  while(std::getline(file, line))
  {
    result.push_back(line);
  }
  file.close();
  return result;
}

Random::Random() :
  mersenne_twister_(getSeed()),
  worm_names_(getWormNames())
{
}

int Random::getRandomInt(int lower, int upper)
{
  std::uniform_int_distribution<int> distribution(lower, upper);
  return distribution(mersenne_twister_);
}

std::string Random::getWormName()
{
  std::uniform_int_distribution<size_t> distribution(0, worm_names_.size() - 1);
  return worm_names_.at(distribution(mersenne_twister_));
}
