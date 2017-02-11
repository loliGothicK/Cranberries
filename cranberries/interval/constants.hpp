#ifndef CRAMBERRIES_INTERVAL_LIB_CONSTANTS_HPP
#define CRAMBERRIES_INTERVAL_LIB_CONSTANTS_HPP

#include <limits>
#include "interval.hpp"

namespace cranberries {
namespace interval_lib {
namespace constants
{
  /* Contants */

  // Mathematical and physical constants.
  // These are defined variable templates.
  // Permitted template parameters are interval or arithmetic type.
  // These physical constansts using 2014 CODATA recommended values.
  // Choice SI UNIT.
  // Defined canstants are simple.
  /*
  [Example:Pi is defined value. Let 'T' is arithmetic type.
  pi<T> = static_cast<T>(3.141592653589793238462643383279L);
  pi<interval<T>> = interval<T>{ static_cast<T>(3.141592653589793238462643383279L), static_cast<T>(3.141592653589793238462643383279L) }
  - end example]
  *//*
  [Note:About standard measurement uncertainty.
  Constants as interval considering uncertainty.
  - end note]
  *//*
  [Example:Constant of gravitation.
  Standard measurement uncertainty is 4.7×10^−5.
  G = (6.67408±0.00031)×10^−11.
  g<interval<T>> = interval<T>{ (6.67408-0.00031)E-11, (6.67408+0.00031)E-11 };
  - end example]
  */
  // Constants detail.
  // Traits wrapper class.
  namespace constants_detail
  {
    template < typename T >
    struct Traits
    {
      CRANBERRIES_CONCEPT_ASSERT( std::is_arithmetic<T>::value && std::is_signed<T>::value );
      template < typename U >
      static T invoke( U&& x ) { return static_cast<T>( x ); }

      template < typename U >
      static T invoke( U&& x, U ) { return static_cast<T>( x ); }
    };

    template < typename T >
    struct Traits<interval<T>>
    {
      CRANBERRIES_CONCEPT_ASSERT( std::is_arithmetic<T>::value && std::is_signed<T>::value );
      template < typename U >
      static interval<T> invoke( U x ) {
        DOWNWARD_POLICY;
        auto l = static_cast<T>( x );
        UPWARD_POLICY;
        auto r = static_cast<T>( x );
        return interval<T>{ l, r };
      }
      template < typename U >
      static interval<T> invoke( U x, U delta ) {
        DOWNWARD_POLICY;
        auto l = static_cast<T>( x - delta );
        UPWARD_POLICY;
        auto r = static_cast<T>( x + delta );
        return interval<T>{ l, r };
      }
    };
  }
  inline namespace basics
  {
    template < typename T = double >
    T max = std::numeric_limits<T>::max();

    template < typename T = double >
    T zero = constants_detail::Traits<T>::invoke( 0.0L );

    template < typename T = double >
    T one = constants_detail::Traits<T>::invoke( 1.0L );

    template < typename T = double >
    T inf = std::numeric_limits<T>::infinity();
  }

  inline namespace mathematics
  {
    // the ratio of the circumference of a circle to its diameter
    template < typename T = double >
    T pi = constants_detail::Traits<T>::invoke( 3.141592653589793238462643383279L );

    // Napier's constant
    template < typename T = double >
    T e = constants_detail::Traits<T>::invoke( 2.718281828459045235360287471352L );

    // log_e(2)
    template < typename T = double >
    T ln2 = constants_detail::Traits<T>::invoke( 0.693147180559945309417232121458L );

    // log_e(10)
    template < typename T = double >
    T ln10 = constants_detail::Traits<T>::invoke( 2.302585092994045684017991454684L );

    // golden ratio
    template < typename T = double >
    T golden = constants_detail::Traits<T>::invoke( 1.61803398874989484820458683436563811L );
  }

  inline namespace physics
  {
    inline namespace universal
    {
      // speed of light in vacuum
      // UNIT : m·s^-1
      // defined value
      template < typename T = double >
      T c = constants_detail::Traits<T>::invoke( 299792458.0L );

      // Newtonian constant of gravitation
      // UNIT : m^3·kg^-1·s^-2
      // standard measurement uncertainty : 4.7×10^−5
      template < typename T = double >
      T G = constants_detail::Traits<T>::invoke( 6.67408E-11L, 0.00031E-11L );

      // Plank constant
      // UNIT : J·s
      // standard measurement uncertainty : 1.2×10^−8
      template < typename T = double >
      T Planck = constants_detail::Traits<T>::invoke( 6.626070040E-34L, 0.000000081E-34L );

      // reduced Planck constant
      // UNIT : J·s
      // standard measurement uncertainty : 1.2×10^−8
      template < typename T = double >
      T rPlanck = constants_detail::Traits<T>::invoke( 1.0545718E-34L, 0.000000013E-34L );
    }

    inline namespace electromagnetism
    {
      // magnetic constant (vacuum permeability)
      // UNIT : N·A^−2
      // defined value
      template < typename T = double >
      T mu_0 = constants_detail::Traits<T>::invoke( 1.256637061435917295385057E-6L );

      // electric constant (vacuum permittivity)
      // UNIT : F·m^−1
      // defined value
      template < typename T = double >
      T epsilon_0 = constants_detail::Traits<T>::invoke( 8.854187817L );

      // characteristic impedance of vacuum
      // UNIT : Ω
      // defined value
      template < typename T = double >
      T Z_0 = constants_detail::Traits<T>::invoke( 376.730313461L );

      // Coulomb's constant
      // UNIT : N·m^2·C^−2
      // defined value
      template < typename T = double >
      T k_e = constants_detail::Traits<T>::invoke( 8.854187817620389850536563E9L );

      // elementary charge
      // UNIT : C
      // standard measurement uncertainty : 2.2×10^−8
      template < typename T = double >
      T e_charge = constants_detail::Traits<T>::invoke( 1.6021766208E-19L, 0.0000000098E-19L );

      // Bohr magneton
      // UNIT : J·T^−1
      // standard measurement uncertainty : 2.2×10^−8
      template < typename T = double >
      T mu_b = constants_detail::Traits<T>::invoke( 9.27400968E-24L, 0.0000002E-24L );

      // conductance quantum
      // UNIT : S
      // standard measurement uncertainty : 3.2×10^-10
      template < typename T = double >
      T G_0 = constants_detail::Traits<T>::invoke( 7.7480917310E-5L, 0.0000000018E-5L );

      // inverse conductance quantum
      // UNIT : S
      // standard measurement uncertainty : 3.2×10^-10
      template < typename T = double >
      T G_0_inv = constants_detail::Traits<T>::invoke( 12906.4037217, 0.0000042 );

      // magnetic flux quantum
      // UNIT : Wb
      // standard measurement uncertainty : 2.2×10−8
      template < typename T = double >
      T phi_0 = constants_detail::Traits<T>::invoke( 2.067833831E-15L, 0.000000013E-15L );

      // Nuclear magneton
      // UNIT : J·T^−1
      // standard measurement uncertainty : 2.2×10−8
      template < typename T = double >
      T mu_N = constants_detail::Traits<T>::invoke( 5.05078353E-27L, 0.00000011E-27L );

      // von Klitzing constant
      // UNIT : Ω
      // standard measurement uncertainty : 3.2×10^-10
      template < typename T = double >
      T R_k = constants_detail::Traits<T>::invoke( 25812.8074434, 0.0000084 );
    }
    inline namespace atomic_and_nuclear
    {
      // Bohr radius
      // UNIT : m
      // standard measurement uncertainty  : 3.2×10^-9
      template < typename T = double >
      T a_0 = constants_detail::Traits<T>::invoke( 5.2917721092E-11L, 0.0000000017E-11L );

      // classical electron radius
      // UNIT : m
      // standard measurement uncertainty  : 9.7×10^-10
      template < typename T = double >
      T r_e = constants_detail::Traits<T>::invoke( 2.8179403267E-15L, 0.0000000027E-15L );

      // electron mass
      // UNIT : kg
      // standard measurement uncertainty  : 4.4×10^8
      template < typename T = double >
      T m_e = constants_detail::Traits<T>::invoke( 9.10938291E-31L, 0.0000004E-31L );

      // Fermi coupling constant
      // UNIT : GeV^−2
      // standard measurement uncertainty  : 4.3×10^-6
      template < typename T = double >
      T G_F = constants_detail::Traits<T>::invoke( 1.166364E-5L, 0.000005E-5L );

      // fine-structure constant
      // UNIT : dimensionless
      // standard measurement uncertainty  : 3.2×10^-10
      template < typename T = double >
      T alpha = constants_detail::Traits<T>::invoke( 7.2973525698E-3L, 0.0000000024E-3L );

      // Hartree energy
      // UNIT : J
      // standard measurement uncertainty  : 4.4×10^-8
      template < typename T = double >
      T E_h = constants_detail::Traits<T>::invoke( 4.35974434E-18L, 0.00000019E-18L );

      // proton mass
      // UNIT : kg
      // standard measurement uncertainty  : 4.4×10^-8
      template < typename T = double >
      T m_p = constants_detail::Traits<T>::invoke( 1.672621777E-27L, 0.000000074E-27L );

      // Rydberg constant
      // UNIT : m^-1
      // standard measurement uncertainty  : 5.0×10^-12
      template < typename T = double >
      T R_inf = constants_detail::Traits<T>::invoke( 10973731.568539, 0.000055 );

      // weak mixing angle
      // UNIT : dimensionless
      // standard measurement uncertainty  : 9.5×10^-3
      template < typename T = double >
      T wma = constants_detail::Traits<T>::invoke( 0.2223, 0.0021 );
    }
    inline namespace physico_chemical
    {
      // 
      // standard measurement uncertainty : 
      template < typename T = double >
      T N_A = constants_detail::Traits<T>::invoke( 6.022140857E-23L, 0.000000074E-23L );

      // 
      // standard measurement uncertainty : 
      template < typename T = double >
      T k = constants_detail::Traits<T>::invoke( 1.38064852E-23L, 0.00000079E-23L );

      // Faraday constant
      // standard measurement uncertainty : 6.2×10^−9
      template < typename T = double >
      T F = constants_detail::Traits<T>::invoke( 96485.33289, 0.00059 );

      // Loschmidt constant
      // UNIT : m^-3
      // standard measurement uncertainty  : 9.1×10^-7
      template < typename T = double >
      T n_0 = constants_detail::Traits<T>::invoke( 2.6867805E25L, 0.0000024E25L );

      // gas constant
      // UNIT : J·K^−1·mol^−1
      // standard measurement uncertainty  : 9.1×10^-7
      template < typename T = double >
      T R_0 = constants_detail::Traits<T>::invoke( 8.3144621, 0.0000075 );

      // molar volume of an ideal gas
      // UNIT : m^3·mol^−1
      // standard measurement uncertainty  : 9.1×10^-7
      template < typename T = double >
      T V_m = constants_detail::Traits<T>::invoke( 2.2413968E-2L, 0.000002E-2L );

      // Sackur-Tetrode constant
      // UNIT : demensionless
      // standard measurement uncertainty  : 1.9×10^-6
      template < typename T = double >
      T S_0_per_R = constants_detail::Traits<T>::invoke( -1.16487, 0.000023 );
    }
  }

} // ! cranberries
} // ! interval_lib
} // ! constants

#endif // ! CRAMBERRIES_INTERVAL_LIB_CONSTANTS_HPP