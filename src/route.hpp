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
#include "json_var.hpp"

namespace zest {
namespace server {

///
/// A router route.
///
class route
  : public boost::enable_shared_from_this<route>
{

public:

  typedef boost::shared_ptr<route> ptr;
  
  /// Create a new route.
  static ptr create(const std::string& path);
  
  /// Add a param to the route.
  template <typename Type>
  ptr add_param(const std::string& name, const std::string& pattern)
  {
    param_option_ptr option = param_option_ptr(
        new typed_param_option<Type>(pattern));
    param_options_[name] = option;
    
    boost::regex e(":" + name);
    
    std::string replace = "(?<" + name + ">" + pattern + ")";
    
    path_with_params_ = boost::regex_replace(path_with_params_, e, replace);
    
    regex_ = boost::regex(path_with_params_);

    return shared_from_this();
  }
  
  /// True if route matches a path.
  bool match(const std::string& path, json_var& object);

private:

  typedef boost::unordered_map<std::string, param_option_ptr>
    param_option_map;

  /// Private constructor.
  explicit route(const std::string& path);

  /// Route path.
  const std::string& path_;
  
  /// Route path with nested param regex.
  std::string path_with_params_;
  
  /// List of params.
  param_option_map param_options_;
  
  /// Final regex.
  boost::regex regex_;

};

typedef boost::shared_ptr<route> route_ptr;

} // namespace server
} // namespace zest

#endif // ZEST_ROUTE_HPP
