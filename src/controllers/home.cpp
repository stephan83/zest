//................................:: ZEST ::....................................
//
// home.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include "home.hpp"
#include "reply.hpp"

namespace zest {
namespace server {
  
  const char html[] =
  "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\""
  " \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">"
  "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"fr\" lang=\"fr\">"
  "<head>"
  "<meta http-equiv=\"Content-Type\""
  " content=\"text/html; charset=utf-8\" />"
  "<title>Zest</title>"
  "</head>"
  "<body>"
  "<p>Hello, Internet.</p>"
  "</body>"
  "</html>";

void home_controller::add_actions()
{
  ZEST_ADD_ACTION(home_controller, index)
}

void home_controller::index(const request& req, json_var &params,
  response& resp, model_map& models)
{
  resp.status = reply::ok;
  resp.format = "html";
  
  resp.content << html;
}

} // namespace server
} // namespace zest
