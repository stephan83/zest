//
// header.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ZEST_SERVER_HEADER_HPP
#define ZEST_SERVER_HEADER_HPP

#include <string>

namespace zest {
namespace server {

struct header
{
  std::string name;
  std::string value;
};

} // namespace server
} // namespace zest

#endif // ZEST_SERVER_HEADER_HPP
