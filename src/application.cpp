//................................:: ZEST ::....................................
//
// application.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include "application.hpp"

namespace zest {
namespace server {

void application::add_controller(const std::string& name, controller_ptr c)
{
  controllers_[name] = c;
}

controller_ptr application::get_controller(const std::string& name)
{
  return controllers_[name];
}

} // namespace server
} // namespace zest
