//................................:: ZEST ::....................................
//
// form_parser.cpp
// ~~~~~~~~~~~~~~
//
// Copyright 2010-2011 by Stephan Florquin.
//
//..............................................................................

#include <boost/lexical_cast.hpp>
#include "form_parser.hpp"
#include "utils.hpp"
//#include <iostream>
namespace zest {
namespace server {

form_parser::form_parser()
  : state_(var_start)
{
}

void form_parser::reset()
{
  state_ = var_start;
  value_.clear();
}

boost::tribool form_parser::consume(json_var& variable, char input)
{//std::cout << input << "<" << state_ << "\n";
  switch (state_)
  {
  case var_start:
    if (!is_char(input))
    {
      return false;
    }
    else if(input == '\0')
    {
      return true;
    }
    else
    {
      value_ += input;
      state_ = var;
      return boost::indeterminate;
    }
  case var:
    if (input == '=')
    {
      (*var_)[value_] = json_var(json_var::string_var);
      var_ = &(*var_)[value_];
      value_.clear();
      state_ = value_start;
      return boost::indeterminate;
    }
    else if (input == '[')
    {
      state_ = property_start;
      return boost::indeterminate;
    }
    else if (input == '&')
    {
      (*var_)[value_] = json_var(true);
      value_.clear();
      state_ = var_start;
      return boost::indeterminate;
    }
    else if(input == '\0')
    {
      (*var_)[value_] = json_var(true);
      return true;
    }
    else if (!is_char(input) || is_tspecial(input))
    {
      return false;
    }
    else
    {
      value_ += input;
      return boost::indeterminate;
    }
  case property_start:
    if (is_char(input) && !is_tspecial(input))
    {
      if((*var_)[value_].get_type() != json_var::object_var)
       (*var_)[value_] = json_var(json_var::object_var);
      var_ = &(*var_)[value_];
      value_.clear();
      value_ += input;
      state_ = property;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case property:
    if (input == ']')
    {
      state_ = property_end;
      return boost::indeterminate;
    }
    else if (is_char(input) && !is_tspecial(input))
    {
      value_ += input;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case property_end:
    if (input == '=')
    {
      (*var_)[value_] = json_var(json_var::string_var);
      var_ = &(*var_)[value_];
      value_.clear();
      state_ = value_start;
      return boost::indeterminate;
    }
    else if (input == '[')
    {
      state_ = property_start;
      return boost::indeterminate;
    }
    else if (input == '&')
    {
      (*var_)[value_] = json_var("true");
      value_.clear();
      var_ = &variable;
      state_ = var_start;
      return boost::indeterminate;
    }
    else if(input == '\0')
    {
      (*var_)[value_] = json_var("true");
      return true;
    }
    else
    {
      return false;
    }
  case value_start:
    if (input == '&')
    {
      return false;
    }
    else
    {
      value_ += input;
      state_ = value;
      return boost::indeterminate;
    }
  case value:
    if (input == '&')
    {
      //std::cout << value_ << '\n';
      std::string value;
      utils::url_decode(value_, value);
      (*var_) = value;
      value_.clear();
      var_ = &variable;
      state_ = var_start;
      return boost::indeterminate;
    }
    else if (input == '\0')
    {
      //std::cout << value_ << '\n';
      std::string value;
      utils::url_decode(value_, value);
      (*var_) = value;
      return true;
    }
    else
    {
      value_ += input;
      return boost::indeterminate;
    }
  default:
    return false;
  }
}

bool form_parser::is_tspecial(int c)
{
  switch (c)
  {
  case '[': case ']': case '=': case '&':
    return true;
  default:
    return false;
  }
}

bool form_parser::is_char(int c)
{
  return c >= 0 && c <= 127;
}

bool form_parser::is_digit(int c)
{
  return c >= '0' && c <= '9';
}

} // namespace server
} // namespace zest
