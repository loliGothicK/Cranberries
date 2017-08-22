/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_EXCEPTION_HPP
#define CRANBERRIES_EXCEPTION_HPP
#include <exception>
#include <stdexcept>
#include <string>

#define CRANBERRIES_FULL_FUNCSIG
#ifndef CRANBERRIES_FULL_FUNCSIG
#define CRANBERRIES_FUNCTION __FUNCTION__
#elif _MSC_VER
#define CRANBERRIES_FUNCTION __FUNCSIG__
#else
#define CRANBERRIES_FUNCTION __PRETTY_FUNCTION__
#endif

#define CRANBERRIES_RUNTIME_ERROR_THROW_IF( COND ) if( COND ){ throw cranberries::runtime_error("runtime_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_ELSE_IF( COND ) else if( COND ){ throw cranberries::runtime_error("runtime_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_ELSE else{ throw cranberries::runtime_error("runtime_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_WITH_MSG_IF( COND, MESSAGE ) if( COND ){ throw cranberries::runtime_error("runtime_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_WITH_MSG_ELSE_IF( COND, MESSAGE ) else if( COND ){ throw cranberries::runtime_error("runtime_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::runtime_error("runtime_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::runtime_error("runtime_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} )
#define CRANBERRIES_RUNTIME_ERROR_THROW_CONDITIONAL( COND ) ( COND ) ? throw cranberries::runtime_error( "runtime_error", __FILE__, __FUNCTION__, __LINE__, #COND )
#define CRANBERRIES_RUNTIME_ERROR_THROW_CONDITIONAL_WITH_MSG( COND, MESSAGE ) ( COND ) ? throw cranberries::runtime_error( "runtime_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE )

#define CRANBERRIES_RANGE_ERROR_THROW_IF( COND ) if( COND ){ throw cranberries::range_error( "range_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_RANGE_ERROR_THROW_ELSE_IF( COND ) else if( COND ){ throw cranberries::range_error( "range_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_RANGE_ERROR_THROW_ELSE else{ throw cranberries::range_error( "range_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_RANGE_ERROR_THROW_WITH_MSG_IF( COND, MESSAGE ) if( COND ){ throw cranberries::range_error( "range_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_RANGE_ERROR_THROW_WITH_MSG_ELSE_IF( COND, MESSAGE ) else if( COND ){ throw cranberries::range_error( "range_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_RANGE_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::range_error( "range_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_RANGE_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::range_error( "range_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} )
#define CRANBERRIES_RANGE_ERROR_THROW_CONDITIONAL( COND ) ( COND ) ? throw cranberries::range_error( "range_error", __FILE__, __FUNCTION__, __LINE__, #COND )
#define CRANBERRIES_RANGE_ERROR_THROW_CONDITIONAL_WITH_MSG( COND, MESSAGE ) ( COND ) ? throw cranberries::range_error( "range_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE )

#define CRANBERRIES_OVERFLOW_ERROR_THROW_IF( COND ) if( COND ){ throw cranberries::overflow_error( "overflow_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_OVERFLOW_ERROR_THROW_ELSE_IF( COND ) else if( COND ){ throw cranberries::overflow_error( "overflow_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_OVERFLOW_ERROR_THROW_ELSE else{ throw cranberries::overflow_error( "overflow_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_OVERFLOW_ERROR_THROW_WITH_MSG_IF( COND, MESSAGE ) if( COND ){ throw cranberries::overflow_error( "overflow_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_OVERFLOW_ERROR_THROW_WITH_MSG_ELSE_IF( COND, MESSAGE ) else if( COND ){ throw cranberries::overflow_error( "overflow_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_OVERFLOW_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::overflow_error( "overflow_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_OVERFLOW_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::overflow_error( "overflow_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} )
#define CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( COND ) ( COND ) ? throw cranberries::overflow_error( "overflow_error", __FILE__, __FUNCTION__, __LINE__, #COND )
#define CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL_WITH_MSG( COND, MESSAGE ) ( COND ) ? throw cranberries::overflow_error( "overflow_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE )

#define CRANBERRIES_UNDERFLOW_ERROR_THROW_IF( COND ) if( COND ){ throw cranberries::underflow_error( "underflow_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_UNDERFLOW_ERROR_THROW_ELSE_IF( COND ) else if( COND ){ throw cranberries::underflow_error( "underflow_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_UNDERFLOW_ERROR_THROW_ELSE else{ throw cranberries::underflow_error( "underflow_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_UNDERFLOW_ERROR_THROW_WITH_MSG_IF( COND, MESSAGE ) if( COND ){ throw cranberries::underflow_error( "underflow_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_UNDERFLOW_ERROR_THROW_WITH_MSG_ELSE_IF( COND, MESSAGE ) else if( COND ){ throw cranberries::underflow_error( "underflow_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_UNDERFLOW_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::underflow_error( "underflow_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_UNDERFLOW_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::underflow_error( "underflow_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} )
#define CRANBERRIES_UNDERFLOW_ERROR_THROW_CONDITIONAL( COND ) ( COND ) ? throw cranberries::underflow_error( "underflow_error", __FILE__, __FUNCTION__, __LINE__, #COND )
#define CRANBERRIES_UNDERFLOW_ERROR_THROW_CONDITIONAL_WITH_MSG( COND, MESSAGE ) ( COND ) ? throw cranberries::underflow_error( "underflow_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE )

#define CRANBERRIES_LOGIC_ERROR_THROW_IF( COND ) if( COND ){ throw cranberries::logic_error( "logic_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_ELSE_IF( COND ) else if( COND ){ throw cranberries::logic_error( "logic_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_ELSE else{ throw cranberries::logic_error( "logic_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_WITH_MSG_IF( COND, MESSAGE ) if( COND ){ throw cranberries::logic_error( "logic_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_WITH_MSG_ELSE_IF( COND, MESSAGE ) else if( COND ){ throw cranberries::logic_error( "logic_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::logic_error( "logic_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::logic_error( "logic_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} )
#define CRANBERRIES_LOGIC_ERROR_THROW_CONDITIONAL( COND ) ( COND ) ? throw cranberries::logic_error( "logic_error", __FILE__, __FUNCTION__, __LINE__, #COND )
#define CRANBERRIES_LOGIC_ERROR_THROW_CONDITIONAL_WITH_MSG( COND, MESSAGE ) ( COND ) ? throw cranberries::logic_error( "logic_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE )

#define CRANBERRIES_DOMAIN_ERROR_THROW_IF( COND ) if( COND ){ throw cranberries::domain_error( "domain_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_ELSE_IF( COND ) else if( COND ){ throw cranberries::domain_error( "domain_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_ELSE else{ throw cranberries::domain_error( "domain_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG_IF( COND, MESSAGE ) if( COND ){ throw cranberries::domain_error( "domain_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG_ELSE_IF( COND, MESSAGE ) else if( COND ){ throw cranberries::domain_error( "domain_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::domain_error( "domain_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::domain_error( "domain_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} )
#define CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL( COND ) ( COND ) ? throw cranberries::domain_error( "domain_error", __FILE__, __FUNCTION__, __LINE__, #COND )
#define CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL_WITH_MSG( COND, MESSAGE ) ( COND ) ? throw cranberries::domain_error( "domain_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE )

#define CRANBERRIES_INVALID_ARGUMENT_THROW_IF( COND ) if( COND ){ throw cranberries::invalid_argument( "invalid_argument", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_ELSE_IF( COND ) else if( COND ){ throw cranberries::invalid_argument( "invalid_argument", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_ELSE else{ throw cranberries::invalid_argument( "invalid_argument", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( COND, MESSAGE ) if( COND ){ throw cranberries::invalid_argument( "invalid_argument", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_ELSE_IF( COND, MESSAGE ) else if( COND ){ throw cranberries::invalid_argument( "invalid_argument", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::invalid_argument( "invalid_argument", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG( MESSAGE ) throw cranberries::invalid_argument( "invalid_argument", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} )
#define CRANBERRIES_INVALID_ARGUMENT_THROW_CONDITIONAL( COND ) ( COND ) ? throw cranberries::invalid_argument( "invalid_argument", __FILE__, __FUNCTION__, __LINE__, #COND )
#define CRANBERRIES_INVALID_ARGUMENT_THROW_CONDITIONAL_WITH_MSG( COND, MESSAGE ) ( COND ) ? throw cranberries::invalid_argument( "invalid_argument", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE )

#define CRANBERRIES_LENGTH_ERROR_THROW_IF( COND ) if( COND ){ throw cranberries::length_error( "length_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_LENGTH_ERROR_THROW_ELSE_IF( COND ) else if( COND ){ throw cranberries::length_error( "length_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_LENGTH_ERROR_THROW_ELSE else{ throw cranberries::length_error( "length_error", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_LENGTH_ERROR_THROW_WITH_MSG_IF( COND, MESSAGE ) if( COND ){ throw cranberries::length_error( "length_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_LENGTH_ERROR_THROW_WITH_MSG_ELSE_IF( COND, MESSAGE ) else if( COND ){ throw cranberries::length_error( "length_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_LENGTH_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::length_error( "length_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_LENGTH_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::length_error( "length_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} )
#define CRANBERRIES_LENGTH_ERROR_THROW_CONDITIONAL( COND ) ( COND ) ? throw cranberries::length_error( "length_error", __FILE__, __FUNCTION__, __LINE__, #COND )
#define CRANBERRIES_LENGTH_ERROR_THROW_CONDITIONAL_WITH_MSG( COND, MESSAGE ) ( COND ) ? throw cranberries::length_error( "length_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE )

#define CRANBERRIES_OUT_OF_RANGE_THROW_IF( COND ) if( COND ){ throw cranberries::out_of_range( "out_of_range", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_OUT_OF_RANGE_THROW_ELSE_IF( COND ) else if( COND ){ throw cranberries::out_of_range( "out_of_range", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_OUT_OF_RANGE_THROW_ELSE else{ throw cranberries::out_of_range( "out_of_range", __FILE__, __FUNCTION__, __LINE__, #COND ); }
#define CRANBERRIES_OUT_OF_RANGE_THROW_WITH_MSG_IF( COND, MESSAGE ) if( COND ){ throw cranberries::out_of_range( "out_of_range", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_OUT_OF_RANGE_THROW_WITH_MSG_ELSE_IF( COND, MESSAGE ) else if( COND ){ throw cranberries::out_of_range( "out_of_range", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE ); }
#define CRANBERRIES_OUT_OF_RANGE_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::out_of_range( "out_of_range", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_OUT_OF_RANGE_THROW_WITH_MSG( MESSAGE ) throw cranberries::out_of_range( "out_of_range", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} )
#define CRANBERRIES_OUT_OF_RANGE_THROW_CONDITIONAL( COND ) ( COND ) ? throw cranberries::out_of_range( "out_of_range", __FILE__, __FUNCTION__, __LINE__, #COND )
#define CRANBERRIES_OUT_OF_RANGE_THROW_CONDITIONAL_WITH_MSG( COND, MESSAGE ) ( COND ) ? throw cranberries::out_of_range( "out_of_range", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE )

namespace cranberries
{
  class message_tag {};

  class exception {};

  class runtime_error : public std::runtime_error, public cranberries::exception
  {
  public:
    runtime_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : std::runtime_error(
        std::string( "excetopn : " ) + except_name + "\n"
        + "  in " + sorce_name + "\n"
        + "  " + func_name + "() (line." + std::to_string( line ) + ")\n"
        + "  follow by below\n"
        + "  Condition : " + condition
        + ( ( msg.empty() || msg[0] == '\0' ) ? "\n" : "\n  Message   : " + msg + "\n" )
      )
    {}
    runtime_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& ) noexcept
      : std::runtime_error(
        std::string( "excetopn : " ) + except_name + "\n"
        + " in " + sorce_name + "\n"
        + "  " + func_name + "() (line." + std::to_string( line ) + ")\n"
        + "  follow by below\n"
        + "  Message : " + msg + "\n"
      )
    {}
  };

  // out of range error
  class range_error : public runtime_error
  {
  public:
    range_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, msg )
    {}
    range_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, "" )
    {}
    range_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, msg, std::move( tag ) )
    {}
  };
  // divergence error
  class overflow_error : public runtime_error
  {
  public:
    overflow_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, msg )
    {}
    overflow_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, "" )
    {}
    overflow_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, msg, std::move( tag ) )
    {}
  };

  // out of doamin error
  class underflow_error : public runtime_error
  {
  public:
    underflow_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, msg )
    {}
    underflow_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, "" )
    {}
    underflow_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, msg, std::move( tag ) )
    {}
  };

  class logic_error : public std::logic_error, public cranberries::exception
  {
  public:
    logic_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : std::logic_error(
        std::string( "excetopn : " ) + except_name + "\n"
        + "  in " + sorce_name + "\n"
        + "  " + func_name + "() (line." + std::to_string( line ) + ")\n"
        + "  follow by below\n"
        + "  Condition : " + condition
        + ( ( msg.empty() || msg[0] == '\0' ) ? "\n" : "\n  Message   : " + msg + "\n" )
      )
    {}
    logic_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& ) noexcept
      : std::logic_error(
        std::string( "excetopn : " ) + except_name + "\n"
        + " in " + sorce_name + "\n"
        + "  " + func_name + "() (line." + std::to_string( line ) + ")\n"
        + "  follow by below\n"
        + "  Message : " + msg + "\n"
      )
    {}
  };


  class domain_error : public logic_error
  {
  public:
    domain_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, condition, msg )
    {}
    domain_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, condition, "" )
    {}
    domain_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, msg, std::move( tag ) )
    {}
  };

  class invalid_argument : public logic_error
  {
  public:
    invalid_argument( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, condition, msg )
    {}
    invalid_argument( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, condition, "" )
    {}
    invalid_argument( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, msg, std::move( tag ) )
    {}
  };
  class length_error : public logic_error
  {
  public:
    length_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, condition, msg )
    {}
    length_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, condition, "" )
    {}
    length_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, msg, std::move( tag ) )
    {}
  };
  class out_of_range : public logic_error
  {
  public:
    out_of_range( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, condition, msg )
    {}
    out_of_range( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, condition, "" )
    {}
    out_of_range( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag ) noexcept
      : logic_error( except_name, sorce_name, func_name, line, msg, std::move( tag ) )
    {}
  };

}
#endif