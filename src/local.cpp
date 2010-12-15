//................................:: ZEST ::....................................
//
// local.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include "local.hpp"
#include "controllers/rate.hpp"
#include "middlewares/date.hpp"
#include "middlewares/gzip.hpp"
#include "middlewares/server_info.hpp"
#include "middlewares/extra_headers.hpp"
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
  r->get(zest::server::route::create("/:subject/rates/:object.:format")
        ->add_param<std::string>("subject", "[^/.]{1,255}")
        ->add_param<std::string>("object", "[^/.]{1,255}")
        ->add_param<std::string>("format", "[a-z0-9]{3,5}"),
          ZEST_BIND_ACTION(rate_controller, show));
          
  r->post(zest::server::route::create("/:subject/rates/:object.:format")
        ->add_param<std::string>("subject", "[^/.]{1,255}")
        ->add_param<std::string>("object", "[^/.]{1,255}")
        ->add_param<std::string>("format", "[a-z0-9]{3,5}"),
          ZEST_BIND_ACTION(rate_controller, show));
}

void local::define_models()
{
  redis_ = parse_redis_servers(redis_servers_);

  model_ptr entity = model::define("entity", redis_)
      ->primary_key("uid")
      ->field<model::zset_field>("rates")
      ->field<model::zset_field>("rated")
  ;
  
  json_var test = entity->load("stephan");
  
  /*
  
  json_var options(json_var::object_var);
  
  options["rated"] = json_var(json_var::null_var);
  options["rates"] = json_var(json_var::object_var);
  options["rates"]["order"] = "desc";
  options["rates"]["start"] = 0;
  options["rates"]["end"] = 10;
  
  json_var test = entity->load("stephan", options);
  
  */
  
  std::cout << test.to_json() << '\n';
}

void local::add_middlewares()
{
  add_middleware(middleware_ptr(new server_info_middleware()));
  add_middleware(middleware_ptr(new date_middleware()));
  add_middleware(middleware_ptr(new gzip_middleware()));
  add_middleware(middleware_ptr(new extra_headers_middleware()));
}

redis_ptr local::parse_redis_servers(const std::string options) const
{
  std::vector<redis::connection_data> redis_servers;
  
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep1(",");
  boost::char_separator<char> sep2(":");
  tokenizer tok1(options, sep1);
  
  for(tokenizer::iterator beg1 = tok1.begin(); beg1 != tok1.end(); ++beg1)
  {
    tokenizer tok2(*beg1, sep2);
    
    for(tokenizer::iterator beg2 = tok2.begin(); beg2 != tok2.end(); ++beg2)
    {
      redis::connection_data con;
      con.host = *beg2;
      con.port = boost::lexical_cast<uint16_t>(*++beg2);
      con.dbindex = boost::lexical_cast<int>(*++beg2);
      redis_servers.push_back(con);
    }
  }
  
  return redis_ptr(new redis::client(redis_servers.begin(),
      redis_servers.end()));
}

} // namespace server
} // namespace zest
