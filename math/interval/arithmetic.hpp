#ifndef CRANBRIIES_INTERVAL_LIB_ARITHMETIC_HPP
#define CRANBRIIES_INTERVAL_LIB_ARITHMETIC_HPP

namespace cranberries {
namespace interval_lib
{
  //------------------------------//
  //                              //
  /*  Interval Member Functions   */
  //                              //
  //------------------------------//

  /*  Interval Increment/Decrement Operator Definition  */

  /*  Interval Increment Operator  */

  //  prefix increment
  template < typename T >
  inline interval<T> interval<T>::operator ++() noexcept
  {
    ++( *pImpl );
    return *this;
  }

  //  postfix increment
  template < typename T >
  inline interval<T> interval<T>::operator ++( int ) noexcept
  {
    auto tmp( *this );
    ++( *pImpl );
    return tmp;
  }

  /*  Interval Increment Operator  */

  //  prefix decrement
  template < typename T >
  inline interval<T> interval<T>::operator --() noexcept
  {
    --( *pImpl );
    return *this;
  }

  //  postfix decrement
  template < typename T >
  inline interval<T> interval<T>::operator --( int ) noexcept
  {
    auto tmp( *this );
    --( *pImpl );
    return tmp;
  }

  //----------------------------------------//
  //                                        //
  /*  Interval Numeric Function Definition  */
  //                                        //
  //----------------------------------------//


  template < typename T >
  inline interval<T> interval<T>::inverse() const
  {
    CRANBERRIES_OVERFLOW_ERROR_THROW_IF( pImpl->lower() <= 0.0 && 0.0 <= pImpl->upper() );
    return ACCURACY_ASSURANCE( 1.0 / pImpl->upper(), 1.0 / pImpl->lower() );
  }


} // ! interval_lib
} // ! cranberries
#endif // ! CRANBRIIES_INTERVAL_LIB_ARITHMETIC_HPP