#include "scope_guard.hpp"

omar::iofmtguard::iofmtguard(std::basic_ios< char >& s):
  s_(s),
  fmt_(s.flags()),
  width_(s.width()),
  precision_(s.precision()),
  fill_(s.fill())
{}

omar::iofmtguard::~iofmtguard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
