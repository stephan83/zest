//................................:: ZEST ::....................................
//
// controller.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include "controller.hpp"
#include "mime_types.hpp"

namespace zest {
namespace server {

void controller::add_action(const std::string& name, action a)
{
  actions_[name] = a;
}

action controller::get_action(const std::string& name)
{
  return actions_[name];
}

void controller::handle_request(const request& req, json_var &params,
  reply& rep, const std::string& action_name)
{
  action a = get_action(action_name);
  response resp;
  
  
  a(req, params, resp);
  
  rep.status = resp.status;
  
  rep.content = resp.content.str();
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
  
  
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = zest::server::mime_types::extension_to_type(
    resp.format);
}

} // namespace server
} // namespace zest
