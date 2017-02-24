#ifndef CRANBERRIES_STREAMS_OPERATORS_CHUNK_HPP
#define CRANBERRIES_STREAMS_OPERATORS_CHUNK_HPP
#include <utility>
#include <type_traits>
#include "../utility.hpp"
#include "../forward.hpp"
#include "../cranberries_magic/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  template < size_t N > struct ChunkProxy{};

  template <
    std::size_t N,
    typename Old
  >
  class Chunk
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    Chunk() = default;
    Chunk(Chunk const&) = default;
    Chunk(Chunk&&) = default;
    Chunk(Old stream) : old_{std::forward<Old>(stream)} {}
    ~Chunk() = default;

    template <
      typename Stream,
      typename T = element_type_of_t<Stream>
    >
    auto operator()(Stream&& stream_) {
      old_.eval();
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF(old_.empty());
      auto&& v = old_.get();
      auto iter = v.begin();
      for (std::size_t i{}, size = v.size(); i < size / N; ++i) {
        T cont{ {} };
        for (std::size_t j{}; j < N; ++j,++iter) cont[j] = std::move(*iter);
        stream_.emplace_back(std::move(cont));
      }

      return std::forward<Stream>(stream_);
    }

  private:
    Old old_;
  };

} // ! namespace operators
} // ! namespace streams
} // ! namespace cranberries

#endif