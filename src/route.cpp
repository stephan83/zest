//................................:: ZEST ::....................................
//
// route.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include <string>
#include <iostream>
#include <boost/foreach.hpp>
#include "route.hpp"

namespace zest {
namespace server {

route::route(const std::string& path)
  : path_(path), path_with_params_(path)
{

}

route_ptr route::create(const std::string& path)
{
  return route_ptr(new route(path));
}

route_ptr route::add_param(const std::string& name,
  const abstract_param_option& option)
{
  params_[name] = option;
  
  boost::regex e(":" + name);
  
  std::string pattern = "(?<" + name + ">" + option.pattern() + ")";
  
  path_with_params_ = boost::regex_replace(path_with_params_, e, pattern);
std::cout << path_with_params_ << '\n';
  return shared_from_this();
}

bool route::match(const std::string& path) const
{
  boost::regex e(path_with_params_);
  boost::smatch values;
  
  boost::regex_match(path, values, e,
      boost::match_extra);
      
  std::cout << values["format"] << std::endl;
  
  /*while(itr != end)
  {
    std::cout << (*itr).suffix() << '\n';
    ++itr;
  }*/
      
  return false;
}

} // namespace server
} // namespace zest
