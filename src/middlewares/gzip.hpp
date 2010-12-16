//................................:: ZEST ::....................................
//
// gzip.hpp
// ~~~~~~~~~~~~~~
//
// Adds gzip compression to the response if available.
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_MIDDLEWARE_GZIP_HPP
#define ZEST_MIDDLEWARE_GZIP_HPP

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include "../middleware.hpp"

///
/// Enables gzip compression.
///
namespace zest {
namespace server {

class gzip_middleware
  : public middleware
{

public:
  
  void before_action(const request& req, json_var &params,
    reply& repl, response& resp)
  {
    if(accepts_gzip(req))
    {
      resp.content.push(boost::iostreams::gzip_compressor());
    }
  }
  
  void after_action(const request& req, json_var &params,
    reply& repl, response& resp)
  {
    if(accepts_gzip(req))
    {
      resp.content.pop();
      repl.headers["Content-Encoding"] = "gzip";
    }
  }
  
private:

  /// Return whether a client accepts gzip compression.
  static bool accepts_gzip(const request& req)
  {
    header_map::const_iterator itr = req.headers.find("Accept-Encoding");
    
    if(itr == req.headers.end())
    {
      return false;
    }
    
    const std::string& value = (*itr).second;
    size_t pos = value.find("gzip");
  
    return pos < value.size();
  }
  
};

} // namespace server
} // namespace zest

#endif // ZEST_MIDDLEWARE_GZIP_HPP
