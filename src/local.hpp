//................................:: ZEST ::....................................
//
// local.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_LOCAL_HPP
#define ZEST_LOCAL_HPP

#include <string>
#include "application.hpp"
#include "router.hpp"
#include "request.hpp"
#include "reply.hpp"
#include "param.hpp"

namespace zest {
namespace server {

class local
  : public application
{

public:

  explicit local();

  void add_options(boost::program_options::options_description &o);

  void map_routes(router_ptr r);
  
  void define_models();
  
  void add_middlewares();
    
private:

  redis_ptr parse_redis_servers(const std::string options) const;

  std::string redis_servers_;
  
  redis_ptr redis_;
  
};

} // namespace server
} // namespace zest

#endif // ZEST_LOCAL_HPP
