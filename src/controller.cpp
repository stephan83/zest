//................................:: ZEST ::....................................
//
// controller.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include <locale>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "controller.hpp"
#include "mime_types.hpp"
#include "version.hpp"
#include <boost/iostreams/filtering_stream.hpp>

namespace zest {
namespace server {

using namespace boost::posix_time;
using namespace boost::iostreams;

void controller::add_action(const std::string& name, action a)
{
  actions_[name] = a;
}

action controller::get_action(const std::string& name)
{
  return actions_[name];
}

void controller::handle_request(const request& req, json_var &params,
  reply& rep, const std::string& action_name, model_map& models)
{
  action a = get_action(action_name);
  response resp;
  std::ostringstream content;
  gzip_params g_params;
  //g_params.window_bits = -1;
  resp.content.push(gzip_compressor(g_params));
  resp.content.push(content);
  a(req, params, resp, models);
  
  /*resp.content.flush();
  content.flush();
  resp.content.strict_sync();*/
  resp.content.pop();
  resp.content.pop();
  rep.status = resp.status;
  
  if(!boost::iequals(req.method, "head"))
  {
    rep.content = content.str();
  }
  
  rep.headers.resize(8);
  
  time_facet* output_facet = new time_facet("%a, %d %b %Y %H:%M:%S GMT");
  std::ostringstream oss;
  oss.imbue(std::locale(std::locale::classic(), output_facet));
  ptime now = second_clock::universal_time();
  oss << now;
  
  rep.headers[0].name = "Date";
  rep.headers[0].value = oss.str();
  
  rep.headers[1].name = "Server";
  rep.headers[1].value = "zest ";
  rep.headers[1].value += version_string;
  
  rep.headers[2].name = "Content-Length";
  rep.headers[2].value = boost::lexical_cast<std::string>(rep.content.size());
  
  rep.headers[3].name = "Content-Type";
  rep.headers[3].value = zest::server::mime_types::extension_to_type(
    resp.format);
  
  rep.headers[4].name = "X-XSS-Protection";
  rep.headers[4].value = "X-XSS-Protection:1; mode=block";
  
  rep.headers[5].name = "X-Content-Type-Options";
  rep.headers[5].value = "nosniff";
  
  rep.headers[6].name = "X-Powered-By";
  rep.headers[6].value = "Vitamin C";
  //Accept-Encoding:gzip,deflate,sdch
  rep.headers[7].name = "Content-Encoding";
  rep.headers[7].value = "gzip";
}

} // namespace server
} // namespace zest
