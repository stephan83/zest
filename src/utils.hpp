//................................:: ZEST ::....................................
//
// utils.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_UTILS_HPP
#define ZEST_UTILS_HPP

#include <string>

namespace zest {
namespace server {

///
/// Useful functions.
///
class utils
{

public:

  /// Decode a URL.
  static bool url_decode(const std::string& in, std::string& out);
  
  /// Escape double quotes.
  static std::string escape(const std::string& in);

};

} // namespace server
} // namespace zest

#endif // ZEST_UTILS_HPP
