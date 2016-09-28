#ifndef CRANBERRIES_STREAMS_OPERATORS_SUMMARY_STAT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_SUMMARY_STAT_HPP
#include <utility>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include "..\stat\stat.hpp"
#include "..\utility.hpp"

#define PUTS(ARG) os << #ARG" : " << ARG << std::endl

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
    template < typename T >
    SummaryStat( std::vector<T>&& v ) : data_{ std::move( v ) } { calc(); }


  private:
    void calc() {
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
      IQR = upper_quantile - lower_quantile;
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
      ue_variance = size / (size - 1.0)*variance;
      deviation = std::sqrt( variance );

      for (auto&& e : data_) {
        skewness += std::pow( (e - average) / deviation, 3.0 );
        kurtosis += std::pow( (e - average) / deviation, 4.0 );
      }
      skewness /= size;
      kurtosis /= size;
    }
  public:
    void print(std::ostream& os) {
      PUTS( size );
      PUTS( min );
      PUTS( max );
      PUTS( value_range );
      PUTS( mode );
      PUTS( median );
      PUTS( lower_quantile );
      PUTS( upper_quantile );
      PUTS( IQR );
      PUTS( average );
      PUTS( variance );
      PUTS( ue_variance );
      PUTS( deviation );
      PUTS( skewness );
      PUTS( kurtosis );
    }


  public:
    size_t size;
    T min;
    T max;
    T value_range;
    std::common_type_t<double, T> lower_quantile;
    std::common_type_t<double, T> upper_quantile;
    std::common_type_t<double, T> IQR;
    std::common_type_t<double, T> median;
    std::common_type_t<double, T> mode;
    std::common_type_t<double, T> average;
    std::common_type_t<double, T> variance;
    std::common_type_t<double, T> ue_variance;
    std::common_type_t<double, T> deviation;
    std::common_type_t<double, T> skewness;
    std::common_type_t<double, T> kurtosis;

  private:
    std::vector<T> data_;
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#undef PUTS
#endif