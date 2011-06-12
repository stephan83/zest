//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

///
/// Modified by Stephan Florquin:
///
/// * Hooked up the Zest router
/// * Added a callback when a connection is closed.
/// * Could add KeepAlive support, by is it worth it if it won't server static
///   files?
/// * Added log4cxx
///

#ifndef ZEST_SERVER_CONNECTION_HPP
#define ZEST_SERVER_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <log4cxx/logger.h>
#include "reply.hpp"
#include "request.hpp"
#include "router.hpp"
#include "request_parser.hpp"

namespace zest {
namespace server {

typedef boost::function<void ()> connection_closed_func;

/// Represents a single connection from a client.
class connection
  : public boost::enable_shared_from_this<connection>,
    private boost::noncopyable
{
public:
  /// Construct a connection with the given io_service.
  explicit connection(boost::asio::io_service& io_service,
      router_ptr& r, connection_closed_func closed_handler);

  /// Get the socket associated with the connection.
  boost::asio::ip::tcp::socket& socket();

  /// Start the first asynchronous operation for the connection.
  void start();

private:
  /// Handle completion of a read operation.
  void handle_read(const boost::system::error_code& e,
      std::size_t bytes_transferred);
  
  /// Handle when the reply is done.
  void on_reply(const request& req, reply& rep);

  /// Handle completion of a write operation.
  void handle_write(const boost::system::error_code& e);
  
  /// Access logger.
  static log4cxx::LoggerPtr access_logger_;

  /// Strand to ensure the connection's handlers are not called concurrently.
  boost::asio::io_service::strand strand_;

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;
  
  /// URL router.
  router_ptr router_;

  /// Buffer for incoming data.
  boost::array<char, 8192> buffer_;

  /// The incoming request.
  request request_;

  /// The parser for the incoming request.
  request_parser request_parser_;

  /// The reply to be sent back to the client.
  reply reply_;
  
  /// Close callback.
  connection_closed_func closed_handler_;
};

typedef boost::shared_ptr<connection> connection_ptr;

} // namespace server
} // namespace zest

#endif // ZEST_SERVER_CONNECTION_HPP
