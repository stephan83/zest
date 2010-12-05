//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ZEST_SERVER_REQUEST_HANDLER_HPP
#define ZEST_SERVER_REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>

namespace zest {
namespace server {

struct reply;
struct request;

/// The common handler for all incoming requests.
class request_handler
  : private boost::noncopyable
{
public:
  /// Construct with a directory containing files to be served.
  explicit request_handler();

  /// Handle a request and produce a reply.
  void handle_request(const request& req, reply& rep);

private:
};

} // namespace server
} // namespace zest

#endif // ZEST_SERVER_REQUEST_HANDLER_HPP
