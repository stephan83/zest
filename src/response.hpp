//................................:: ZEST ::....................................
//
// controller.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_RESPONSE_HPP
#define ZEST_RESPONSE_HPP

#include <iostream>
#include "reply.hpp"

namespace zest {
namespace server {

struct response
{
  reply::status_type status;
  std::ostringstream format;
  std::ostringstream content;
};

} // namespace server
} // namespace zest

#endif // ZEST_RESPONSE_HPP
