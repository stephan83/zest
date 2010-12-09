//................................:: ZEST ::....................................
//
// rate.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include "rate.hpp"
#include "reply.hpp"

namespace zest {
namespace server {

void rate_controller::add_actions()
{
  ZEST_ADD_ACTION(rate_controller, show)
}
    
void rate_controller::show(const request& req, json_var &params,
  response& resp)
{
  resp.status = reply::ok;
  resp.format = params["format"].to_string();
  
  if(resp.format == "json")
  {
    resp.content << params.to_json();
  }
  else if(resp.format == "html")
  {
    resp.content << params.to_html();
  }
}

} // namespace server
} // namespace zest
