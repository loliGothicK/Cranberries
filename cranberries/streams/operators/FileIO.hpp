/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATORS_FILE_IO_HPP
#define CRANBERRIES_STREAMS_OPERATORS_FILE_IO_HPP
#include <type_traits>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "../utility.hpp"
#include "../concept.hpp"
#include "../../common/defaulted_type.hpp"


namespace cranberries {
namespace streams {
namespace operators {

  class Fopen 
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    Fopen() = default;
    Fopen(Fopen const&) = default;
    Fopen(Fopen&&) = default;
    ~Fopen() = default;
    Fopen(std::string path) : path_{ path } {}

    template <
      typename FileStream
    >
    decltype(auto)
    operator()(FileStream&& filestream_) {
      filestream_.fopen(path_);
      return std::forward<FileStream>(filestream_);
    }

  private:
    std::string path_{};
  };
  
  class Read
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    Read() = default;
    Read(Read const&) = default;
    Read(Read&&) = default;
    ~Read() = default;

    template <
      typename FileStream,
      typename T = typename std::decay_t<FileStream>::element_type
    >
    decltype(auto)
    operator()(FileStream&& filestream_) {
      auto ifs = std::ifstream{ filestream_.path(), filestream_.openmode() };
      T buf;
      CRANBERRIES_STREAM_FOPEN_ERROR_THROW_IF(!ifs.is_open());
      while (ifs >> buf) {
        filestream_.emplace_back(buf);
      }
      return std::forward<FileStream>(filestream_);
    }
  };

  class ReadLine
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    ReadLine() = default;
    ReadLine(ReadLine const&) = default;
    ReadLine(ReadLine&&) = default;
    ~ReadLine() = default;

    template <
      typename FileStream,
      typename T = typename std::decay_t<FileStream>::element_type
    >
    decltype(auto)
    operator()(FileStream&& filestream_) {
      CRANBERRIES_ASSERT(std::is_same<T,std::string>::value);
      auto ifs = std::ifstream{ filestream_.path(), filestream_.openmode() };
      std::string buf;
      CRANBERRIES_STREAM_FOPEN_ERROR_THROW_IF(!ifs.is_open());
      while (std::getline(ifs, buf)) {
        filestream_.emplace_back(buf);
      }
      return std::forward<FileStream>(filestream_);
    }
  };

  template < >
  class ReadByte<cranberries_magic::defaulted_t, cranberries_magic::defaulted_t>
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    ReadByte() = default;
    ReadByte(ReadByte const&) = default;
    ReadByte(ReadByte&&) = default;
    ~ReadByte() = default;

    template <
      typename FileStream,
      typename T = typename std::decay_t<FileStream>::element_type
    >
    decltype(auto)
    operator()(FileStream&& filestream_) {
      auto ifs = std::ifstream{ filestream_.path(), filestream_.openmode() };
      CRANBERRIES_STREAM_FOPEN_ERROR_THROW_IF(!ifs.is_open());
      T buf;
      while (ifs.read(reinterpret_cast<char*>(&buf), sizeof(T))) {
        filestream_.emplace_back(buf);
      }
      return std::forward<FileStream>(filestream_);
    }
  };

  template <
    typename Ate
  >
  class ReadByte<Ate,cranberries_magic::defaulted_t>
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    ReadByte(Ate ate ) : ate_{ ate } {}
    ReadByte() = default;
    ReadByte(ReadByte const&) = default;
    ReadByte(ReadByte&&) = default;
    ~ReadByte() = default;

    template <
      typename FileStream,
      typename T = typename std::decay_t<FileStream>::element_type
    >
    decltype(auto)
    operator()(FileStream&& filestream_) {
      auto ifs = std::ifstream{ filestream_.path(), filestream_.openmode() };
      CRANBERRIES_STREAM_FOPEN_ERROR_THROW_IF(!ifs.is_open());
      T buf;
      while(ifs.read(reinterpret_cast<char*>(&buf), ate_)) {
        filestream_.emplace_back(buf);
      }
      filestream_.pop_back();
      return std::forward<FileStream>(filestream_);
    }

  private:
    Ate ate_;
  };

  template <
    typename MaxSize
  >
  class ReadByte<cranberries_magic::defaulted_t, MaxSize>
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    ReadByte(MaxSize max_size) : max_size{ max_size } {}
    ReadByte() = default;
    ReadByte(ReadByte const&) = default;
    ReadByte(ReadByte&&) = default;
    ~ReadByte() = default;

    template <
      typename FileStream,
      typename T = typename std::decay_t<FileStream>::element_type
    >
    decltype(auto)
    operator()(FileStream&& filestream_) {
      auto ifs = std::ifstream{ filestream_.path(), filestream_.openmode() };
      T buf;
      CRANBERRIES_STREAM_FOPEN_ERROR_THROW_IF(!ifs.is_open());
      for (size_t i{};i<max_size;i+=max_size) {
        ifs.read(reinterpret_cast<char*>(&buf), sizeof(T));
        filestream_.emplace_back(buf);
      }
      return std::forward<FileStream>(filestream_);
    }

  private:
    MaxSize max_size;
  };

  template <
    typename Ate,
    typename MaxSize
  >
  class ReadByte
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    ReadByte(Ate ate, MaxSize max_size) : ate_{ ate }, max_size{max_size} {}
    ReadByte() = default;
    ReadByte(ReadByte const&) = default;
    ReadByte(ReadByte&&) = default;
    ~ReadByte() = default;

    template <
      typename FileStream,
      typename T = typename std::decay_t<FileStream>::element_type
    >
    decltype(auto)
    operator()(FileStream&& filestream_) {
      auto ifs = std::ifstream{ filestream_.path(), filestream_.openmode() };
      T buf;
      CRANBERRIES_STREAM_FOPEN_ERROR_THROW_IF(!ifs.is_open());
      for (size_t i{}; i<max_size; i += ate_) {
        ifs.read(reinterpret_cast<char*>(&buf), ate_);
        filestream_.emplace_back(buf);
      }
      return std::forward<FileStream>(filestream_);
    }

  private:
    Ate ate_;
    MaxSize max_size;
  };

  class Write
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    Write() = default;
    Write(Write const&) = default;
    Write(Write&&) = default;
    ~Write() = default;

    template <
      typename FileStream
    >
    decltype(auto)
    operator()(FileStream&& filestream_) {
      auto ofs = std::ofstream{ filestream_.path(), filestream_.openmode() };
      for (auto const& e : filestream_) {
        ofs << e << std::endl;
      }
      return std::forward<FileStream>(filestream_);
    }
  };

  class WriteLine
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    WriteLine() = default;
    WriteLine(WriteLine const&) = default;
    WriteLine(WriteLine&&) = default;
    ~WriteLine() = default;
    WriteLine(std::string delim) : delim_{ delim } {}

    template <
      typename FileStream
    >
    decltype(auto)
    operator()(FileStream&& filestream_) {
      auto ofs = std::ofstream{ filestream_.path(), filestream_.openmode() };
      CRANBERRIES_STREAM_FOPEN_ERROR_THROW_IF(!ofs.is_open());
      auto&& iter = filestream_.begin();
      ofs << *iter;
      ++iter;
      for (;iter!=filestream_.end();++iter) {
        ofs << delim_ << *iter;
      }
      ofs << std::endl;
      return std::forward<FileStream>(filestream_);
    }

  private:
    std::string delim_{};
  };

}
}
}

#endif