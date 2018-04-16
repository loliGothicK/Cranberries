#ifndef CRANBERRIES_OPTIONAL_HPP
#define CRANBERRIES_OPTIONAL_HPP

#include "./common/definition.hpp"
#include <type_traits>
#include <exception>
#include <memory>
#include <utility>
#include <initializer_list>
#include <new>
#include <functional>
#include <cassert>
#include "./type_traits.hpp"
namespace cranberries {

#ifdef NO_CONSTEXPR_STD_MOVE
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR typename std::remove_reference<T>::type &&move(T &&t) noexcept{
		using U = typename std::remove_reference<T>::type;
		return static_cast<U &&>(t);
	}
#else // NO_CONSTEXPR_STD_MOVE
	using std::move;
#endif // NO_CONSTEXPR_STD_MOVE

#ifdef NO_CONSTEXPR_STD_MOVE_IF_NOEXCEPT
	template <typename T, 
		typename std::enable_if<(!(std::is_nothrow_move_constructible<T>::value)) && std::is_copy_constructible<T>::value>::type * = nullptr>
	CRANBERRIES_CXX11_CONSTEXPR const T &move_if_noexcept(T &x) noexcept{
		return x;
	}
	template <typename T, 
		typename std::enable_if<std::is_nothrow_move_constructible<T>::value || (!(std::is_copy_constructible<T>::value))>::type * = nullptr>
	CRANBERRIES_CXX11_CONSTEXPR T &&move_if_noexcept(T &x) noexcept{
		return std::move(x);
	}
#else // NO_CONSTEXPR_STD_MOVE_IF_NOEXCEPT
	using std::move_if_noexcept;
#endif // NO_CONSTEXPR_STD_MOVE_IF_NOEXCEPT

#ifdef NO_CONSTEXPR_STD_FORWARD
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR T &&forward(typename std::remove_reference<T>::type &t) noexcept{
		return static_cast<T &&>(t);
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR T &&forward(typename std::remove_reference<T>::type &&t) noexcept{
		static_assert(!(std::is_lvalue_reference<T>::value), "cannot forward rvalue as lvalue");
		return static_cast<T &&>(t);
	}
#else // NO_CONSTEXPR_STD_FORWARD
	using std::forward;
#endif // NO_CONSTEXPR_STD_FORWARD


	struct nullopt_t{};
	constexpr static nullopt_t nullopt{};

	struct in_place_t{
		explicit in_place_t() = default;
	};
	constexpr static in_place_t in_place{};

	class bad_optional_access: public std::exception{
	public:
		bad_optional_access() noexcept = default;

		bad_optional_access(const bad_optional_access &) noexcept = default;

		bad_optional_access &operator=(const bad_optional_access &) noexcept = default;

		~bad_optional_access() = default;

		const char *what() const noexcept{
						return "bad optional access";
				}
	};

	// forward declaration
	template <typename>
	class optional;

	namespace _detail{

		template <typename T, typename U>
		struct other_optional_copy_convertible_impl{
			constexpr static bool value = 
				std::is_constructible<T, const U &>::value && 
				(!std::is_constructible<T, optional<U> &>::value) && 
				(!std::is_constructible<T, const optional<U> &>::value) && 
				(!std::is_constructible<T, optional<U> &&>::value) && 
				(!std::is_constructible<T, const optional<U> &&>::value) &&
				(!std::is_convertible<optional<U> &, T>::value) && 
				(!std::is_convertible<const optional<U> &, T>::value) && 
				(!std::is_convertible<optional<U> &&, T>::value) && 
				(!std::is_convertible<const optional<U> &&, T>::value);
		};

		template <typename T, typename U>
		struct other_optional_move_convertible_impl{
			constexpr static bool value = 
				std::is_constructible<T, U &&>::value && 
				(!std::is_constructible<T, optional<U> &>::value) && 
				(!std::is_constructible<T, const optional<U> &>::value) && 
				(!std::is_constructible<T, optional<U> &&>::value) && 
				(!std::is_constructible<T, const optional<U> &&>::value) &&
				(!std::is_convertible<optional<U> &, T>::value) && 
				(!std::is_convertible<const optional<U> &, T>::value) && 
				(!std::is_convertible<optional<U> &&, T>::value) && 
				(!std::is_convertible<const optional<U> &&, T>::value);
		};

		template <typename T, typename U>
		struct other_optional_copy_convertible
			:public std::integral_constant<bool, other_optional_copy_convertible_impl<T, U>::value>{};
		
		template <typename T, typename U>
		struct other_optional_move_convertible
			:public std::integral_constant<bool, other_optional_move_convertible_impl<T, U>::value>{};

		template <typename T, typename U>
		struct other_optional_assignable_impl{
			constexpr static bool value = 
				!(std::is_constructible<T, optional<U> &>::value) && 
				!(std::is_constructible<T, const optional<U> &>::value) && 
				!(std::is_constructible<T, optional<U> &&>::value) && 
				!(std::is_constructible<T, const optional<U> &&>::value) && 
				!(std::is_convertible<optional<U> &, T>::value) && 
				!(std::is_convertible<const optional<U> &, T>::value) && 
				!(std::is_convertible<optional<U> &&, T>::value) && 
				!(std::is_convertible<const optional<U> &&, T>::value) && 
				!(std::is_assignable<T &, optional<U> &>::value) && 
				!(std::is_assignable<T &, const optional<U> &>::value) && 
				!(std::is_assignable<T &, optional<U> &&>::value) && 
				!(std::is_assignable<T &, const optional<U> &&>::value);
		};

		template <typename T, typename U>
		struct other_optional_assignable
			:public std::integral_constant<bool, other_optional_assignable_impl<T, U>::value>{};

	} // namespace _detail

	namespace _detail{

		// to satisfy optional's destructor's requirements
		template <typename T, class optional_t, typename trivial_destruct = void>
		class optional_impl;
		// case for trivially destructible T
		template <typename T, class optional_t>
		class optional_impl<T, optional_t, typename std::enable_if<std::is_trivially_destructible<T>::value>::type>{
		public:
			// trivial destructor
			~optional_impl() = default;
		}; // class optional_impl
		// case for non trivially desturctible T
		template <typename T, class optional_t>
		class optional_impl<T, optional_t, typename std::enable_if<!(std::is_trivially_destructible<T>::value)>::type>{
		public:
			// non trival destructor
			~optional_impl(){
				optional_t *derived = static_cast<optional_t *>(this);
				if(derived->hasvalue){
					derived->destory();
				}
			}
		}; // class optional_impl

		template <typename T, typename is_class = void>
		class optional_value_holder;
		// only classes and enums are allowed to overload operators.
		template <typename T>
		class optional_value_holder<T, typename std::enable_if<(std::is_class<T>::value || std::is_enum<T>::value)>::type>
			:public T{
			//friend class optional<T>;
		public:
			optional_value_holder(const optional_value_holder &) = default;
			optional_value_holder(optional_value_holder &&) = default;

			optional_value_holder &operator=(const optional_value_holder &) = default;
			optional_value_holder &operator=(optional_value_holder &&) = default;

			~optional_value_holder() = default;

			template <typename ...Args>
			explicit CRANBERRIES_CXX11_CONSTEXPR optional_value_holder(std::true_type, Args &&...args)
				:T(std::forward<Args>(args)...){}
			template <typename U, typename ...Args>
			explicit CRANBERRIES_CXX11_CONSTEXPR optional_value_holder(std::true_type, std::initializer_list<U> ilist, Args &&...args)
				:T(ilist, std::forward<Args>(args)...){}

			template <typename U>
			optional_value_holder &assign(U &&_rhs){
				T::operator=(std::forward<U>(_rhs));
				return *this;
			}

			CRANBERRIES_CXX11_CONSTEXPR const T *get_addr() const{
				return static_cast<const T *>(this);
			}
			CRANBERRIES_CXX14_CONSTEXPR T *get_addr(){
				return static_cast<T *>(this);
			}

			CRANBERRIES_CXX11_CONSTEXPR const T &get_ref() const &{
				return static_cast<const T &>(*this);
			}
			CRANBERRIES_CXX14_CONSTEXPR T &get_ref() &{
				return static_cast<T &>(*this);
			}
			CRANBERRIES_CXX11_CONSTEXPR const T &&get_ref() const &&{
				return static_cast<const T &&>(*this);
			}
			CRANBERRIES_CXX14_CONSTEXPR T &&get_ref() &&{
				return static_cast<T &&>(*this);
			}
		}; // class optional_value_holder
		template <typename T>
		class optional_value_holder<T, typename std::enable_if<!(std::is_class<T>::value || std::is_enum<T>::value)>::type>{
			//friend class optional<T>;
		public:
			optional_value_holder(const optional_value_holder &) = default;
			optional_value_holder(optional_value_holder &&) = default;

			optional_value_holder &operator=(const optional_value_holder &) = default;
			optional_value_holder &operator=(optional_value_holder &&) = default;

			~optional_value_holder() = default;

			template <typename ...Args>
			explicit CRANBERRIES_CXX11_CONSTEXPR optional_value_holder(std::true_type, Args &&...args)
				:value(std::forward<Args>(args)...){}
			template <typename U, typename ...Args>
			explicit CRANBERRIES_CXX11_CONSTEXPR optional_value_holder(std::true_type, std::initializer_list<U> ilist, Args &&...args)
				:value(ilist, std::forward<Args>(args)...){}

			template <typename U>
			optional_value_holder &assign(U &&_rhs){
				value = std::forward<U>(_rhs);
				return *this;
			}

			CRANBERRIES_CXX11_CONSTEXPR const T *get_addr() const{
				return &value;
			}
			CRANBERRIES_CXX14_CONSTEXPR T *get_addr(){
				return &value;
			}

			CRANBERRIES_CXX11_CONSTEXPR const T &get_ref() const &{
				return static_cast<const T &>(value);
			}
			CRANBERRIES_CXX14_CONSTEXPR T &get_ref() &{
				return static_cast<T &>(value);
			}
			CRANBERRIES_CXX11_CONSTEXPR const T &&get_ref() const &&{
				return static_cast<const T &&>(value);
			}
			CRANBERRIES_CXX14_CONSTEXPR T &&get_ref() &&{
				return static_cast<T &&>(value);
			}
		private:
			T value;
		}; // class optional_value_holder

	} // namespace _detail

	template <typename T>
	class optional
		:private _detail::optional_impl<T, optional<T>>{
	private:
		friend class _detail::optional_impl<T, optional<T>>;

		using base_t = _detail::optional_impl<T, optional<T>>;
		using holder_t = _detail::optional_value_holder<T>;

		class empty_t{};

		union storage_t{
			empty_t empty;
			holder_t holder;

			// initialize with empty
			explicit CRANBERRIES_CXX11_CONSTEXPR storage_t(std::false_type)
				:empty(){}
			// initialize with value
			template <typename ...Args>
			explicit CRANBERRIES_CXX11_CONSTEXPR storage_t(std::true_type, Args &&...args)
				:holder(std::true_type{}, std::forward<Args>(args)...){}
			template <typename U, typename ...Args>
			explicit CRANBERRIES_CXX11_CONSTEXPR storage_t(std::true_type, std::initializer_list<U> ilist, Args &&...args)
				:holder(std::true_type{}, ilist, std::forward<Args>(args)...){}
			
			~storage_t() = default;
		};
	public:
		using value_type = T;
	private:
		// storage_t hold the same address of value T.
		CRANBERRIES_CXX11_CONSTEXPR const void *getptr_impl() const{
			return &storage;
		}
		CRANBERRIES_CXX14_CONSTEXPR void *getptr_impl(){
			return &storage;
		}

		// accessing address of an nullopt_t optional is undefined behavior.
		CRANBERRIES_CXX11_CONSTEXPR const T *getptr() const{
			//assert(hasvalue);
			//return static_cast<const T*>(getptr_impl());
			return static_cast<const T *>(storage.holder.get_addr());
		}
		CRANBERRIES_CXX14_CONSTEXPR T *getptr(){
			assert(hasvalue);
			return static_cast<T *>(storage.holder.get_addr());
		}

		CRANBERRIES_CXX11_CONSTEXPR const T &get_value() const &{
			//assert(hasvalue);
			return static_cast<const T &>(storage.holder.get_ref());
		}
		CRANBERRIES_CXX14_CONSTEXPR T &get_value() &{
			assert(hasvalue);
			return static_cast<T &>(storage.holder.get_ref());
		}
		CRANBERRIES_CXX11_CONSTEXPR const T &&get_value() const &&{
			//assert(hasvalue);
			return static_cast<const T &&>(std::move(storage.holder).get_ref());
		}
		CRANBERRIES_CXX14_CONSTEXPR T &&get_value() &&{
			assert(hasvalue);
			return static_cast<T &&>(std::move(storage.holder).get_ref());
		}
	public:
		// ====================== constructors ======================
		CRANBERRIES_CXX11_CONSTEXPR optional() noexcept
			:storage(std::false_type{}), hasvalue(false){}
		CRANBERRIES_CXX11_CONSTEXPR optional(nullopt_t) noexcept
			:storage(std::false_type{}), hasvalue(false){}
		
		// dependent false trick to fool the compiler
		template <typename Dummy = T, 
			typename std::enable_if<(sizeof(Dummy), std::is_copy_constructible<T>::value)>::type * = nullptr, 
			typename std::enable_if<!(sizeof(Dummy), std::is_trivially_copy_constructible<T>::value)>::type * = nullptr>
		optional(const optional &_rhs)
			:storage(std::false_type{}), hasvalue(false){
			if(_rhs.hasvalue){
				// change the active union member
				new (getptr_impl()) holder_t(_rhs.storage.holder);
			}
		}
		template <typename Dummy = T, 
			typename std::enable_if<(sizeof(Dummy), std::is_copy_constructible<T>::value)>::type * = nullptr, 
			typename std::enable_if<(sizeof(Dummy), std::is_trivially_copy_constructible<T>::value)>::type * = nullptr>
		CRANBERRIES_CXX11_CONSTEXPR optional(const optional &_rhs)
			// trivially-copyables can finish their copy by directly memmove their underlying storage
			// active member of union cannot be changed in constexpr function
			:storage(_rhs.storage), hasvalue(_rhs.hasvalue){}
		
		template <typename Dummy = T, 
			typename std::enable_if<(sizeof(Dummy), std::is_move_constructible<T>::value)>::type * = nullptr, 
			typename std::enable_if<!(sizeof(Dummy), std::is_trivially_move_constructible<T>::value)>::type * = nullptr>
		optional(optional &&_rhs) noexcept(std::is_nothrow_move_constructible<T>::value)
			:storage(std::false_type{}), hasvalue(false){
			if(_rhs.hasvalue){
				new (getptr_impl()) holder_t(std::move(std::move(_rhs).storage.holder));
			}
		}
		template <typename Dummy = T, 
			typename std::enable_if<(sizeof(Dummy), std::is_move_constructible<T>::value)>::type * = nullptr, 
			typename std::enable_if<(sizeof(Dummy), std::is_trivially_move_constructible<T>::value)>::type * = nullptr>
		CRANBERRIES_CXX11_CONSTEXPR optional(optional &&_rhs)
			:storage(_rhs.storage), hasvalue(_rhs.hasvalue){}
		
		template <typename U, 
			typename std::enable_if<!(std::is_same<T, U>::value)>::type * = nullptr, 
			typename std::enable_if<_detail::other_optional_copy_convertible<T, U>::value>::type * = nullptr, 
			typename std::enable_if<std::is_convertible<const U &, T>::value>::type * = nullptr>
		optional(const optional<U> &_rhs)
			:storage(std::false_type{}), hasvalue(false){
			if(_rhs.hasvalue){
				new (getptr_impl()) holder_t(std::true_type{}, _rhs.storage.holder.get_ref());
			}
		}
		template <typename U, 
			typename std::enable_if<!(std::is_same<T, U>::value)>::type * = nullptr, 
			typename std::enable_if<_detail::other_optional_copy_convertible<T, U>::value>::type * = nullptr, 
			typename std::enable_if<!(std::is_convertible<const U &, T>::value)>::type * = nullptr>
		explicit optional(const optional<U> &_rhs)
			:storage(std::false_type{}), hasvalue(false){
			if(_rhs.hasvalue){
				new (getptr_impl()) holder_t(std::true_type{}, _rhs.storage.holder.get_ref());
			}
		}

		template <typename U, 
			typename std::enable_if<!(std::is_same<T, U>::value)>::type * = nullptr, 
			typename std::enable_if<_detail::other_optional_move_convertible<T, U>::value>::type * = nullptr, 
			typename std::enable_if<std::is_convertible<U &&, T>::value>::type * = nullptr>
		optional(optional<U> &&_rhs)
			:storage(std::false_type{}), hasvalue(false){
			if(_rhs.hasvalue){
				new (getptr_impl()) holder_t(std::true_type{}, std::move(_rhs.storage.holder).get_ref());
			}
		}
		template <typename U, 
			typename std::enable_if<!(std::is_same<T, U>::value)>::type * = nullptr, 
			typename std::enable_if<_detail::other_optional_move_convertible<T, U>::value>::type * = nullptr, 
			typename std::enable_if<!(std::is_convertible<U &&, T>::value)>::type * = nullptr>
		explicit optional(optional<U> &&_rhs)
			:storage(std::false_type{}), hasvalue(false){
			if(_rhs.hasvalue){
				new (getptr_impl()) holder_t(std::true_type{}, std::move(_rhs.storage.holder).get_ref());
			}
		}

		template <typename ...Args, 
			typename std::enable_if<std::is_constructible<T, Args...>::value>::type * = nullptr>
		CRANBERRIES_CXX11_CONSTEXPR explicit optional(in_place_t, Args &&...args)
			:storage(std::true_type{}, std::forward<Args>(args)...), hasvalue(true){}

		template <typename U, typename ...Args, 
			typename std::enable_if<std::is_constructible<T, std::initializer_list<U> &, Args &&...>::value>::type * = nullptr>
		CRANBERRIES_CXX11_CONSTEXPR explicit optional(in_place_t, std::initializer_list<U> ilist, Args &&...args)
			:storage(std::true_type{}, ilist, std::forward<Args>(args)...), hasvalue(true){}

		template <typename U = T, 
			typename std::enable_if<std::is_constructible<T, U &&>::value>::type * = nullptr, 
			typename std::enable_if<!(std::is_same<typename std::decay<U>::type, in_place_t>::value)>::type * = nullptr, 
			typename std::enable_if<!(std::is_same<typename std::decay<U>::type, optional<T>>::value)>::type * = nullptr, 
			typename std::enable_if<std::is_convertible<U &&, T>::value>::type * = nullptr>
		CRANBERRIES_CXX11_CONSTEXPR optional(U &&value)
			:storage(std::true_type{}, std::forward<U>(value)), hasvalue(true){}

		template <typename U = T, 
			typename std::enable_if<std::is_constructible<T, U &&>::value>::type * = nullptr, 
			typename std::enable_if<!(std::is_same<typename std::decay<U>::type, in_place_t>::value)>::type * = nullptr, 
			typename std::enable_if<!(std::is_same<typename std::decay<U>::type, optional<T>>::value)>::type * = nullptr, 
			typename std::enable_if<!(std::is_convertible<U &&, T>::value)>::type * = nullptr>
		CRANBERRIES_CXX11_CONSTEXPR explicit optional(U &&value)
			:storage(std::true_type{}, std::forward<U>(value)), hasvalue(true){}
		
		// ====================== destructors ======================
		// implicitly declared destructor
		// will become trivial destructor if inherted from proper base
		~optional() = default;

		// ====================== copy/move assignment operators ======================
		optional &operator=(nullopt_t) noexcept{
			if(hasvalue){
				destroy();
				hasvalue = false;
			}
			return *this;
		}

		template <typename Dummy = T, 
			typename std::enable_if<(sizeof(Dummy), std::is_copy_constructible<T>::value)>::type * = nullptr, 
			typename std::enable_if<(sizeof(Dummy), std::is_copy_assignable<T>::value)>::type * = nullptr>
		optional &operator=(const optional &_rhs){
			if(this == &_rhs){
				return *this;
			}
			if((!hasvalue) && (!(_rhs.hasvalue))){
				return *this;
			}
			if(hasvalue && (!(_rhs.hasvalue))){
				destroy();
				hasvalue = false;
				return *this;
			}
			assert(_rhs.hasvalue);
			if(hasvalue){
				storage.holder = _rhs.storage.holder;
			}
			else{
				new (getptr_impl()) holder_t(_rhs.storage.holder);
				hasvalue = true;
			}
			return *this;
		}

		template <typename Dummy = T, 
			typename std::enable_if<(sizeof(Dummy), std::is_move_constructible<T>::value)>::type * = nullptr, 
			typename std::enable_if<(sizeof(Dummy), std::is_move_assignable<T>::value)>::type * = nullptr>
		optional &operator=(optional &&_rhs) 
			noexcept(std::is_nothrow_move_assignable<T>::value && std::is_nothrow_move_constructible<T>::value){
			if((!hasvalue) && (!(_rhs.hasvalue))){
				return *this;
			}
			if(hasvalue && (!(_rhs.hasvalue))){
				destroy();
				hasvalue = false;
				return *this;
			}
			assert(_rhs.hasvalue);
			if(hasvalue){
				storage.holder = std::move(_rhs.storage.holder);
			}
			else{
				new (getptr_impl()) holder_t(std::move(_rhs.storage.holder));
				hasvalue = true;
			}
			return *this;
		}

		template <typename U = T, 
			typename std::enable_if<!(std::is_same<typename std::decay<T>::type, optional<T>>::value)>::type * = nullptr, 
			typename std::enable_if<std::is_constructible<T, U>::value>::type * = nullptr, 
			typename std::enable_if<std::is_assignable<T, U>::value>::type * = nullptr, 
			typename std::enable_if<!(std::is_scalar<T>::value && std::is_same<typename std::decay<U>::type, T>::value)>::type * = nullptr>
		optional &operator=(U &&value){
			if(hasvalue){
				storage.holder.assign(std::forward<U>(value));
			}
			else{
				new (getptr_impl()) holder_t(std::true_type{}, std::forward<U>(value));
				hasvalue = true;
			}
			return *this;
		}

		template <typename U, 
			// to avoid collision with copy assignment operator that was intendedly made template
			typename std::enable_if<!(std::is_same<T, U>::value)>::type * = nullptr, 
			typename std::enable_if<_detail::other_optional_assignable<T, U>::value>::type * = nullptr, 
			typename std::enable_if<std::is_constructible<T, const U &>::value>::type * = nullptr, 
			typename std::enable_if<std::is_assignable<T &, const U &>::value>::type * = nullptr>
		optional &operator=(const optional<U> &_rhs){
			if((!hasvalue) && (!(_rhs.hasvalue))){
				return *this;
			}
			if(hasvalue && (!(_rhs.hasvalue))){
				destroy();
				hasvalue = false;
				return *this;
			}
			assert(_rhs.hasvalue);
			if(hasvalue){
				storage.holder.assign(_rhs.storage.holder.get_ref());
			}
			else{
				new (getptr_impl()) holder_t(std::true_type{}, _rhs.storage.holder.get_ref());
				hasvalue = true;
			}
			return *this;
		}

		template <typename U, 
			typename std::enable_if<!(std::is_same<T, U>::value)>::type * = nullptr, 
			typename std::enable_if<_detail::other_optional_assignable<T, U>::value>::type * = nullptr, 
			typename std::enable_if<std::is_constructible<T, U>::value>::type * = nullptr, 
			typename std::enable_if<std::is_assignable<T &, U>::value>::type * = nullptr>
		optional &operator=(optional<U> &&_rhs){
			if((!hasvalue) && (!(_rhs.hasvalue))){
				return *this;
			}
			if(hasvalue && (!(_rhs.hasvalue))){
				destroy();
				hasvalue = false;
				return *this;
			}
			assert(_rhs.hasvalue);
			if(hasvalue){
				storage.holder.assign(std::move(std::move(_rhs.storage.holder).get_ref()));
			}
			else{
				new (getptr_impl()) holder_t(std::move(std::move(_rhs.storage.holder).get_ref()));
				hasvalue = true;
			}
			return *this;
		}

		// ====================== operator->/operator* ======================
		CRANBERRIES_CXX11_CONSTEXPR const T *operator->() const{
			return getptr();
		}
		CRANBERRIES_CXX14_CONSTEXPR T *operator->(){
			return getptr();
		}

		CRANBERRIES_CXX11_CONSTEXPR const T &operator*() const &{
			return get_value();
		}
		CRANBERRIES_CXX14_CONSTEXPR T &operator*() &{
			return get_value();
		}
		CRANBERRIES_CXX11_CONSTEXPR const T &&operator*() const &&{
			return std::move(*this).get_value();
		}
		CRANBERRIES_CXX14_CONSTEXPR T &&operator*() &&{
			return std::move(*this).get_value();
		}

		// ====================== operator bool/has_value ======================
		CRANBERRIES_CXX11_CONSTEXPR explicit operator bool() const noexcept{
			return hasvalue;
		}
		CRANBERRIES_CXX11_CONSTEXPR bool has_value() const noexcept{
			return hasvalue;
		}

		// ====================== value ======================
		CRANBERRIES_CXX14_CONSTEXPR T &value() & {
			if (hasvalue) {
				return storage.holder.get_ref();
			}
			throw bad_optional_access();
		}
		CRANBERRIES_CXX14_CONSTEXPR const T &value() const & {
			if (hasvalue) {
				return storage.holder.get_ref();
			}
			throw bad_optional_access();
		}
		CRANBERRIES_CXX14_CONSTEXPR T &&value() && {
			if (hasvalue) {
				return std::move(storage.holder).get_ref();
			}
			throw bad_optional_access();
		}
		CRANBERRIES_CXX14_CONSTEXPR const T &&value() const &&{
			if (hasvalue) {
				return std::move(storage.holder).get_ref();
			}
			throw bad_optional_access();
		}
		// ====================== value_or ======================
		// the standard only requires the program is ill-formed if the enable_if cond is not met.
		// anyway...
		template <typename U, 
			typename std::enable_if<std::is_copy_constructible<T>::value>::type * = nullptr, 
			typename std::enable_if<std::is_convertible<U &&, T>::value>::type * = nullptr>
		CRANBERRIES_CXX11_CONSTEXPR T value_or(U &&default_value) const &{
			return hasvalue? storage.holder.get_ref(): static_cast<T>(std::forward<U>(default_value));
		}
		template <typename U, 
			typename std::enable_if<std::is_move_constructible<T>::value>::type * = nullptr, 
			typename std::enable_if<std::is_convertible<U &&, T>::value>::type * = nullptr>
		CRANBERRIES_CXX14_CONSTEXPR T value_or(U &&default_value) &&{
			return hasvalue? std::move(storage.holder).get_ref(): static_cast<T>(std::forward<U>(default_value));
		}

		// ====================== cranberries extenssions ======================

		// ====================== value_or_else ======================
		template <typename F,
			enabler_t<conjunction_v<
				std::is_move_constructible<T>,
				std::is_convertible<decltype(std::declval<F>()()), T>,
				::cranberries::is_callable<F(), T>
			>> = nullptr >
		CRANBERRIES_CXX11_CONSTEXPR T value_or_else(F &&f) const &{
			return hasvalue? storage.holder.get_ref(): static_cast<T>(f());
		}

		template <typename F,
			enabler_t<conjunction_v<
				std::is_move_constructible<T>,
				std::is_convertible<decltype(std::declval<F>()()), T>,
				::cranberries::is_callable<F(), T>
			>> = nullptr >
		CRANBERRIES_CXX14_CONSTEXPR T value_or_else(F &&f) &&{
			return hasvalue? std::move(storage.holder).get_ref(): static_cast<T>(f());
		}

		// ====================== map ======================
		template <typename F,
			enabler_t<::cranberries::is_callable_v<F(const T&)>> = nullptr >
			CRANBERRIES_CXX11_CONSTEXPR auto map(F &&f) const
			-> optional<::cranberries::invoke_result_t< F, const T& >>
		{
			using result_opt = optional<::cranberries::invoke_result_t< F, const T& >>;
			return hasvalue ? result_opt{ f(storage.holder.get_ref()) } : result_opt{ nullopt };
		}

		// ====================== map_or ======================
		template <typename F, typename U,
			enabler_t<std::is_convertible<decltype(f(std::declval<const T&>())), U>::value> = nullptr >
		CRANBERRIES_CXX11_CONSTEXPR auto map_or(F &&f, U&& u) const
			-> optional<decltype(f(std::declval<const T&>()))>
		{
			using result_opt = optional<decltype(f(std::declval<const T&>()))>;
			return hasvalue ? result_opt{ f(storage.holder.get_ref()) } : result_opt{ u };
		}

		// ====================== map_or_else ======================
		template <typename F, typename DefaultFn,
			enabler_t<std::is_convertible<decltype(f(std::declval<const T&>())), decltype(std::declval<DefaultFn>()())>::value> = nullptr >
		CRANBERRIES_CXX11_CONSTEXPR auto map_or_else(F &&f, DefaultFn&& default_fn) const
			-> optional<decltype(f(std::declval<const T&>()))>
		{
			using result_opt = optional<decltype(f(std::declval<const T&>()))>;
			return hasvalue ? result_opt{ f(storage.holder.get_ref()) } : result_opt{ default_fn() };
		}

		// ====================== and_ ======================
		template < typename U >
		CRANBERRIES_CXX11_CONSTEXPR optional<U> and_(const optional<U>& u) const
		{
			return hasvalue && u ? u : nullopt;
		}

		// ====================== and_then ======================
		template < typename F, enabler_t<::cranberries::is_callable_v<F(T), T>> = nullptr >
		CRANBERRIES_CXX11_CONSTEXPR auto and_then(F &&f) const
			-> optional<decltype(f(std::declval<const T&>()))>
		{
			using result_opt = optional<decltype(f(std::declval<const T&>()))>;
			return hasvalue ? result_opt{ f(storage.holder.get_ref()) } : result_opt{ nullopt };
		}


		// ====================== or_ ======================
		CRANBERRIES_CXX11_CONSTEXPR optional or_(const optional& x) const
		{
			return hasvalue ? storage.holder.get_ref(): (x ? x.value(): nullopt);
		}

		// ====================== or_else ======================
		template < typename F,
			enabler_t<std::is_same<remove_cvr_t<decltype(std::declval<F>()())>, optional>::value> = nullptr >
		CRANBERRIES_CXX11_CONSTEXPR optional or_else(F &&f) const
		{
			return hasvalue ? optional{ storage.holder.get_ref() } : f();
		}

		// ====================== swap ======================
		void swap(optional &_rhs) 
			noexcept(std::is_nothrow_move_constructible<T>::value && ::cranberries::is_nothrow_swappable<T>::value){
			using std::swap;
			if((!hasvalue) && (!(_rhs.hasvalue))){
				return ;
			}
			if(hasvalue && _rhs.hasvalue){
				swap(storage.holder.get_ref(), _rhs.storage.holder.get_ref());
			}

			optional *in = hasvalue? (this): (&_rhs);
			optional *un = hasvalue? (&_rhs): (this);
			new (un->getptr_impl()) holder_t(std::move(std::move(in->storage.holder).get_ref()));
			in->destroy();
			un->hasvalue = true;
			in->hasvalue = false;
			return ;
		}

		// ====================== reset ======================
		void reset() noexcept{
			if(hasvalue){
				destroy();
				hasvalue = false;
			}
		}

		// ====================== emplace ======================
		template <typename ...Args>
		T &emplace(Args &&...args){
			static_assert(std::is_constructible<T, Args &&...>::value, 
				"T must be constructible from Args...");
			if(hasvalue){
				destroy();
				hasvalue = false;
			}

			new (getptr_impl()) holder_t(std::true_type{}, std::forward<Args>(args)...);
			hasvalue = true;
			return static_cast<optional &>(*this).get_value();
		}

		template <typename U, typename ...Args, 
			typename std::enable_if<std::is_constructible<T, std::initializer_list<U> &, Args &&...>::value>::type * = nullptr>
		T &emplace(std::initializer_list<U> ilist, Args &&...args){
			if(hasvalue){
				destroy();
				hasvalue = false;
			}

			new (getptr_impl()) holder_t(std::true_type{}, ilist, std::forward<Args>(args)...);
			hasvalue = true;
			return static_cast<optional &>(*this).get_value();
		}

		friend std::ostream& operator<<(std::ostream& os, const optional& opt) {
			if (opt.hasvalue) return os << opt.storage.holder.get_ref();
			else return os << "(nullopt)";
		}

	private:
		// actually, it might not matter whether this method has SFINAE restriction on it or not.
		template <typename Dummy = T, 
			typename std::enable_if<!(sizeof(Dummy), std::is_trivially_destructible<T>::value)>::type * = nullptr>
		void destroy() noexcept{
			assert(hasvalue);
			storage.holder.holder_t::~holder_t();
		}
		template <typename Dummy = T, 
			typename std::enable_if<(sizeof(Dummy), std::is_trivially_destructible<T>::value)>::type * = nullptr>
		void destroy() noexcept{
			assert(hasvalue);
			// do nothing
		}

		storage_t storage;
		bool hasvalue;
	}; // class optional

	// ====================== operator==, !=, <, <=, >, >= ======================
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator==(const optional<T> &_lhs, const optional<U> &_rhs){
		return bool(_lhs) != bool(_rhs)? false: (bool(_lhs) == false? true: *_lhs == *_rhs);
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator!=(const optional<T> &_lhs, const optional<U> &_rhs){
		return bool(_lhs) != bool(_rhs)? true: (bool(_lhs) == false? false: *_lhs != *_rhs);
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator<(const optional<T> &_lhs, const optional<U> &_rhs){
		return bool(_rhs) == false? false: (bool(_lhs) == false? true: *_lhs < *_rhs);
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator<=(const optional<T> &_lhs, const optional<U> &_rhs){
		return bool(_lhs) == false? true: (bool(_rhs) == false? false: *_lhs <= *_rhs);
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator>(const optional<T> &_lhs, const optional<U> &_rhs){
		return bool(_lhs) == false? false: (bool(_rhs) == false? true: *_lhs > *_rhs);
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator>=(const optional<T> &_lhs, const optional<U> &_rhs){
		return bool(_rhs) == false? true: (bool(_lhs) == false? false: *_lhs >= *_rhs);
	}

	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator==(const optional<T> &opt, nullopt_t) noexcept{
		return !opt;
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator==(nullopt_t, const optional<T> &opt) noexcept{
		return !opt;
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator!=(const optional<T> &opt, nullopt_t) noexcept{
		return bool(opt);
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator!=(nullopt_t, const optional<T> &opt) noexcept{
		return bool(opt);
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator<(const optional<T> &opt, nullopt_t) noexcept{
		return false;
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator<(nullopt_t, const optional<T> &opt) noexcept{
		return bool(opt);
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator<=(const optional<T> &opt, nullopt_t) noexcept{
		return !opt;
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator<=(nullopt_t, const optional<T> &opt) noexcept{
		return true;
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator>(const optional<T> &opt, nullopt_t) noexcept{
		return bool(opt);
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator>(nullopt_t, const optional<T> &opt) noexcept{
		return false;
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator>=(const optional<T> &opt, nullopt_t) noexcept{
		return true;
	}
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR bool operator>=(nullopt_t, const optional<T> &opt) noexcept{
		return !opt;
	}

	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator==(const optional<T> &opt, const U &value){
		return bool(opt)? (*opt == value): false;
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator==(const U &value, const optional<T> &opt){
		return bool(opt)? (value == *opt): false;
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator!=(const optional<T> &opt, const U &value){
		return bool(opt)? (*opt != value): true;
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator!=(const U &value, const optional<T> &opt){
		return bool(opt)? (value != *opt): true;
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator<(const optional<T> &opt, const U &value){
		return bool(opt)? (*opt < value): true;
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator<(const U &value, const optional<T> &opt){
		return bool(opt)? (value < *opt): false;
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator<=(const optional<T> &opt, const U &value){
		return bool(opt)? (*opt <= value): true;
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator<=(const U &value, const optional<T> &opt){
		return bool(opt)? (value <= *opt): false;
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator>(const optional<T> &opt, const U &value){
		return bool(opt)? (*opt > value): false;
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator>(const U &value, const optional<T> &opt){
		return bool(opt)? (value > *opt): true;
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator>=(const optional<T> &opt, const U &value){
		return bool(opt)? (*opt >= value): false;
	}
	template <typename T, typename U>
	CRANBERRIES_CXX11_CONSTEXPR bool operator>=(const U &value, const optional<T> &opt){
		return bool(opt)? (value >= *opt): true;
	}

	// ====================== make_optional ======================
	template <typename T>
	CRANBERRIES_CXX11_CONSTEXPR optional<typename std::decay<T>::type> make_optional(T &&value){
		return optional<typename std::decay<T>::type>(std::forward<T>(value));
	}
	// T has to be moveable due to the lack of guaranteed copy elision in C++17
	template <typename T, typename ...Args>
	CRANBERRIES_CXX11_CONSTEXPR optional<T> make_optional(Args &&...args){
		return optional<T>(in_place, std::forward<Args>(args)...);
	}
	template <typename T, typename U, typename ...Args>
	CRANBERRIES_CXX11_CONSTEXPR optional<T> make_optional(std::initializer_list<U> ilist, Args &&...args){
		return optional<T>(in_place, ilist, std::forward<Args>(args)...);
	}

	template < typename T >
	CRANBERRIES_CXX11_CONSTEXPR optional<remove_cvr_t<T>> some(T&& value) {
		return make_optional(std::forward<T>(value));
	}

	// ====================== std::swap ======================
	template <typename T, 
		typename std::enable_if<std::is_move_constructible<T>::value && ::cranberries::is_swappable<T>::value>::type * = nullptr>
	void swap(optional<T> &_lhs, optional<T> &_rhs)
		noexcept(noexcept(_lhs.swap(_rhs))){
		_lhs.swap(_rhs);
	}

	// ====================== std::hash ======================

	namespace _detail{

		// probably check if a std::hash specialization is enabled 
		// or more probably just mess things up

		template <class>
		struct hash_argument_type;
		template <typename T>
		struct hash_argument_type<std::hash<T>>{
			using type = T;
		};

		struct has_hash_invoke_operator_helper{
			template <class T>
			static auto test(int) 
				-> typename std::is_same<
					decltype(std::declval<T>().operator()(std::declval<typename hash_argument_type<T>::type>())), 
					std::size_t
				>::type;
			template <typename>
			static std::false_type test(...);
		};

		template <class T>
		struct has_hash_invoke_operator_checker{
			constexpr static bool value = decltype(has_hash_invoke_operator_helper::test<T>(0))::value;
		};

		// if disabled hash reqiurement are not met, it does not matter if we mess things up more
		template <class>
		struct is_probably_hash_checker;
		template <typename T>
		struct is_probably_hash_checker<std::hash<T>>
			: public ::cranberries::conjunctional<
				has_hash_invoke_operator_checker,
				std::is_default_constructible,
				std::is_copy_constructible,
				std::is_move_constructible,
				std::is_copy_assignable,
				std::is_move_assignable,
				std::is_destructible>::template apply<std::hash<T>>
		{};

		template <typename T, bool is_probaby_hash>
		struct std_hash_optional_base{
			using argument_type = optional<T>;
			using result_type = std::size_t;

			std::size_t operator()(const optional<T> &key){
				if(key.has_value()){
					std::hash<typename std::remove_const<T>::type> underlying_hash;
					return underlying_hash(*key);
				}
				else{
					std::hash<void *> null_hash;
					return null_hash(nullptr);
				}
			}
			std::size_t operator()(optional<T> &key){
				if(key.has_value()){
					std::hash<typename std::remove_const<T>::type> underlying_hash;
					return underlying_hash(*key);
				}
				else{
					std::hash<void *> null_hash;
					return null_hash(nullptr);
				}
			}
			std::size_t operator()(const optional<T> &&key){
				if(key.has_value()){
					std::hash<typename std::remove_const<T>::type> underlying_hash;
					return underlying_hash(std::move(*std::move(key)));
				}
				else{
					std::hash<void *> null_hash;
					return null_hash(nullptr);
				}
			}
			std::size_t operator()(optional<T> &&key){
				if(key.has_value()){
					std::hash<typename std::remove_const<T>::type> underlying_hash;
					return underlying_hash(std::move(*std::move(key)));
				}
				else{
					std::hash<void *> null_hash;
					return null_hash(nullptr);
				}
			}
		};
		template <typename T>
		struct std_hash_optional_base<T, false>{
			std_hash_optional_base() = delete;
			std_hash_optional_base(const std_hash_optional_base &) = delete;
			std_hash_optional_base(std_hash_optional_base &&) = delete;
			std_hash_optional_base &operator=(const std_hash_optional_base &) = delete;
			std_hash_optional_base &operator=(std_hash_optional_base &&) = delete;
			~std_hash_optional_base() = delete;

			std::size_t operator()(const optional<T> &) = delete;
		};

	} // namespace _detail


}

#endif // CRANBERRIES_OPTIONAL_HPP