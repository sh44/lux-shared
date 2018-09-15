#pragma once

#include <type_traits>
#include <vector>
#include <string>
#include <array>
#include <queue>
#include <deque>
#include <unordered_map>
#include <unordered_set>
//
#include <lux_shared/slice.hpp>

template<typename T>
using DynArr = std::vector<T>;
template<typename T, std::size_t len>
using Arr = std::array<T, len>;
template<typename T>
using Queue = std::queue<T>;
template<typename T>
using Deque = std::deque<T>;
template<typename K, typename V, typename Hasher>
using HashMap = std::unordered_map<K, V, Hasher>;
template<typename V, typename Hasher>
using HashSet = std::unordered_set<V, Hasher>;
template<typename T>
using AlignArr = std::aligned_storage_t<sizeof(T), alignof(T)>;

typedef std::string String;