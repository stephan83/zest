//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.hpp"
#include "reply.hpp"
#include "request.hpp"
#include "signals.hpp"
#include <iostream>
namespace zest {
namespace server {

request_handler::request_handler()
{
}

void request_handler::handle_request(const request& req, reply& rep)
{
  request_sig(req, rep);
}

} // namespace server
} // namespace zest
