#ifndef CRANBERRIES_UTILITY_REVERSE_INDEX_SEQUENCE_HPP
#define CRANBERRIES_UTILITY_REVERSE_INDEX_SEQUENCE_HPP
#include "../pack_operations.hpp"

namespace cranberries {
template < size_t N >
using make_reversed_index_sequence = typename pack_reverse<std::make_index_sequence<N>>::type;
}
#endif