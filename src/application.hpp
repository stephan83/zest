//................................:: ZEST ::....................................
//
// application.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_APPLICATION_HPP
#define ZEST_APPLICATION_HPP

#include <string>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "router.hpp"
#include "controller.hpp"
#include "model.hpp"
#include "middleware.hpp"

namespace zest {
namespace server {

///
/// Base class used by Zest applications.
///
class application
{

public:

  /// Add command line / configuration file options.
  virtual void add_options(boost::program_options::options_description &o) = 0;
  
  /// Map router URLs.
  virtual void map_routes(router_ptr r) = 0;
  
  /// Define the models used by the application.
  virtual void define_models() = 0;
  
  /// Add middlewares used by the application.
  virtual void add_middlewares() = 0;
  
  /// Handle an HTTP request.
  void handle_request(const request& req, json_var &params,
    reply& reply, const std::string& c, const std::string& a);
  
protected:

  /// Register a controller.
  void add_controller(const std::string& name, controller_ptr c);
  
  /// Returns a controller by name.
  controller_ptr get_controller(const std::string& name);

  /// Register a model.
  void add_model(model_ptr m);

  /// Get a model by name.
  model_ptr get_model(const std::string& name);

  /// Register a middleware.
  void add_middleware(middleware_ptr m);
  
private:

  typedef boost::unordered_map<std::string, controller_ptr> controller_map;
  
  /// Hashtable of controllers.
  controller_map controllers_;
  
  /// Hashtable of models.
  model_map models_;
  
  /// Vector of middleware.
  middleware_vec middlewares_;
  
};

typedef boost::shared_ptr<application> application_ptr;

/// Macro to simplify registering a controller.

#define ZEST_ADD_CONTROLLER(name) \
  add_controller(#name, controller_ptr(new name())); \
  get_controller(#name)->add_actions();
  
/// Macro to simplify binding (most likely a route) to a controller action.
  
#define ZEST_BIND_ACTION(controller_name, action) \
  boost::bind(&application::handle_request, this, \
    _1, _2, _3, #controller_name, #action)

} // namespace server
} // namespace zest

#endif // ZEST_APPLICATION_HPP
