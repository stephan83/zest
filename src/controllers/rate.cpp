//................................:: ZEST ::....................................
//
// rate.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include "rate.hpp"

namespace zest {
namespace server {

void rate_controller::add_actions()
{
  ZEST_ADD_ACTION(rate_controller, show)
}
    
void rate_controller::show(const request& req, param_map &params,
  response& resp)
{
  resp.format << params["format"];
  
  resp.content
    << "<ul>"
      << "<li>" << params["subject"] << "</li>"
      << "<li>" << params["object"]  << "</li>"
      << "<li>" << params["format"]  << "</li>"
    << "</ul>"
  ;
}

} // namespace server
} // namespace zest
