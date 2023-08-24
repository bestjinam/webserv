#include "Utils.hpp"
#include <sstream>

std::string intToString(int num)
{
  std::stringstream sstream(num);
  return sstream.str();
}
