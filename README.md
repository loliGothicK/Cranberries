![logo](https://github.com/LoliGothick/Cranberries/blob/master/icon/logo_with_txt.png)

雑多な趣味コーディングによる成果物の墓場です

# Build Status

## Linux with GCC, Clang
[![CircleCI](https://circleci.com/gh/LoliGothick/Cranberries/tree/master.svg?style=svg)](https://circleci.com/gh/LoliGothick/Cranberries/tree/master)


## Windows with Visual Studio
[![Build status](https://ci.appveyor.com/api/projects/status/bk0tjc41t7db0ome/branch/master?svg=true)](https://ci.appveyor.com/project/LoliGothick/cranberries/branch/master) 
 

# License

These codes are licensed under CC0.

[![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png "CC0")](http://creativecommons.org/publicdomain/zero/1.0/deed.ja)

# Overview

- **Interval Arithmetic Library(区間演算ライブラリ)**
  - cranberries/interval.hpp

- **Time Measurement Library(実行時間計測ライブラリ)**
  - cranberries/stopwatch.hpp

- **Interpolation Library(補間法ライブラリ : 最小２乗法, チェビシェフ多項式近似)**
  - cranberries/interpolation.hpp

- **Dual Number Library for AD(自動微分用の二重数ライブラリ)**
  - cranberries/dual_number.hpp

- **Traversal Library(Range走査ライブラリ)**
  - cranberries/traversal/**/**.hpp

- **Type Traits Library（メタ関数ライブラリ）**
  - cranberries/type_traits.hpp

- **Utility Library（ユーティリティーライブラリ）**
  - cranberries/utility.hpp（以下が全て含まれる）
  - cranberries/utility/utility.hpp (pack_operations/integers に依存しない部分)
  - cranberries/utility/reverse_index_sequence.hpp (pack_operationsに依存するreverse_index_sequenceとreverse_applyが含まれる)
  - cranberries/utility/byte_swap.hpp (integersに依存するbyte_swapが含まれる)
        

- **Pack Operations Library (型レベルパック操作ライブラリ)**
  - cranberries/pack_operations.hpp

- **Function Utility Library (関数ユーティリティーライブラリ)**
  - cranberries/func_util.hpp（以下が全て含まれる）
  - cranberries/func_util/curry.hpp (カリー化)
  - cranberries/func_util/composition.hpp (関数合成)
  - cranberries/func_util/apply_pipe.hpp (関数に引数を適用するユーティリティー群)

- **Meta Bind Library (メタ関数バインドとメタ関数合成ライブラリ)**
  - cranberries/meta_bind.hpp

- **Math Library (数学ライブラリ)**
  - cranberries/math.hpp

- **Algorithm Library (アルゴリズムライブラリ)**
  - cranberries/algorithm.hpp
