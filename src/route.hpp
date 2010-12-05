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
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>
#include <boost/any.hpp>
#include "request.hpp"
#include "reply.hpp"

namespace zest {
namespace server {

class abstract_param_option
{

public:
  
  virtual boost::any cast(const std::string& value) const
  {
    return 0;
  }
  
  const std::string& pattern() const
  {
    return pattern_;
  }
  
protected:

  std::string pattern_;
  
};

template<typename Type>
class param_option : public abstract_param_option
{

public:

  param_option(const std::string& p)
  {
    pattern_ = p;
  }
  
  boost::any cast(const std::string& value) const
  {
    return boost::lexical_cast<Type>(value);
  }
  
};

class route
  : public boost::enable_shared_from_this<route>
{

public:

  typedef boost::shared_ptr<route> ptr;
  
  static ptr create(const std::string& path);
  
  ptr add_param(const std::string& name, const abstract_param_option& param);
  
  bool match(const std::string& path) const;

private:

  explicit route(const std::string& path);

  const std::string& path_;
  
  std::string path_with_params_;
  
  boost::unordered_map<std::string, abstract_param_option> params_;

};

typedef boost::shared_ptr<route> route_ptr;

} // namespace server
} // namespace zest

#endif // ZEST_ROUTE_HPP
