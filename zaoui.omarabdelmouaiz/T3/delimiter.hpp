#ifndef DELIMITER_HPP
#define DELIMITER_HPP
#include <iostream>

namespace omar
{
  struct DelimiterIO
  {
    char expected;
  };

  std::istream& operator>>(std::istream& in, const DelimiterIO&& dest);
}

#endif
