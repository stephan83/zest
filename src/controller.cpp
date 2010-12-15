//................................:: ZEST ::....................................
//
// controller.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include <iostream>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "controller.hpp"
#include "mime_types.hpp"

namespace zest {
namespace server {

using namespace boost::posix_time;
using namespace boost::iostreams;

void controller::add_action(const std::string& name, action a)
{
  actions_[name] = a;
}

action controller::get_action(const std::string& name)
{
  return actions_[name];
}

void controller::handle_request(const request& req, json_var &params,
    reply& rep, const std::string& action_name, model_map& models,
    middleware_vec& middlewares)
{
  response resp;

  BOOST_FOREACH(middleware_vec::value_type m, middlewares)
  {
    m->before_action(req, params, rep, resp);
  }

  std::ostringstream content;
  resp.content.push(content);
  action a = get_action(action_name);
  a(req, params, resp, models);
  
  BOOST_FOREACH(middleware_vec::value_type m, middlewares)
  {
    m->after_action(req, params, rep, resp);
  }
  
  resp.content.pop();
  rep.status = resp.status;
  
  if(!boost::iequals(req.method, "head"))
  {
    rep.content = content.str();
  }
  rep.headers["Content-Length"] =
    boost::lexical_cast<std::string>(rep.content.size());
  rep.headers["Content-Type"] = zest::server::mime_types::extension_to_type(
    resp.format);
}

} // namespace server
} // namespace zest
