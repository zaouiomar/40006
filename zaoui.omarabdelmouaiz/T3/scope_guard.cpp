#include "scope_guard.hpp"

omar::StreamGuard::StreamGuard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  flags_(s.flags()),
  fill_(s.fill())
{}

omar::StreamGuard::~StreamGuard()
{
  s_.precision(precision_);
  s_.flags(flags_);
}