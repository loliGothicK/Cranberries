/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIESE_INTERVAL_LIB_VERSION_HPP
#define CRANBERRIESE_INTERVAL_LIB_VERSION_HPP

namespace cranberries {

  /*  version tag  */
  enum class interval_version
  {
    v1_0_0 = 0x01000000,
    v1_0_1,
    v1_0_2,
    v1_0_3,
    v1_0_4,
    v1_0_5,
    v1_0_6,
    v1_0_7,
    v1_0_8,
    v1_1_0 = 0x01010000,
    v1_1_1,
    v1_2_0 = 0x01020000,
    v2_0_0 = 0x02000000,
    v2_0_1,
    v2_0_2,
    v2_1_0 = 0x02010000,
    // Support Expression Template
    v3_0_0 = 0x03000000,
    v3_0_1,
    v3_1_0 = 0x03010000,
    // Bug fix
    v4_0_0 = 0x04000000,
    v4_1_0 = 0x04010000,
    // conditoinal operator
    v5_0_0 = 0x05000000,
    v6_0_0 = 0x06000000,
  	v6_0_1,
    // constexpr
    v7_0_0 = 0x07000000,
    stable = v7_0_0,
    latest = v7_0_0,
  };

} // ! cranbrries

#endif 