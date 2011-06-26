//................................:: ZEST ::....................................
//
// home.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_CONTROLLER_HOME_HPP
#define ZEST_CONTROLLER_HOME_HPP

#include "../controller.hpp"

namespace zest {
namespace server {

class home_controller
  : public controller,
    public boost::enable_shared_from_this<home_controller>
{

public:
  
  void add_actions();
  
  void index(const request& req, json_var &params, response& resp, 
      model_map& models);
  
};

} // namespace server
} // namespace zest

#endif // ZEST_CONTROLLER_HOME_HPP
