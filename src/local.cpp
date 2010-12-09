//................................:: ZEST ::....................................
//
// local.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include "local.hpp"
#include "controllers/rate.hpp"
#include <iostream>
namespace zest {
namespace server {

local::local()
{
  ZEST_ADD_CONTROLLER(rate_controller)
}

void local::add_options(boost::program_options::options_description &o)
{
  o.add_options()
      ("redis-servers,r",
        boost::program_options::value<std::string>(&redis_servers_)
          ->default_value("127.0.0.1:6379:1"), "set redis servers");
}

void local::map_routes(router_ptr r)
{
  r->map(zest::server::route::create("/:subject/rates/:object.:format")
        ->add_param<std::string>("subject", "[^/.]{1,255}")
        ->add_param<std::string>("object", "[^/.]{1,255}")
        ->add_param<std::string>("format", "[a-z0-9]{3,5}"),
          ZEST_BIND_ACTION(rate_controller, show));
}

void local::define_models()
{
  model_ptr entity = model::define("entity")
      ->primary_key("uid")
      ->field<model::zset_field>("rates")
      ->field<model::zset_field>("rated")
  ;
  
  json_var test = entity->create("stephan");
  test["rates"]["mix6355"] = .5f;
  
  std::cout << test.to_json() << '\n';
}

} // namespace server
} // namespace zest