//................................:: ZEST ::....................................
//
// utils.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
#include "utils.hpp"

namespace zest {
namespace server {

bool utils::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

const boost::regex escape_e("[\\\"\\\\]");
const std::string escape_r("\\\\\\1&");

std::string utils::escape(const std::string& in)
{
  return regex_replace(in, escape_e, escape_r, boost::match_default |
      boost::format_sed);
}

} // namespace server
} // namespace zest
