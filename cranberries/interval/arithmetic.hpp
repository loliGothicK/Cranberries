#ifndef CRANBRIIES_INTERVAL_LIB_ARITHMETIC_HPP
#define CRANBRIIES_INTERVAL_LIB_ARITHMETIC_HPP

namespace cranberries {

  //------------------------------//
  //                              //
  /*  Interval Member Functions   */
  //                              //
  //------------------------------//

  /*  Interval Increment/Decrement Operator Definition  */

  /*  Interval Increment Operator  */

  //  prefix increment
  template < typename T >
  inline constexpr interval<T> interval<T>::operator ++() noexcept
  {
    ++lower_;
    ++upper_;
    return *this;
  }

  //  postfix increment
  template < typename T >
  inline constexpr interval<T> interval<T>::operator ++( int ) noexcept
  {
    auto tmp( *this );
    --(*this);
    return tmp;
  }

  /*  Interval Increment Operator  */

  //  prefix decrement
  template < typename T >
  inline constexpr interval<T> interval<T>::operator --() noexcept
  {
    --lower_;
    --upper_;
    return *this;
  }

  //  postfix decrement
  template < typename T >
  inline constexpr interval<T> interval<T>::operator --( int ) noexcept
  {
    auto tmp( *this );
    --(*this);
    return tmp;
  }

  //----------------------------------------//
  //                                        //
  /*  Interval Numeric Function Definition  */
  //                                        //
  //----------------------------------------//


  template < typename T >
  inline constexpr interval<T> interval<T>::inverse() const
  {
    return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( lower() <= 0.0 && 0.0 <= upper() )
      : ACCURACY_ASSURANCE( 1.0 / upper(), 1.0 / lower() );
  }


} // ! cranberries
#endif // ! CRANBRIIES_INTERVAL_LIB_ARITHMETIC_HPP