//................................:: ZEST ::....................................
//
// router.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_ROUTER_HPP
#define ZEST_ROUTER_HPP

#include <string>
#include "request.hpp"
#include "reply.hpp"

namespace zest {
namespace server {

class router
{

public:

  // Process a request. 
  void process(const request& req, reply& rep);

private:

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool url_decode(const std::string& in, std::string& out);
};

} // namespace server
} // namespace zest

#endif // ZEST_ROUTER_HPP
