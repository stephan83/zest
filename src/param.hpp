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
#include "json_var.hpp"

namespace zest {
namespace server {

typedef boost::variant< int, float, bool, std::string > param_value;

class param_option
{

public:

  param_option()
  {
  }

  param_option(const std::string& p)
  {
    pattern_ = p;
  }
  
  virtual void cast(const std::string& in, json_var& out) const
  {
    out = in;
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
  
  void cast(const std::string& in, json_var& out) const
  {
    out = boost::lexical_cast<Type>(in);
  }
  
};

typedef boost::shared_ptr<param_option> param_option_ptr;

} // namespace server
} // namespace zest

#endif // ZEST_PARAM_HPP
