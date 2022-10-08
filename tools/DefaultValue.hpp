/**
 * @file DefaultValue.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief Return the default value of given type
 * @version 0.1
 * @date 2022-10-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <type_traits>

namespace Tool {

template <typename T>
concept integer
    = std::is_same_v<T, int>
    or std::is_same_v<T, short>
    or std::is_same_v<T, long>
    or std::is_same_v<T, long long>;

template <typename T>
concept unsigned_integer
    = std::is_same_v<T, unsigned int>
    or std::is_same_v<T, unsigned short>
    or std::is_same_v<T, unsigned long>
    or std::is_same_v<T, unsigned long long>;

template <typename T>
concept decimal
    = std::is_same_v<T, float>
    or std::is_same_v<T, double>;

template <typename T>
concept mathematical
    = integer<T>
    or unsigned_integer<T>
    or decimal<T>;

} // namespace Tool