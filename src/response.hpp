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
#include <boost/iostreams/filtering_stream.hpp>
#include "reply.hpp"

namespace zest {
namespace server {

///
/// Sort of a higher level HTTP reply, used by controller actions.
///
struct response
{
  /// Response status code.
  reply::status_type status;
  
  /// Response format (mime type).
  std::string format;
  
  /// Output stream.
  boost::iostreams::filtering_ostream content;
};

} // namespace server
} // namespace zest

#endif // ZEST_RESPONSE_HPP
