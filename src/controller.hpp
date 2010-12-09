//................................:: ZEST ::....................................
//
// controller.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_CONTROLLER_HPP
#define ZEST_CONTROLLER_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
#include <boost/bind.hpp>
#include "router.hpp"
#include "request.hpp"
#include "reply.hpp"
#include "json_var.hpp"
#include "response.hpp"

namespace zest {
namespace server {

typedef boost::function<void (const request& req, json_var &params,
  response& response)> action;

class controller
{

public:

  virtual void add_actions() = 0;

  void add_action(const std::string& name, action a);
  
  action get_action(const std::string& name);
  
  void handle_request(const request& req, json_var &params, reply& rep,
      const std::string& action_name);
    
private:

  typedef boost::unordered_map<std::string, action> action_map;
  
  action_map actions_;
  
};

typedef boost::shared_ptr<controller> controller_ptr;

#define ZEST_ADD_ACTION(controller, action) \
  add_action(#action, boost::bind(&controller::action, shared_from_this(), \
      _1, _2, _3));

} // namespace server
} // namespace zest

#endif // ZEST_CONTROLLER_HPP
