//................................:: ZEST ::....................................
//
// signals.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_SIGNALS_HPP
#define ZEST_SIGNALS_HPP

#include <string>
#include <boost/signals2.hpp>
#include "reply.hpp"
#include "request.hpp"

namespace zest {
namespace server {

// Called when a reply is requested.
extern 
boost::signals2::signal<void (const request&, reply&)> request_sig;

// Called when a reply is completed.
extern 
boost::signals2::signal<void (const request&, reply&)> reply_sig;

} // namespace server
} // namespace zest

#endif // ZEST_SIGNALS_HPP
