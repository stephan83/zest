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
#include "request.hpp"
#include "reply.hpp"
#include "route.hpp"

namespace zest {
namespace server {

class router
  : public boost::enable_shared_from_this<router>
{

public:

  /// Map a route.
  void map(route_ptr r);

  /// Process a request. 
  void process(const request& req, reply& rep);

private:

  typedef boost::shared_ptr<boost::regex> regex_ptr;

  /// A route.
  struct route
  {
    explicit route(const std::string &p, regex_ptr e)
      : pattern(p), regex(e)
    {
    
    }
    
    std::string pattern;
    regex_ptr regex;
  };
    
  /// A vector of routes.
  typedef std::vector<route_ptr> routes;

  /// A routes iterator.
  typedef routes::iterator routes_itr;

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool url_decode(const std::string& in, std::string& out);
  
  /// Vector of routes.
  routes routes_;
  
};

typedef boost::shared_ptr<router> router_ptr;

} // namespace server
} // namespace zest

#endif // ZEST_ROUTER_HPP
