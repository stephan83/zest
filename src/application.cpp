//................................:: ZEST ::....................................
//
// application.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include "application.hpp"

namespace zest {
namespace server {

void application::add_controller(const std::string& name, controller_ptr c)
{
  controllers_[name] = c;
}

controller_ptr application::get_controller(const std::string& name)
{
  return controllers_[name];
}

void application::add_model(model_ptr m)
{
  models_[m->name()] = m;
}

model_ptr application::get_model(const std::string& name)
{
  return models_[name];
}

void application::add_middleware(middleware_ptr m)
{
  middlewares_.push_back(m);
}

void application::handle_request(const request& req, json_var &params,
    reply& reply, const std::string& c, const std::string& a)
{
  get_controller(c)->handle_request(req, params, reply, a, models_,
      middlewares_);
}

} // namespace server
} // namespace zest
