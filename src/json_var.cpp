//................................:: ZEST ::....................................
//
// json_var.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include <boost/lexical_cast.hpp>
#include "json_var.hpp"
#include "utils.hpp"

namespace zest {
namespace server {

json_var nil;

const std::string nil_str = "null";
const std::string true_str = "true";
const std::string false_str = "false";

json_var::json_var()
  : dirty_(false)
{
  value_ = 0;
  type_ = null_var;
}

json_var::json_var(type t)
  : dirty_(false)
{
  switch(t)
  {
    case string_var:
    value_ = std::string("");
    break;
    case int_var:
    value_ = 0;
    break;
    case double_var:
    value_ = .0f;
    break;
    case bool_var:
    value_ = false;
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
  : dirty_(false)
{
  value_ = value;
  type_ = string_var;
}

json_var::json_var(const char* value)
  : dirty_(false)
{
  value_ = std::string(value);
  type_ = string_var;
}

json_var::json_var(int value)
  : dirty_(false)
{
  value_ = value;
  type_ = int_var;
}

json_var::json_var(double value)
  : dirty_(false)
{
  value_ = value;
  type_ = double_var;
}

json_var::json_var(bool value)
  : dirty_(false)
{
  value_ = value;
  type_ = bool_var;
}

json_var::json_var(const object& value)
  : dirty_(false)
{
  value_ = value;
  type_ = object_var;
}

json_var::json_var(const array& value)
  : dirty_(false)
{
  value_ = value;
  type_ = array_var;
}

std::string json_var::to_string() const
{
  if(type_ == string_var)
  {
    return *boost::any_cast<std::string>(&value_);
  }
  
  return to_json();
}

std::string json_var::to_json() const
{
  if(type_ == string_var)
  {
    std::string result = "\"";
    result += utils::escape(*boost::any_cast<std::string>(&value_));
    result += "\"";
    
    return result;
  }
  else if(type_ == int_var)
  {
    std::string result = boost::lexical_cast<std::string>(
      *boost::any_cast<int>(&value_));
    return result;
  }
  else if(type_ == double_var)
  {
    std::string result = boost::lexical_cast<std::string>(
      *boost::any_cast<double>(&value_));
    return result;
  }
  else if(type_ == bool_var)
  {
    if(*boost::any_cast<bool>(&value_))
    {
      return true_str;
    }
    return false_str;
  }
  else if(type_ == object_var)
  {
    object obj = to_object();
    std::string result = "{";
    object::iterator begin = obj.begin();
    object::iterator end = obj.end();
    
    while(begin != end)
    {
      object::value_type& element = *begin;
      
      result += '"';
      result += utils::escape(element.first);
      result += "\": ";
      result += element.second.to_json();
    
      ++begin;
    
      if(begin != end)
      {
        result += ", ";
      }
    }
    
    result += '}';
    
    return result;
  }
  else if(type_ == array_var)
  {
    array arr = to_array();
    std::string result = "[";
    array::iterator begin = arr.begin();
    array::iterator end = arr.end();
    
    while(begin != end)
    {
      array::value_type& element = *begin;
      
      result += element.to_json();
    
      ++begin;
    
      if(begin != end)
      {
        result += ", ";
      }
    }
    
    result += ']';
    
    return result;
  }
  else
  {
    return nil_str;
  }
}

std::string json_var::to_html() const
{
  if(type_ == string_var)
  {
    return *boost::any_cast<std::string>(&value_);
  }
  else if(type_ == int_var)
  {
    std::string result = boost::lexical_cast<std::string>(
      *boost::any_cast<int>(&value_));
    return result;
  }
  else if(type_ == double_var)
  {
    std::string result = boost::lexical_cast<std::string>(
      *boost::any_cast<double>(&value_));
    return result;
  }
  else if(type_ == bool_var)
  {
    if(*boost::any_cast<bool>(&value_))
    {
      return true_str;
    }
    return false_str;
  }
  else if(type_ == object_var)
  {
    object obj = to_object();
    std::string result;
    object::iterator begin = obj.begin();
    object::iterator end = obj.end();
    
    while(begin != end)
    {
      object::value_type& element = *begin;
      
      result += "<div class=\"";
      result += utils::escape(element.first);
      result += "\">";
      result += element.second.to_html();
      result += "</div>";
      
      ++begin;
    }
    
    return result;
  }
  else if(type_ == array_var)
  {
    array arr = to_array();
    std::string result = "<ul>";
    array::iterator begin = arr.begin();
    array::iterator end = arr.end();
    
    while(begin != end)
    {
      array::value_type& element = *begin;
      
      result += "<li>";
      result += element.to_html();
      result += "</li>";
    
      ++begin;
    }
    
    result += "</ul>";
    
    return result;
  }
  else
  {
    return nil_str;
  }
}

int json_var::to_int() const
{
  if(type_ == int_var)
  {
    return *boost::any_cast<int>(&value_);
  }
  
  return 0;
}

double json_var::to_double() const
{
  if(type_ == double_var)
  {
    return *boost::any_cast<double>(&value_);
  }
  
  return .0f;
}

bool json_var::to_bool() const
{
  if(type_ == bool_var)
  {
    return *boost::any_cast<bool>(&value_);
  }
  
  return false;
}

json_var& json_var::operator=(const std::string& value)
{
  if(this != &nil)
  {
    value_ = value;
    type_ = string_var;
    dirty_ = true;
  }
  return *this;
}

json_var& json_var::operator=(const char* value)
{
  if(this != &nil)
  {
    value_ = std::string(value);
    type_ = string_var;
    dirty_ = true;
  }
  return *this;
}

json_var& json_var::operator=(int value)
{
  if(this != &nil)
  {
    value_ = value;
    type_ = int_var;
    dirty_ = true;
  }
  return *this;
}

json_var& json_var::operator=(double value)
{
  if(this != &nil)
  {
    value_ = value;
    type_ = double_var;
    dirty_ = true;
  }
  return *this;
}

json_var& json_var::operator=(bool value)
{
  if(this != &nil)
  {
    value_ = value;
    type_ = bool_var;
    dirty_ = true;
  }
  return *this;
}

json_var& json_var::operator=(const object& value)
{
  if(this != &nil)
  {
    value_ = value;
    type_ = object_var;
    dirty_ = true;
  }
  return *this;
}

json_var& json_var::operator=(const array& value)
{
  if(this != &nil)
  {
    value_ = value;
    type_ = array_var;
    dirty_ = true;
  }
  return *this;
}

json_var& json_var::operator[](const std::string& property)
{
  if(type_ == object_var)
  {
    dirty_ = true;
    return to_object()[property];
  }
  else
  {
    return nil;
  }
}

const json_var& json_var::operator[](const std::string& property) const
{
  if(type_ == object_var)
  {
    const object& obj = to_object();
    object::const_iterator itr =  obj.find(property);
    
    if(itr == obj.end())
    {
      return nil;
    }
    
    return (*itr).second;
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

const json_var& json_var::operator[](size_t index) const
{
  if(type_ == array_var)
  {
    const array& arr = to_array();
    
    if(index < arr.size())
    {
      return arr[index];
    }
    
    return nil;
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
    dirty_ = true;
  }
  return *this;
}

json_var& json_var::operator+=(const char* value)
{
  if(type_ == string_var)
  {
    (*boost::any_cast<std::string>(&value_)) += value;
    dirty_ = true;
  }
  return *this;
}

json_var& json_var::operator+=(char value)
{
  if(type_ == string_var)
  {
    (*boost::any_cast<std::string>(&value_)) += value;
    dirty_ = true;
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

bool json_var::dirty() const
{
  return dirty_;
}

json_var::object::iterator json_var::begin()
{
  return to_object().begin();
}

json_var::object::iterator json_var::end()
{
  return to_object().end();
}

json_var::object::const_iterator json_var::begin() const
{
  return to_object().begin();
}

json_var::object::const_iterator json_var::end() const
{
  return to_object().end();
}


} // namespace server
} // namespace zest
