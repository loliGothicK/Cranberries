#ifndef CRANBERRIES_STREAMS_OPERATORS_SUMMARY_STAT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_SUMMARY_STAT_HPP
#include <utility>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include "../stat/stat.hpp"
#include "../utility.hpp"

#define CRANBERRIES_PUTS(MSG, ARG) os << MSG" : " << ARG << std::endl

namespace cranberries {
namespace streams {
namespace operators {

  struct SummaryStatProxy {};

  template <
    typename T
  >
  class SummaryStat
  {
  public:
    SummaryStat( std::vector<T>&& v ) : data_{ std::move( v ) } { calc(); }


  private:
    void calc() noexcept {
      std::sort( data_.begin(), data_.end() );
      size = data_.size();
      min = *data_.begin();
      max = *(data_.end()-1);
      value_range = max - min;

      median = size % 2 == 0
        ? (*(data_.begin() + size / 2) + *(data_.begin() + size / 2 + 1)) / 2.0
        : static_cast<std::common_type_t<double, T>>(*(data_.begin() + size / 2 + 1));
      lower_quantile = (*(data_.begin() + size / 4) + *(data_.begin() + size / 4 + 1)) / 2.0;
      upper_quantile = (*(data_.begin() + static_cast<size_t>(std::ceil( size*0.75 ))) + *(data_.begin() + static_cast<size_t>(std::ceil( size*0.75 )) + 1)) / 2.0;
      interquartile_range = upper_quantile - lower_quantile;
      // mode
      auto mid = data_.begin(), prev = data_.begin();
      auto end = data_.end();
      auto w = (*prev - *end) / 5.0;
      int n{};
      for (size_t i{ 1 }; i < 5; ++i) {
        mid = std::upper_bound( data_.begin(), data_.end(), w*i );
        if (n < std::distance( prev, mid )) mode = cranberries::stat::median( prev, mid );
        prev = mid;
      }
      if (n < std::distance( prev, end )) mode = cranberries::stat::median( prev, end );

      std::common_type_t<double, T> squared_sum{};
      for (auto&& e : data_) {
        average += e;
        squared_sum += e*e;
      }
      average /= size;
      variance = squared_sum / size - average*average;
      unbiased_variance = size / (size - 1.0)*variance;
      deviation = std::sqrt( variance );

      for (auto&& e : data_) {
        skewness += std::pow( (e - average) / deviation, 3.0 );
        kurtosis += std::pow( (e - average) / deviation, 4.0 );
      }
      skewness /= size;
      kurtosis /= size;
    }
  public:
    std::ostream& print(std::ostream& os) const noexcept
    {
      CRANBERRIES_PUTS( "size", size );
      CRANBERRIES_PUTS( "min", min );
      CRANBERRIES_PUTS( "max", max );
      CRANBERRIES_PUTS( "value range", value_range );
      CRANBERRIES_PUTS( "mode", mode );
      CRANBERRIES_PUTS( "median", median );
      CRANBERRIES_PUTS( "lower quantile", lower_quantile );
      CRANBERRIES_PUTS( "upper_quantile", upper_quantile );
      CRANBERRIES_PUTS( "interquartile range", interquartile_range );
      CRANBERRIES_PUTS( "average", average );
      CRANBERRIES_PUTS( "variance", variance );
      CRANBERRIES_PUTS( "unbiased variance", unbiased_variance );
      CRANBERRIES_PUTS( "deviation", deviation );
      CRANBERRIES_PUTS( "skewness", skewness );
      CRANBERRIES_PUTS( "kurtosis", kurtosis );

      return os;
    }


  public:
    size_t size{};
    T min{};
    T max{};
    T value_range{};
    std::common_type_t<double, T> lower_quantile{};
    std::common_type_t<double, T> upper_quantile{};
    std::common_type_t<double, T> interquartile_range{};
    std::common_type_t<double, T> median{};
    std::common_type_t<double, T> mode{};
    std::common_type_t<double, T> average{};
    std::common_type_t<double, T> variance{};
    std::common_type_t<double, T> unbiased_variance{};
    std::common_type_t<double, T> deviation{};
    std::common_type_t<double, T> skewness{};
    std::common_type_t<double, T> kurtosis{};

  private:
    std::vector<T> data_;
  };

  template < typename T >
  inline std::ostream& operator<< ( std::ostream& os, SummaryStat<T> const& ss ) {
    return ss.print( os );
  }

  template < typename T, bool B >
  inline std::ostream& operator >> ( SummaryStat<T> const& ss, Printer<B>&& p) {
    return ss.print( p.os_ );
  }

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#undef CRANBERRIES_PUTS
#endif