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
    
void rate_controller::show(const request& req, param_map &params,
  response& resp)
{
  resp.status = reply::ok;
  resp.format << params["format"];
  
  if(resp.format.str() == "json")
  {
    resp.content
      << "{\n"
        << "  \"subject\": \"" << params["subject"] << "\",\n"
        << "  \"object\": \""  << params["object"]  << "\",\n"
        << "  \"format\": \""  << params["format"]  << "\"\n"
      << "}\n"
    ;
  }
  else if(resp.format.str() == "html")
  {
    resp.content
      << "<ul>"
        << "<li>" << params["subject"] << "</li>"
        << "<li>" << params["object"]  << "</li>"
        << "<li>" << params["format"]  << "</li>"
      << "</ul>"
    ;
  }
}

} // namespace server
} // namespace zest
