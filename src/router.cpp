//................................:: ZEST ::....................................
//
// router.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include <boost/algorithm/string.hpp>
#include "router.hpp"
#include "utils.hpp"
#include "form_parser.hpp"
//#include <iostream>

namespace zest {
namespace server {

void router::get(route_ptr r, router_func f)
{
  gets_.push_back(mapped_route(r, f));
}

void router::post(route_ptr r, router_func f)
{
  posts_.push_back(mapped_route(r, f));
}

void router::put(route_ptr r, router_func f)
{
  puts_.push_back(mapped_route(r, f));
}

void router::del(route_ptr r, router_func f)
{
  dels_.push_back(mapped_route(r, f));
}

void router::process(const request& req, reply& rep)
{
  std::string request_path;
  json_var params(json_var::object_var);
  form_parser parser;
  
  size_t get_pos = req.uri.find_first_of('?');
  
  //std::cout << req.uri << std::endl;
  if(get_pos < req.uri.size())
  {
    request_path = req.uri.substr(0, get_pos);
    
    boost::tribool result;
    boost::tie(result, boost::tuples::ignore) =
      parser.parse(params, req.uri.begin() + get_pos + 1, req.uri.end());
      
    if(!result)
    {
      reply _rep = reply::stock_reply(reply::bad_request);
      rep.status = _rep.status;
      rep.headers = _rep.headers;
      rep.content = _rep.content;
      return;
    }
  }
  else
  {
    request_path = req.uri;
  }
  
  if(req.content.size())
  {
    parser.reset();
    boost::tribool result;
    boost::tie(result, boost::tuples::ignore) =
      parser.parse(params, req.content.begin(), req.content.end());
      
    if(!result)
    {
      reply _rep = reply::stock_reply(reply::bad_request);
      rep.status = _rep.status;
      rep.headers = _rep.headers;
      rep.content = _rep.content;
      return;
    }
  }
  
  route_itr itr;
  route_itr end;
  
  if(boost::iequals(req.method, "get") || boost::iequals(req.method, "head"))
  {
    itr = gets_.begin();
    end = gets_.end();
  }
  else if(boost::iequals(req.method, "post"))
  {
    itr = posts_.begin();
    end = posts_.end();
  }
  else if(boost::iequals(req.method, "put"))
  {
    itr = puts_.begin();
    end = puts_.end();
  }
  else if(boost::iequals(req.method, "del"))
  {
    itr = dels_.begin();
    end = dels_.end();
  }
  
  while(itr != end)
  {
    mapped_route r = *itr;
    if(r.get<0>()->match(request_path, params))
    {      
      r.get<1>()(req, params, rep);
      
      //std::cout << params.to_json() << '\n';
      return;
      
      
    }
    ++itr;
  }
  
  // Didn't find a matching pattern.
  reply _rep = reply::stock_reply(reply::not_found);
  rep.status = _rep.status;
  rep.headers = _rep.headers;
  rep.content = _rep.content;
}

} // namespace server
} // namespace zest
