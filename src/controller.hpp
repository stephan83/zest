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
#include "model.hpp"
#include "middleware.hpp"

namespace zest {
namespace server {

/// An action is simply a function pointer.
typedef boost::function<void (const request& req, json_var &params,
  response& response, model_map& models)> action;

///
/// Base class used by controllers.
///
class controller
{

public:

  /// Register actions.
  virtual void add_actions() = 0;

  /// Register an action.
  void add_action(const std::string& name, action a);
  
  /// Return an action by name.
  action get_action(const std::string& name);
  
  /// Handle an HTTP request.
  void handle_request(const request& req, json_var &params, reply& rep,
      const std::string& action_name, model_map& models,
        middleware_vec& middlewares);
    
private:

  typedef boost::unordered_map<std::string, action> action_map;
  
  /// Hashtable of actions.
  action_map actions_;
  
};

typedef boost::shared_ptr<controller> controller_ptr;

/// Macro to simplify registering a controller.
#define ZEST_ADD_ACTION(controller, action) \
  add_action(#action, boost::bind(&controller::action, shared_from_this(), \
      _1, _2, _3, _4));
     

} // namespace server
} // namespace zest

#endif // ZEST_CONTROLLER_HPP
