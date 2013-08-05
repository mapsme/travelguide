#pragma once

#include <boost/array.hpp>

template <class T, size_t N>
inline size_t ArraySize(T (&) [N]) { return N; }
