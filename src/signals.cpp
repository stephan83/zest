//................................:: ZEST ::....................................
//
// signals.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................


#include "signals.hpp"
#include "reply.hpp"
#include "request.hpp"

namespace zest {
namespace server {

boost::signals2::signal<void (const request&, reply&)> request_sig;
boost::signals2::signal<void (const request&, reply&)> reply_sig;

} // namespace server
} // namespace zest
