#ifndef SCOPE_GUARD_HPP
#define SCOPE_GUARD_HPP
#include <ios>

namespace omar
{
  class StreamGuard
  {
    public:
      explicit StreamGuard(std::basic_ios< char >& s);
      ~StreamGuard();

    private:
      std::basic_ios< char >& s_;
      std::streamsize width_;
      std::streamsize precision_;
      std::basic_ios< char >::fmtflags flags_;
      char fill_;
  };
}

#endif
