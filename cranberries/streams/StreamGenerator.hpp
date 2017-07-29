#ifndef CRANBERRIES_STREAMS_STREAM_GENERATOR_HPP
#define CRANBERRIES_STREAMS_STREAM_GENERATOR_HPP
#include <utility>
#include <string>
#include <regex>
#include <type_traits>
#include <random>
#include <limits>
#include "forward.hpp"
#include "utility.hpp"
#include "cranberries_magic/tag.hpp"
#include "InfiniteStream.hpp"
#include "operators/Splitter.hpp"
#include "io.hpp"

namespace cranberries {
namespace streams {



  //------------------//
  // stream Generator //
  //------------------//



  template < typename T >
  struct Generator
  {
    T operator()() noexcept { return value_; }
    T value_;
  };

  template <
    typename ResultType,
    typename Distribution,
    typename Engine,
    typename Seed = unsigned
  >
  class UniformGen
  {
  public:
    UniformGen( ResultType low, ResultType up, Seed seed )
      : dist_( low, up )
      , engine_{ seed }
    {}

    ResultType operator()() noexcept { return dist_( engine_ ); }
  
  private:
    Distribution dist_;
    Engine engine_;
  };

  template <
    typename ResultType,
    typename Init,
    typename Distribution,
    typename Engine,
    typename Seed = unsigned
  >
  class GeneralizedDistGen
  {
  public:
    GeneralizedDistGen( Init init, Seed seed ) noexcept
      : dist_{ init }
      , engine_{ seed }
    {}

    GeneralizedDistGen( Init init1, Init init2, Seed seed ) noexcept
      : dist_( init1, init2 )
      , engine_{ seed }
    {}

    ResultType operator()() noexcept { return dist_( engine_ ); }

  private:
    Distribution dist_;
    Engine engine_;
  };

  template <
    typename RealType,
    size_t Bits,
    typename Engine
  >
  class CanonicalGen
  {
  public:
    CanonicalGen( unsigned seed ) noexcept
      : engine_{ seed }
    {}

    RealType operator()() noexcept { return std::generate_canonical<RealType, Bits>( engine_ ); }

  private:
    Engine engine_;
  };

  struct make_stream
  {
    template < typename T >
    static stream<T> empty() noexcept {
      return{};
    }

    template <
      typename Iterator,
      typename T = element_type_of_t<Iterator>
    >
    static
    stream<T>
    of
    (
      Iterator&& first,
      Iterator&& last
    )
      noexcept
    {
      return stream<T>{ std::forward<Iterator>(first), std::forward<Iterator>(last) };
    }

    template <
      typename T
    >
    static
    stream<T>
    of
    (
      std::initializer_list<T> il
    )
      noexcept
    {
      return stream<T>{ il };
    }


    template <
      typename Range,
      enabler_t<is_range_v<Range>> = nullptr
    >
    static
    auto
    from
    (
      Range&& range
    )
      noexcept
    {
      return stream<element_type_of_t<Range>>{ range };
    }

    static
    stream<std::string>
    from
    (
      operators::Splitter&& sp
    ) {
      auto&& str = std::move(sp.target_);
      auto&& r_ = std::move(sp.r_);
      return {
        std::sregex_token_iterator{ str.begin(), str.end(), r_, -1 },
        std::sregex_token_iterator{}
      };
    }

    template <
      typename T
    >
    static
    stream<T, Ranged<T>>
    range
    (
      T first,
      T last,
      T step = 1
    ) {
      return { std::vector<T>{}, Ranged<T>{first, last, step} };
    }

    template <
      typename T,
      typename UnaryOperator
    >
    static
    IterateStream<T, UnaryOperator>
    iterate
    (
      T&& seed_,
      UnaryOperator&& operator_
    )
      noexcept
    {
      return { std::forward<T>(seed_), std::forward<UnaryOperator>(operator_) };
    }

    template <
      typename Supplier
    >
    static
    GenerateStream<std::result_of_t<Supplier(void)>, Supplier>
    generate
    (
      Supplier&& supplier_
    )
      noexcept
    {
      return { std::forward<Supplier>( supplier_ ) };
    }


    template <
      typename Iterable
    >
    static
    CountingStream<Iterable>
    counter
    (
      Iterable first_
    )
      noexcept
    {
      return { first_ };
    }

    template <
      typename T
    >
    static
    GenerateStream<T, Generator<T>>
    repeat
    (
      T val
    )
      noexcept
    {
      return { Generator<T>{val} };
    }

    template <
      typename T
    >
    static
    stream<T>
    repeat
    (
      T val,
      size_t lim
    )
      noexcept
    {
      return {std::vector<T>(lim, val)};
    }

    template <
      typename Range,
      typename T = element_type_of_t<Range>,
      enabler_t<
        is_range_v<Range>
      > = nullptr
    >
    static
    CyclicStream<T>
    cyclic
    (
      Range&& range
    )
      noexcept
    {
      return { std::forward<Range>(range) };
    }

    template <
      typename T
    >
    static
    CyclicStream<T>
    cyclic
    (
      std::initializer_list<T> il
    )
      noexcept
    {
      return { il };
    }

    template <
      typename Iterator,
      typename T = element_type_of_t<Iterator>,
      enabler_t<
        is_iterator_v<Iterator>
      > = nullptr
    >
    static
    CyclicStream<T>
    cyclic
    (
      Iterator&& first,
      Iterator&& last
    )
      noexcept
    {
      return { std::forward<Iterator>( first ), std::forward<Iterator>( last ) };
    }

    //------------------------------------------------------//

    //                     Flie Streams                     //

    //------------------------------------------------------//
    template <
      typename T = std::string
    >
    static
    stream<T, operators::Identity>
    fopen
    (
      std::string path,
      std::ios_base::openmode openmode = std::ios_base::in | std::ios_base::out
    ) {
      return { path, openmode };
    }


    //------------------------------------------------------//


    //                     Random Streams                   //


    //------------------------------------------------------//


    //--------------------------//
    // Random Stream (Infinite) //
    //--------------------------//


    template <
      typename Engine = std::mt19937,
      typename Seed = unsigned
    >
    static
    GenerateStream<Seed, Engine>
    random
    (
      Seed&& seed = std::random_device{}()
    )
      noexcept
    {
      return{ Engine{seed} };
    }

    //-----------------------------------------------//
    // Uniform Distribution Random Stream (Infinite) //
    //-----------------------------------------------//

    template <
      typename ResultType,
      typename Engine = std::mt19937,
      typename Seed = unsigned
    >
    static
    GenerateStream< ResultType,
      UniformGen< ResultType,
        std::conditional_t<
          std::is_integral<ResultType>::value,
          std::uniform_int_distribution<ResultType>,
          std::uniform_real_distribution<ResultType>
        >,
        Engine, Seed
      >
    >
    uniform_dist
    (
      ResultType min,
      ResultType max,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { min, max, seed } };
    }

    //-------------------------------------------------//
    // Bernoulli Distribution Random Stream (Infinite) //
    //-------------------------------------------------//

    template <
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      bool,
      GeneralizedDistGen<
        bool,
        Init,
        std::bernoulli_distribution,
        Engine,
        Seed
      >
    >
    bernoulli_dist
    (
      Init probability,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { probability, seed } };
    }

    template <
      typename IntType = int,
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      IntType,
      GeneralizedDistGen<
        IntType,
        Init,
        std::poisson_distribution<IntType>,
        Engine,
        Seed
      >
    >
    poisson_dist
    (
      Init avarage,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { avarage, seed } };
    }


    template <
      typename RealType = double,
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      RealType,
      GeneralizedDistGen<
        RealType,
        Init,
        std::exponential_distribution<RealType>,
        Engine,
        Seed
      >
    >
    exponential_dist
    (
      Init exponent,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { exponent, seed } };
    }

    template <
      typename RealType = double,
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      RealType,
      GeneralizedDistGen<
        RealType,
        Init,
        std::gamma_distribution<RealType>,
        Engine,
        Seed
      >
    >
    gamma_dist
    (
      Init shape_param,
      Init scale_param,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { shape_param, scale_param, seed } };
    }

    template <
      typename RealType = double,
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      RealType,
      GeneralizedDistGen<
        RealType,
        Init,
        std::weibull_distribution<RealType>,
        Engine,
        Seed
      >
    >
    weibull_dist
    (
      Init shape_param,
      Init scale_param,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { shape_param, scale_param, seed } };
    }

    template <
      typename RealType = double,
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      RealType,
      GeneralizedDistGen<
        RealType,
        Init,
        std::extreme_value_distribution<RealType>,
        Engine,
        Seed
      >
    >
    extreme_value_dist
    (
      Init location_param,
      Init scale_param,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { location_param, scale_param, seed } };
    }

    //----------------------------------------------//
    // Normal Distribution Random Stream (Infinite) //
    //----------------------------------------------//

    template <
      typename RealType = double,
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      RealType,
      GeneralizedDistGen<
        RealType,
        Init,
        std::normal_distribution<RealType>,
        Engine,
        Seed
      >
    >
    normal_dist
    (
      Init average,
      Init standard_deviation,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { average, standard_deviation, seed } };
    }

    template <
      typename RealType = double,
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      RealType,
      GeneralizedDistGen<
        RealType,
        Init,
        std::lognormal_distribution<RealType>,
        Engine,
        Seed
      >
    >
    lognormal_dist
    (
      Init average,
      Init standard_deviation,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { average, standard_deviation, seed } };
    }

    template <
      typename RealType = double,
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      RealType,
      GeneralizedDistGen<
        RealType,
        Init,
        std::chi_squared_distribution<RealType>,
        Engine,
        Seed
      >
    >
    chi_squared_dist
    (
      Init degree_of_freedom,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { degree_of_freedom, seed } };
    }

    template <
      typename RealType = double,
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      RealType,
      GeneralizedDistGen<
        RealType,
        Init,
        std::cauchy_distribution<RealType>,
        Engine,
        Seed
      >
    >
    cauchy_dist
    (
      Init location_param,
      Init scale_param,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { location_param, scale_param, seed } };
    }

    template <
      typename RealType = double,
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      RealType,
      GeneralizedDistGen<
        RealType,
        Init,
        std::fisher_f_distribution<RealType>,
        Engine,
        Seed
      >
    >
    fisher_f_dist
    (
      Init degree_of_freedom1,
      Init degree_of_freedom2,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { degree_of_freedom1, degree_of_freedom2, seed } };
    }
 
    template <
      typename RealType = double,
      typename Engine = std::mt19937,
      typename Init,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      RealType,
      GeneralizedDistGen<
        RealType,
        Init,
        std::student_t_distribution<RealType>,
        Engine,
        Seed
      >
    >
    student_t_dist
    (
      Init degree_of_freedom,
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { degree_of_freedom, seed } };
    }
 
    /***************************************************
    
    
    | Uniform Distribution Random Stream (Infinite)  
    | that has been deployed (virtually normalized)  
    | to the real interval [ 0.0, 1.0 ).             
    
    
    ***************************************************/


    template <
      typename RealType = double,
      size_t Bits = std::numeric_limits<RealType>::digits,
      typename Engine = std::mt19937,
      typename Seed = unsigned
    >
    static
    GenerateStream<
      RealType,
      CanonicalGen<RealType,Bits,Engine>
    >
    generate_canonical
    (
      Seed seed = std::random_device{}()
    )
      noexcept
    {
      return{ { seed } };
    }
  };


  template <
    typename T
  >
  class stream_builder 
  {
  public:
    stream_builder() = default;
    stream_builder(stream_builder const&) = default;
    stream_builder(stream_builder&&) = default;
    ~stream_builder() = default;

    stream_builder& add(T const& a) & noexcept {
      data_.emplace_back( a );
      return *this;
    };

    stream_builder&& add( T const& a ) && noexcept {
      data_.emplace_back( a );
      return std::move( *this );
    };

    stream_builder& operator +=( T const& a ) & noexcept {
      data_.emplace_back( a );
      return *this;
    }

    stream_builder&& operator +=( T const& a ) && noexcept {
      data_.emplace_back( a );
      return std::move( *this );
    }


    stream<T> build() noexcept {
      return{ std::move(data_) };
    }
  private:
    std::vector<T> data_;
  };

  class stream_builder_t {};

  constexpr auto build = stream_builder_t{};

  template < typename T >
  inline stream_builder<T>& operator << ( stream_builder<T>& sb, T const& a ) noexcept {
    return sb += a;
  }

  template < typename T >
  inline stream_builder<T>&& operator << ( stream_builder<T>&& sb, T const& a ) noexcept {
    return std::move( sb += a );
  }

  template < typename T >
  inline auto operator << ( stream_builder<T>& sb, stream_builder_t) noexcept {
    return sb.build();
  }

  template < typename T >
  inline auto operator << ( stream_builder<T>&& sb, stream_builder_t ) noexcept {
    return sb.build();
  }



} // ! namespace stream
} // ! namespace cranberries

#endif