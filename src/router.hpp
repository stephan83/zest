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
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>
#include "request.hpp"
#include "reply.hpp"
#include "route.hpp"
#include "json_var.hpp"

namespace zest {
namespace server {

typedef boost::function<void (const request& req, json_var &params,
  reply& rep)> router_func;

class router
  : public boost::enable_shared_from_this<router>
{

public:

  /// Map a route.
  void map(route_ptr r, router_func f);

  /// Process a request. 
  void process(const request& req, reply& rep);

private:

  typedef boost::tuple<route_ptr, router_func> mapped_route;

  typedef std::vector<mapped_route> route_vec;
  
  typedef route_vec::iterator route_itr;
  
  route_vec routes_;
  
};

typedef boost::shared_ptr<router> router_ptr;

} // namespace server
} // namespace zest

#endif // ZEST_ROUTER_HPP
