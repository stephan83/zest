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
#include <boost/unordered_map.hpp>

namespace zest {
namespace server {

typedef boost::unordered_map <std::string, std::string> header_map;

} // namespace server
} // namespace zest

#endif // ZEST_SERVER_HEADER_HPP
