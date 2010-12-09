//................................:: ZEST ::....................................
//
// route.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include <string>
#include <boost/foreach.hpp>
#include "route.hpp"

namespace zest {
namespace server {

route::route(const std::string& path)
  : path_(path)
{
  boost::regex esc("[\\^\\.\\$\\|\\(\\)\\[\\]\\{\\}\\*\\+\\?\\\\]");
  
  path_with_params_ = regex_replace(path, esc, "\\\\\\1&", boost::match_default
    | boost::format_sed);
}

route_ptr route::create(const std::string& path)
{
  return route_ptr(new route(path));
}

bool route::match(const std::string& path, json_var& object)
{
  boost::regex e(path_with_params_);
  
  boost::smatch values;
  
  bool result = boost::regex_match(path, values, e,
      boost::match_extra);
  
  if(result)
  {
    BOOST_FOREACH(param_option_map::value_type value, param_options_)
    {
      value.second->cast(values[value.first], object[value.first]);
    }
  }
  
  return result;
}

} // namespace server
} // namespace zest
