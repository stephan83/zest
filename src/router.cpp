//................................:: ZEST ::....................................
//
// router.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include "router.hpp"
#include "form_parser.hpp"
#include <iostream>
namespace zest {
namespace server {

void router::map(route_ptr r, router_func f)
{
  routes_.push_back(mapped_route(r, f));
}

void router::process(const request& req, reply& rep)
{
  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    reply _rep = reply::stock_reply(reply::bad_request);
    rep.headers = _rep.headers;
    rep.content = _rep.content;
    return;
  }
  
  route_itr itr = routes_.begin();
  route_itr end = routes_.end();
  param_map params;
  
  while(itr != end)
  {
    mapped_route r = *itr;
    if(r.get<0>()->match(request_path, params))
    {
      form_parser parser;
      json_var var(json_var::object_var);
      boost::tribool result;
      boost::tie(result, boost::tuples::ignore) =
        parser.parse(var, req.content.begin(), req.content.end());
        
      if(!result)
      {
        reply _rep = reply::stock_reply(reply::bad_request);
        rep.headers = _rep.headers;
        rep.content = _rep.content;
        return;
      }
      
      std::cout << var.to_string() << '\n';
      
      r.get<1>()(req, params, rep);
      return;
    }
  
    params.clear();
    ++itr;
  }
  
  // Didn't find a matching pattern.
  reply _rep = reply::stock_reply(reply::not_found);
  rep.headers = _rep.headers;
  rep.content = _rep.content;
}

bool router::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace server
} // namespace zest
