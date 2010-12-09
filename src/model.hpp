//................................:: ZEST ::....................................
//
// model.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_MODEL_HPP
#define ZEST_MODEL_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>
#include "json_var.hpp"

namespace zest {
namespace server {

class model
  : public boost::enable_shared_from_this<model>
{

public:
  
  class field
  {
  
  public:
  
    virtual void create(json_var& var)
    {
    
    }
  
    virtual void save(const std::string& key, const json_var& var)
    {
    
    }
    
    json_var load(const std::string& key)
    {
      return json_var(json_var::null_var);
    }
    
  };
  
  class string_field
    : public field
  {
  
  public:
  
    void create(json_var& var)
    {
      var = json_var(json_var::string_var);
    }
  
    void save(const std::string& key, const json_var& var)
    {
    
    }
    
    json_var load(const std::string& key)
    {
      return json_var(json_var::string_var);
    }
  
  };
  
  class zset_field
    : public field
  {
  
  public:
  
    void create(json_var& var)
    {
      var = json_var(json_var::object_var);
    }
  
    void save(const std::string& key, const json_var& var)
    {
    
    }
    
    json_var load(const std::string& key)
    {
      return json_var(json_var::object_var);
    }
  
  };

  typedef boost::shared_ptr<model> ptr;

  typedef boost::shared_ptr<field> field_ptr;

  typedef boost::unordered_map<std::string, field_ptr> field_map;

  static ptr define(const std::string& name)
  {
    return ptr(new model(name));
  }
  
  ptr primary_key(const std::string& field)
  {
    primary_key_ = field;
    
    return shared_from_this();
  }
  
  template <class T>
  ptr field(const std::string& name)
  {
    fields_[name] = field_ptr(new T());
    
    return shared_from_this();
  }
  
  template <class T>
  json_var create(T id)
  {
    json_var var(json_var::object_var);
    var[primary_key_] = id;
    
    BOOST_FOREACH(field_map::value_type field, fields_)
    {
      field.second->create(var[field.first]);
    }
    
    return var;
  }
  
  template <class T>
  json_var load(T id)
  {
    return json_var(json_var::null_var);
  }
  
  template <class T>
  json_var load_or_create(T id)
  {
    return json_var(json_var::null_var);
  }
  
  void save(json_var &var)
  {
    if(var.dirty())
    {
      BOOST_FOREACH(field_map::value_type field, fields_)
      {
        field.second->save(key(var, field.first), var[field.first]);
      }
    }
  }
  
  const std::string& name()
  {
    return name_;
  }

private:

  model(const std::string& name)
    : name_(name)
  {
    
  }
  
  std::string key(const json_var &var, const std::string& field) const
  {
    std::string result = name_;
    result += ':';
    result += var[primary_key_].to_string();
    result += ':';
    result += field;
    return result;
  }
  
  std::string name_;
  
  field_map fields_;
  
  std::string primary_key_;

};

typedef boost::shared_ptr<model> model_ptr;

typedef boost::unordered_map<std::string, model_ptr> model_map;

} // namespace server
} // namespace zest

#endif // ZEST_MODEL_HPP
