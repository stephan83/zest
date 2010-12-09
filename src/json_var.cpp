//................................:: ZEST ::....................................
//
// json_var.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include <boost/foreach.hpp>
#include "json_var.hpp"

namespace zest {
namespace server {

json_var nil;

const std::string nil_str = "null";

json_var::json_var()
{
  value_ = 0;
  type_ = null_var;
}

json_var::json_var(type t)
{
  switch(t)
  {
    case string_var:
    value_ = std::string("");
    break;
    case object_var:
    value_ = object();
    break;
    case array_var:
    value_ = array();
    break;
    case null_var:
    value_ = 0;
    break;
  }
  
  type_ = t;
}

json_var::json_var(const std::string& value)
{
  value_ = value;
  type_ = string_var;
}

json_var::json_var(const char* value)
{
  value_ = std::string(value);
  type_ = string_var;
}

json_var::json_var(const object& value)
{
  value_ = value;
  type_ = object_var;
}

json_var::json_var(const array& value)
{
  value_ = value;
  type_ = array_var;
}

std::string json_var::to_string()
{
  if(type_ == string_var)
  {
    return *boost::any_cast<std::string>(&value_);
  }
  else if(type_ == object_var)
  {
    object obj = to_object();
    std::string result = "[";
    
    BOOST_FOREACH(object::value_type element, obj)
    {
      result += element.first;
      result += " => ";
      if(element.second.type_ == string_var) result += '"';
      result += element.second.to_string();
      if(element.second.type_ == string_var) result += '"';
      result += "; ";
    }
    
    result += ']';
    
    return result;
  }
  else if(type_ == array_var)
  {
    array arr = to_array();
    std::string result = "[";
    
    BOOST_FOREACH(array::value_type element, arr)
    {
      if(element.type_ == string_var) result += '"';
      result += element.to_string();
      if(element.type_ == string_var) result += '"';
      result += "; ";
    }
    
    result += ']';
    
    return result;
  }
  else
  {
    return nil_str;
  }
}

json_var& json_var::operator=(const std::string& value)
{
  if(this != &nil)
  {
    value_ = value;
    type_ = string_var;
  }
  return *this;
}

json_var& json_var::operator=(const char* value)
{
  if(this != &nil)
  {
    value_ = std::string(value);
    type_ = string_var;
  }
  return *this;
}

json_var& json_var::operator=(const object& value)
{
  if(this != &nil)
  {
    value_ = value;
    type_ = object_var;
  }
  return *this;
}

json_var& json_var::operator=(const array& value)
{
  if(this != &nil)
  {
    value_ = value;
    type_ = array_var;
  }
  return *this;
}

json_var& json_var::operator[](const std::string& property)
{
  if(type_ == object_var)
  {
    return to_object()[property];
  }
  else
  {
    return nil;
  }
}

json_var& json_var::operator[](size_t index)
{
  if(type_ == array_var)
  {
    return to_array()[index];
  }
  else
  {
    return nil;
  }
}

json_var& json_var::operator+=(const std::string& value)
{
  if(type_ == string_var)
  {
    (*boost::any_cast<std::string>(&value_)) += value;
  }
  return *this;
}

json_var& json_var::operator+=(const char* value)
{
  if(type_ == string_var)
  {
    (*boost::any_cast<std::string>(&value_)) += value;
  }
  return *this;
}

json_var& json_var::operator+=(char value)
{
  if(type_ == string_var)
  {
    (*boost::any_cast<std::string>(&value_)) += value;
  }
  return *this;
}

json_var::object& json_var::to_object()
{
  return *boost::any_cast<object>(&value_);
}

json_var::array& json_var::to_array()
{
  return *boost::any_cast<array>(&value_);
}

const json_var::object& json_var::to_object() const
{
  return *boost::any_cast<object>(&value_);
}

const json_var::array& json_var::to_array() const
{
  return *boost::any_cast<array>(&value_);
}

json_var::type json_var::get_type() const
{
  return type_;
}

} // namespace server
} // namespace zest
