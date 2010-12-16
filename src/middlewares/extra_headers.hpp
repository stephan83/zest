//................................:: ZEST ::....................................
//
// extra_headers.hpp
// ~~~~~~~~~~~~~~
//
// Adds a date header to the response.
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_MIDDLEWARE_EXTRA_HEADERS_HPP
#define ZEST_MIDDLEWARE_EXTRA_HEADERS_HPP

#include "../middleware.hpp"

namespace zest {
namespace server {

///
/// Adds various headers to the response.
///
class extra_headers_middleware
  : public middleware
{

public:
  
  void after_action(const request& req, json_var &params,
    reply& repl, response& resp)
  {
    repl.headers["X-XSS-Protection"] = "X-XSS-Protection:1; mode=block";
    repl.headers["X-Content-Type-Options"] = "nosniff";
  }
  
};

} // namespace server
} // namespace zest

#endif // ZEST_MIDDLEWARE_EXTRA_HEADERS_HPP
