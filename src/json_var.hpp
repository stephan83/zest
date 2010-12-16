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

///
/// Emulates a Javascript variable.
///
class json_var
{

public:

  /// Type of the variable.
  enum type
  {
    string_var,
    int_var,
    double_var,
    bool_var,
    object_var,
    array_var,
    null_var
  };

  typedef boost::unordered_map<std::string, json_var> object;
  typedef std::vector<json_var> array;
  
  /// Constructors.
  explicit json_var();
  explicit json_var(type t);
  explicit json_var(const std::string& value);
  explicit json_var(const char* value);
  explicit json_var(int value);
  explicit json_var(double value);
  explicit json_var(bool value);
  explicit json_var(const object& value);
  explicit json_var(const array& value);
  
  /// String representation of the value.
  std::string to_string() const;
  
  /// JSON representation of the value.
  std::string to_json() const;
  
  /// HTML representation of the value.
  std::string to_html() const;
  
  /// Get value as int.
  int to_int() const;
  
  /// Get value as double.
  double to_double() const;
  
  /// Get value as bool.
  bool to_bool() const;
  
  /// Assignement operators.
  json_var& operator=(const std::string& value);
  json_var& operator=(const char* value);
  json_var& operator=(int value);
  json_var& operator=(double value);
  json_var& operator=(bool value);
  json_var& operator=(const object& value);
  json_var& operator=(const array& value);
  
  /// Random access operators.
  json_var& operator[](const std::string& property);
  const json_var& operator[](const std::string& property) const;
  json_var& operator[](size_t index);
  const json_var& operator[](size_t index) const;
  
  /// Add-to operators.
  json_var& operator+=(const std::string& value);
  json_var& operator+=(const char* value);
  json_var& operator+=(char value);
  
  /// Get value type.
  type get_type() const;
  
  /// True if value changed.
  bool dirty() const;
  
  /// Iterators.
  object::iterator begin();
  object::iterator end();
  object::const_iterator begin() const;
  object::const_iterator end() const;
  
private:

  /// Cast to object var.
  object& to_object();
  const object& to_object() const;
  
  /// Cast to array var.
  array& to_array();
  const array& to_array() const;
  
  /// Current value.
  boost::any value_;
  
  /// Current type.
  type type_;
  
  /// Has value changed? Used for ORM.
  bool dirty_;

};

} // namespace server
} // namespace zest

#endif // ZEST_JSON_VAR_HPP
