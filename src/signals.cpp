//................................:: ZEST ::....................................
//
// signals.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................


#include "signals.hpp"

namespace zest {
namespace server {

boost::signals2::signal<void (const request&, reply&)> request_sig;

boost::signals2::signal<void (const request&, reply&)> reply_sig;
  
boost::signals2::signal<void (route_ptr, const request&,
  reply&)> route_sig;

} // namespace server
} // namespace zest
