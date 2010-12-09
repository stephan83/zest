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

namespace zest {
namespace server {

class application
{

public:

  virtual void add_options(boost::program_options::options_description &o) = 0;
  
  virtual void map_routes(router_ptr r) = 0;
  
  virtual void define_models() = 0;
  
protected:

  void add_controller(const std::string& name, controller_ptr c);
  
  controller_ptr get_controller(const std::string& name);
  
private:

  typedef boost::unordered_map<std::string, controller_ptr> controller_map;
  
  controller_map controllers_;
  
};

typedef boost::shared_ptr<application> application_ptr;

#define ZEST_ADD_CONTROLLER(name) \
  add_controller(#name, controller_ptr(new name())); \
  get_controller(#name)->add_actions();
  
#define ZEST_BIND_ACTION(controller_name, action) \
  boost::bind(&controller::handle_request, get_controller(#controller_name), \
    _1, _2, _3, #action)

} // namespace server
} // namespace zest

#endif // ZEST_APPLICATION_HPP