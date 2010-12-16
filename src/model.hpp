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
#include "redisclient.hpp"
#include "json_var.hpp"
#include <iostream>
namespace zest {
namespace server {

typedef boost::shared_ptr<redis::client> redis_ptr;

///
/// Simple models based on using redis and json_var.
///
class model
  : public boost::enable_shared_from_this<model>
{

public:

  typedef std::vector<redis::command> command_vec;
  
  ///
  /// Base class for field types.
  ///
  class field
  {
  
  public:
  
    /// Called when a new model instance is created.
    virtual void create(json_var& var)
    {
    
    }
  
    /// Called when an instance is saved.
    virtual void save(const std::string& key, const json_var& var,
        redis_ptr redis)
    {
    
    }
    
    /// Called when an instance has to be loaded from the database.
    virtual json_var load(const std::string& key, redis_ptr redis)
    {
      return json_var(json_var::null_var);
    }
    
  };
  
  ///
  /// A string field type.
  ///
  class string_field
    : public field
  {
  
  public:
  
    void create(json_var& var)
    {
      var = json_var(json_var::string_var);
    }
  
    void save(const std::string& key, const json_var& var, redis_ptr redis)
    {
    
    }
    
    json_var load(const std::string& key, redis_ptr redis)
    {
      return json_var(json_var::null_var);
    }
  
  };
  
  ///
  /// A zset field type.
  ///
  class zset_field
    : public field
  {
  
  public:
  
    void create(json_var& var)
    {
      var = json_var(json_var::object_var);
    }
  
    void save(const std::string& key, const json_var& var, redis_ptr redis)
    {
      command_vec commands;
      json_var::object::const_iterator end = var.end();
      
      for(json_var::object::const_iterator beg = var.begin(); beg != end; ++beg)
      {
        const json_var::object::value_type& item = *beg;
        const json_var& value = item.second;
        
        if(value.dirty())
        {
          commands.push_back(
              redis::makecmd("ZADD") <<
              redis::key(key) <<
              value.to_double() <<
              item.first
          );
        }
      }
      
      if(commands.size())
      {
        redis->exec_transaction(commands);
      }
    }
    
    json_var load(const std::string& key, redis_ptr redis)
    {
      json_var result(json_var::object_var);
      
      command_vec commands;
      
      int size = redis->zcard(key);
      
      redis::client::string_score_vector values;
      
      redis->zrevrange(key, 0, size, values);
      
      BOOST_FOREACH(redis::client::string_score_vector::value_type value,
          values)
      {
        result[value.first] = value.second;
      }
      
      return result;
    }
  
  };

  typedef boost::shared_ptr<model> ptr;

  typedef boost::shared_ptr<field> field_ptr;

  typedef boost::unordered_map<std::string, field_ptr> field_map;

  /// Define a new model.
  static ptr define(const std::string& name, redis_ptr redis)
  {
    return ptr(new model(name, redis));
  }
  
  /// Specify the primary key field name.
  ptr primary_key(const std::string& field)
  {
    primary_key_ = field;
    
    return shared_from_this();
  }
  
  /// Add a field to the model.
  template <class T>
  ptr field(const std::string& name)
  {
    fields_[name] = field_ptr(new T());
    
    return shared_from_this();
  }
  
  /// Create a new model instance.
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
  
  /// Load a model instance from the database.
  template <class T>
  json_var load(T id)
  {
    json_var var(json_var::object_var);
    var[primary_key_] = id;
    
    BOOST_FOREACH(field_map::value_type field, fields_)
    {
      var[field.first] = field.second->load(key(var, field.first), redis_);
    }
    
    return var;
  }
  
  /// Load a model instance from the database or create it if it doesn't exist.
  template <class T>
  json_var load_or_create(T id)
  {
    return json_var(json_var::null_var);
  }
  
  /// Save model to the database.
  void save(json_var &var)
  {
    if(var.dirty())
    {
      BOOST_FOREACH(field_map::value_type field, fields_)
      {
        field.second->save(key(var, field.first), var[field.first], redis_);
      }
    }
  }
  
  /// Name of the model.
  const std::string& name()
  {
    return name_;
  }

private:

  /// Constuctor is private (use define instead).
  model(const std::string& name, redis_ptr redis)
    : name_(name),redis_(redis)
  {
    
  }
  
  /// Generate a key for a field.
  std::string key(const json_var &var, const std::string& field) const
  {
    std::string result = name_;
    result += ':';
    result += var[primary_key_].to_string();
    result += ':';
    result += field;
    return result;
  }
  
  /// Name of model.
  std::string name_;
  
  /// Hashtable of fields.
  field_map fields_;
  
  /// Primary key field name.
  std::string primary_key_;
  
  /// Redis client.
  redis_ptr redis_;

};

typedef boost::shared_ptr<model> model_ptr;

typedef boost::unordered_map<std::string, model_ptr> model_map;

} // namespace server
} // namespace zest

#endif // ZEST_MODEL_HPP
