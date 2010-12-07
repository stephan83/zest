//................................:: ZEST ::....................................
//
// param.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_PARAM_HPP
#define ZEST_PARAM_HPP

#include <string>
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>
#include <boost/variant.hpp>
#include "request.hpp"
#include "reply.hpp"
#include "param.hpp"

namespace zest {
namespace server {

typedef boost::variant< int, float, double, std::string > param_value;
typedef boost::unordered_map<std::string, param_value> param_map;

class param_option
{

public:
  
  virtual param_value cast(const std::string& value) const
  {
    return value;
  }
  
  const std::string& pattern() const
  {
    return pattern_;
  }
  
protected:

  std::string pattern_;
  
};

template <typename Type>
class typed_param_option : public param_option
{

public:

  typed_param_option(const std::string& p)
  {
    pattern_ = p;
  }
  
  param_value cast(const std::string& value) const
  {
    return boost::lexical_cast<Type>(value);
  }
  
};

typedef boost::shared_ptr<param_option> param_option_ptr;

} // namespace server
} // namespace zest

#endif // ZEST_PARAM_HPP
