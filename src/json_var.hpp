//................................:: ZEST ::....................................
//
// json_var.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_JSON_VAR_HPP
#define ZEST_JSON_VAR_HPP

#include <string>
#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/any.hpp>

namespace zest {
namespace server {

class json_var
{

public:

  enum type
  {
    string_var,
    int_var,
    float_var,
    bool_var,
    object_var,
    array_var,
    null_var
  };

  typedef boost::unordered_map<std::string, json_var> object;
  typedef std::vector<json_var> array;
  
  explicit json_var();
  
  explicit json_var(type t);
  
  explicit json_var(const std::string& value);
  
  explicit json_var(const char* value);
  
  explicit json_var(int value);
  
  explicit json_var(float value);
  
  explicit json_var(bool value);
  
  explicit json_var(const object& value);
  
  explicit json_var(const array& value);
  
  std::string to_string();
  
  std::string to_json();
  
  json_var& operator=(const std::string& value);
  
  json_var& operator=(const char* value);
  
  json_var& operator=(int value);
  
  json_var& operator=(float value);
  
  json_var& operator=(bool value);
  
  json_var& operator=(const object& value);
  
  json_var& operator=(const array& value);
  
  json_var& operator[](const std::string& property);

  json_var& operator[](size_t index);
  
  json_var& operator+=(const std::string& value);
  
  json_var& operator+=(const char* value);
  
  json_var& operator+=(char value);
  
  type get_type() const;
  
private:

  object& to_object();
  
  array& to_array();
  
  const object& to_object() const;
  
  const array& to_array() const;
  
  boost::any value_;
  
  type type_;

};

} // namespace server
} // namespace zest

#endif // ZEST_JSON_VAR_HPP
