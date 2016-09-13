#ifndef CRANBERRIES_STREAM_ERROR_HPP
#define CRANBERRIES_STREAM_ERROR_HPP
#include <exception>
#include <stdexcept>
#include "exception.hpp"

#define CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( EXPR ) if( EXPR ){ throw cranberries::stream_empty_error("stream_empty_error", __FILE__, __FUNCTION__, __LINE__, #EXPR ); }
#define CRANBERRIES_STREAM_EMPTY_ERROR_THROW_ELSE_IF( EXPR ) else if( EXPR ){ throw cranberries::stream_empty_error("stream_empty_error", __FILE__, __FUNCTION__, __LINE__, #EXPR ); }
#define CRANBERRIES_STREAM_EMPTY_ERROR_THROW_ELSE else{ throw cranberries::stream_empty_error("stream_empty_error", __FILE__, __FUNCTION__, __LINE__, #EXPR ); }
#define CRANBERRIES_STREAM_EMPTY_ERROR_THROW_WITH_MSG_IF( EXPR, MESSAGE ) if( EXPR ){ throw cranberries::stream_empty_error("stream_empty_error", __FILE__, __FUNCTION__, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_STREAM_EMPTY_ERROR_THROW_WITH_MSG_ELSE_IF( EXPR, MESSAGE ) else if( EXPR ){ throw cranberries::stream_empty_error("stream_empty_error", __FILE__, __FUNCTION__, __LINE__, #EXPR, MESSAGE ); }
#define CRANBERRIES_STREAM_EMPTY_ERROR_THROW_WITH_MSG_ELSE( MESSAGE ) else{ throw cranberries::stream_empty_error("stream_empty_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} ); }
#define CRANBERRIES_STREAM_EMPTY_ERROR_THROW_WITH_MSG( MESSAGE ) throw cranberries::stream_empty_error("stream_empty_error", __FILE__, __FUNCTION__, __LINE__, MESSAGE, cranberries::message_tag{} );
#define CRANBERRIES_STREAM_EMPTY_ERROR_THROW_CONDITIONAL( COND ) ( COND ) ? throw cranberries::stream_empty_error( "stream_empty_error", __FILE__, __FUNCTION__, __LINE__, #COND )
#define CRANBERRIES_STREAM_EMPTY_ERROR_THROW_CONDITIONAL_WITH_MSG( COND, MESSAGE ) ( COND ) ? throw cranberries::stream_empty_error( "stream_empty_error", __FILE__, __FUNCTION__, __LINE__, #COND, MESSAGE )


namespace cranberries {
namespace streams {
namespace except {

	class stream_empty_error : public cranberries::runtime_error
	{
		explicit stream_empty_error(const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition, const std::string& msg) noexcept
			: runtime_error(except_name, sorce_name, func_name, line, condition, msg)
		{}
		explicit stream_empty_error(const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* condition) noexcept
			: runtime_error(except_name, sorce_name, func_name, line, condition, "")
		{}
		explicit stream_empty_error(const char* except_name, const char* sorce_name, const char* func_name, uint64_t line, const char* msg, message_tag&& tag) noexcept
			: runtime_error(except_name, sorce_name, func_name, line, msg, std::move(tag))
		{}
	};



}
}
}

#endif