//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

///
/// Modified by Stephan Florquin:
///
/// * Hooked up Zest router.
/// * Tracking concurrent connections.
///

#ifndef ZEST_SERVER_SERVER_HPP
#define ZEST_SERVER_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.hpp"
#include "router.hpp"

namespace zest {
namespace server {

/// The top-level class of the HTTP server.
class server
  : private boost::noncopyable
{
public:
  /// Construct the server to listen on the specified TCP address and port.
  explicit server(const std::string& address, const std::string& port,
      std::size_t thread_pool_size, router_ptr r);

  /// Run the server's io_service loop.
  void run();

  /// Stop the server.
  void stop();

private:
  /// Handle completion of an asynchronous accept operation.
  void handle_accept(const boost::system::error_code& e);
  
  /// Handle termination of a connection.
  void connection_closed_handler();

  /// The number of threads that will call io_service::run().
  std::size_t thread_pool_size_;

  /// The io_service used to perform asynchronous operations.
  boost::asio::io_service io_service_;

  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  /// The URL router to map routes.
  router_ptr router_;

  /// The next connection to be accepted.
  connection_ptr new_connection_;
  
  /// Number of concurrent connections.
  unsigned int num_connections_;
};

} // namespace server
} // namespace zest

#endif // ZEST_SERVER_SERVER_HPP
