//................................:: ZEST ::....................................
//
// form_parser.hpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#ifndef ZEST_FORM_PARSER_HPP
#define ZEST_FORM_PARSER_HPP

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include "json_var.hpp"

namespace zest {
namespace server {

struct request;

/// Parser for www-form-urlencoded content.
class form_parser
{
public:
  /// Construct ready to parse the form data.
  form_parser();

  /// Reset to initial parser state.
  void reset();

  /// Parse some data. The tribool return value is true when a complete request
  /// has been parsed, false if the data is invalid, indeterminate when more
  /// data is required. The InputIterator return value indicates how much of the
  /// input has been consumed.
  template <typename InputIterator>
  boost::tuple<boost::tribool, InputIterator> parse(json_var& variable,
      InputIterator begin, InputIterator end)
  {
    var_ = &variable;
    while (begin != end)
    {
      boost::tribool result = consume(variable, *begin++);
      if (result || !result)
        return boost::make_tuple(result, begin);
    }
    boost::tribool result = consume(variable, '\0');
    return boost::make_tuple(result, begin);
  }

private:
  /// Handle the next character of input.
  boost::tribool consume(json_var& variable, char input);

  /// Check if a byte is an HTTP character.
  static bool is_char(int c);

  static bool is_tspecial(int c);

  /// Check if a byte is a digit.
  static bool is_digit(int c);
  
  /// Current value.
  std::string value_;
  
  /// Current variable.
  json_var* var_;

  /// The current state of the parser.
  enum state
  {
    var_start,       // 0
    var,             // 1 
    property_start,  // 2
    property,        // 3
    property_end,    // 4
    value_start,     // 5
    value            // 6
  } state_;
};

} // namespace server
} // namespace zest

#endif // ZEST_FORM_PARSER_HPP
