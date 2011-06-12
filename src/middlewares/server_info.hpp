//................................:: ZEST ::....................................
//
// server_info.hpp
// ~~~~~~~~~~~~~~
//
// Adds a date header to the response.
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_MIDDLEWARE_SERVER_INFO_HPP
#define ZEST_MIDDLEWARE_SERVER_INFO_HPP

#include "../middleware.hpp"

namespace zest {
namespace server {

///
/// Adds server info headers to the response.
///
class server_info_middleware
  : public middleware
{

public:
  
  void after_action(const request& req, json_var &params,
    reply& repl, response& resp)
  {
    repl.headers["Server"] = "zest";
    repl.headers["X-Powered-By"] = "lulz";
  }
  
};

} // namespace server
} // namespace zest

#endif // ZEST_MIDDLEWARE_SERVER_INFO_HPP
