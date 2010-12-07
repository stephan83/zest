//................................:: ZEST ::....................................
//
// route.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_ROUTE_HPP
#define ZEST_ROUTE_HPP

#include <string>
#include <boost/regex.hpp>
#include <boost/unordered_map.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/regex.hpp>
#include <boost/unordered_map.hpp>
#include "request.hpp"
#include "reply.hpp"
#include "param.hpp"

namespace zest {
namespace server {

class route
  : public boost::enable_shared_from_this<route>
{

public:

  typedef boost::shared_ptr<route> ptr;
  
  static ptr create(const std::string& path);
  
  template <typename Type>
  ptr add_param(const std::string& name, const std::string& pattern)
  {
    param_option_ptr option = param_option_ptr(
        new typed_param_option<Type>(pattern));
    param_options_[name] = option;
    
    boost::regex e(":" + name);
    
    std::string replace = "(?<" + name + ">" + pattern + ")";
    
    path_with_params_ = boost::regex_replace(path_with_params_, e, replace);

    return shared_from_this();
  }
  
  bool match(const std::string& path, param_map& params);

private:

  typedef boost::unordered_map<std::string, param_option_ptr>
    param_option_map;

  explicit route(const std::string& path);

  const std::string& path_;
  
  std::string path_with_params_;
  
  param_option_map param_options_;

};

typedef boost::shared_ptr<route> route_ptr;

} // namespace server
} // namespace zest

#endif // ZEST_ROUTE_HPP
