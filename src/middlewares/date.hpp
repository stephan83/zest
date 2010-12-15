//................................:: ZEST ::....................................
//
// date.hpp
// ~~~~~~~~~~~~~~
//
// Adds a date header to the response.
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_MIDDLEWARE_DATE_HPP
#define ZEST_MIDDLEWARE_DATE_HPP

#include <locale>
#include <iostream>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../middleware.hpp"

namespace zest {
namespace server {

class date_middleware
  : public middleware
{

public:
  
  void after_action(const request& req, json_var &params,
    reply& repl, response& resp)
  {
    using namespace boost::posix_time;
    using namespace boost::iostreams;

    time_facet* output_facet = new time_facet("%a, %d %b %Y %H:%M:%S GMT");
    std::ostringstream oss;
    oss.imbue(std::locale(std::locale::classic(), output_facet));
    ptime now = second_clock::universal_time();
    oss << now;
    
    repl.headers["Date"] = oss.str();
  }
  
};

} // namespace server
} // namespace zest

#endif // ZEST_MIDDLEWARE_DATE_HPP
