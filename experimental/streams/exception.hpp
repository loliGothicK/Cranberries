#ifndef CRANBERRIES_EXCEPTION_HPP
#define CRANBERRIES_EXCEPTION_HPP
#include <exception>
#include <stdexcept>
#include <string>


#ifndef CRANBERRIES_FULL_FUNCSIG
#define CRANBERRIES_FUNCTION __FUNCTION__
#elif _MSC_VER
#define CRANBERRIES_PRETTY_FUNCTION __FUNCSIG__
#else
#define CRANBERRIES_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

#define CRANBERRIES_RUNTIME_ERROR_THROW_IF( EXPR ) if( EXPR ){ throw cranberries::runtime_error("runtime_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_ELSE_IF( EXPR ) else if( EXPR ){ throw cranberries::runtime_error("runtime_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_ELSE else{ throw cranberries::runtime_error("runtime_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_WITH_MSG_IF( EXPR, MESSAGE ) if( EXPR ){ throw cranberries::runtime_error("runtime_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_WITH_MSG_ELSE_IF( EXPR, MESSAGE ) else if( EXPR ){ throw cranberries::runtime_error("runtime_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::runtime_error("runtime_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_RUNTIME_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::runtime_error("runtime_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} );

#define CRANBERRIES_RANGE_ERROR_THROW_IF( EXPR ) if( EXPR ){ throw cranberries::range_error( "range_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_RANGE_ERROR_THROW_ELSE_IF( EXPR ) else if( EXPR ){ throw cranberries::range_error( "range_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_RANGE_ERROR_THROW_ELSE else{ throw cranberries::range_error( "range_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_RANGE_ERROR_THROW_WITH_MSG_IF( EXPR, MESSAGE ) if( EXPR ){ throw cranberries::range_error( "range_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_RANGE_ERROR_THROW_WITH_MSG_ELSE_IF( EXPR, MESSAGE ) else if( EXPR ){ throw cranberries::range_error( "range_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_RANGE_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::range_error( "range_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_RANGE_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::range_error( "range_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} );

#define CRANBERRIES_OVER_FLOW_THROW_IF( EXPR ) if( EXPR ){ throw cranberries::over_flow( "over_flow", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_OVER_FLOW_THROW_ELSE_IF( EXPR ) else if( EXPR ){ throw cranberries::over_flow( "over_flow", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_OVER_FLOW_THROW_ELSE else{ throw cranberries::over_flow( "over_flow", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_OVER_FLOW_THROW_WITH_MSG_IF( EXPR, MESSAGE ) if( EXPR ){ throw cranberries::over_flow( "over_flow", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_OVER_FLOW_THROW_WITH_MSG_ELSE_IF( EXPR, MESSAGE ) else if( EXPR ){ throw cranberries::over_flow( "over_flow", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_OVER_FLOW_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::over_flow( "over_flow", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_OVER_FLOW_THROW_WITH_MSG( MESSAGE ) throw cranberries::over_flow( "over_flow", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} );

#define CRANBERRIES_DOMAIN_ERROR_THROW_IF( EXPR ) if( EXPR ){ throw cranberries::domain_error( "domain_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_ELSE_IF( EXPR ) else if( EXPR ){ throw cranberries::domain_error( "domain_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_ELSE else{ throw cranberries::domain_error( "domain_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG_IF( EXPR, MESSAGE ) if( EXPR ){ throw cranberries::domain_error( "domain_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG_ELSE_IF( EXPR, MESSAGE ) else if( EXPR ){ throw cranberries::domain_error( "domain_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::domain_error( "domain_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::domain_error( "domain_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} );

#define CRANBERRIES_LOGIC_ERROR_THROW_IF( EXPR ) if( EXPR ){ throw cranberries::logic_error( "logic_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_ELSE_IF( EXPR ) else if( EXPR ){ throw cranberries::logic_error( "logic_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_ELSE else{ throw cranberries::logic_error( "logic_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_WITH_MSG_IF( EXPR, MESSAGE ) if( EXPR ){ throw cranberries::logic_error( "logic_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_WITH_MSG_ELSE_IF( EXPR, MESSAGE ) else if( EXPR ){ throw cranberries::logic_error( "logic_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::logic_error( "logic_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_LOGIC_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::logic_error( "logic_error", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} );

#define CRANBERRIES_INVALID_ARGUMENT_THROW_IF( EXPR ) if( EXPR ){ throw cranberries::invalid_argument( "invalid_argument", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_ELSE_IF( EXPR ) else if( EXPR ){ throw cranberries::invalid_argument( "invalid_argument", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_ELSE else{ throw cranberries::invalid_argument( "invalid_argument", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( EXPR, MESSAGE ) if( EXPR ){ throw cranberries::invalid_argument( "invalid_argument", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_ELSE_IF( EXPR, MESSAGE ) else if( EXPR ){ throw cranberries::invalid_argument( "invalid_argument", __FILE__, CRANBERRIES_FUNCTION, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::invalid_argument( "invalid_argument", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG( MESSAGE ) throw cranberries::invalid_argument( "invalid_argument", __FILE__, CRANBERRIES_FUNCTION, __LINE__, MESSAGE, cranberries::message_tag{} );

namespace cranberries
{
  class message_tag {};

  //  ALL interval error 
  class runtime_error : public std::runtime_error
  {
  public:
    runtime_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : std::runtime_error(
        std::string( "excetopn : " ) + except_name + "\n"
        + "  in : " + sorce_name + " [ line " + std::to_string( line ) + " ]\n"
        + "  follow by below\n"
        + "  Condition : " + condition
        + ( ( msg.empty() || msg[0] == '\0' ) ? "\n" : "\n  Message   : " + msg + "\n" )
        + "  Note : function\n" + func_name + "\n"
      )
    {}
    runtime_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& ) noexcept
      : std::runtime_error(
        std::string( "excetopn : " ) + except_name + "\n"
        + "  in : " + sorce_name + " [ line " + std::to_string( line ) + "]\n"
        + "  follow by below\n"
        + "  Message : " + msg + "\n"
        + "  Note : function\n" + func_name + "\n"
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
  class over_flow : public runtime_error
  {
  public:
    over_flow( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, msg )
    {}
    over_flow( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, "" )
    {}
    over_flow( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, msg, std::move( tag ) )
    {}
  };

  // out of doamin error
  class domain_error : public runtime_error
  {
  public:
    domain_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, msg )
    {}
    domain_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, "" )
    {}
    domain_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, msg, std::move( tag ) )
    {}
  };

  // 
  class logic_error : public runtime_error
  {
  public:
    logic_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, msg )
    {}
    logic_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, "" )
    {}
    logic_error( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, msg, std::move( tag ) )
    {}
  };

  // n
  class invalid_argument : public runtime_error
  {
  public:
    invalid_argument( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, msg )
    {}
    invalid_argument( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, condition, "" )
    {}
    invalid_argument( const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag ) noexcept
      : runtime_error( except_name, sorce_name, func_name, line, msg, std::move( tag ) )
    {}
  };
}
#endif