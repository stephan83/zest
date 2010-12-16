//................................:: ZEST ::....................................
//
// middleware.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_MIDDLEWARE_HPP
#define ZEST_MIDDLEWARE_HPP

#include <boost/shared_ptr.hpp>
#include "request.hpp"
#include "json_var.hpp"
#include "reply.hpp"
#include "response.hpp"

namespace zest {
namespace server {

///
/// Base class used by middlewares.
///
class middleware
{

public:
  
  /// Do something before an action is called.
  virtual void before_action(const request& req, json_var &params,
    reply& repl, response& resp)
  {
  
  }
  
  /// Do something after an action is called.
  virtual void after_action(const request& req, json_var &params,
    reply& repl, response& resp)
  {
  
  }
  
};

typedef boost::shared_ptr<middleware> middleware_ptr;

typedef std::vector<middleware_ptr> middleware_vec;

} // namespace server
} // namespace zest

#endif // ZEST_MIDDLEWARE_HPP
