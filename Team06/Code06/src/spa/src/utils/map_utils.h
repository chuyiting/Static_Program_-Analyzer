//
// Created by Stephen Tan  Hin Khai on 1/9/22.
//
#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

// A namespace for map_utility functions
namespace map_utils {

// Template for iterating through some collection on some map and returning successful values stored as a set
// return collection may change dependent on usage.
    template<typename SomeCollection, typename SomeMap>
    std::unordered_set<typename SomeMap::mapped_type> valuesIn(const SomeCollection &collection, const SomeMap &map) {
        std::unordered_set<typename SomeMap::mapped_type> r;
        r.reserve(map.size());
        for (const auto &val: collection) {
            auto it = map.find(val);
            if (it != map.end()) {
                r.insert(it->second);
            }
        }
        return r;
    }

    // Template for iterating through a multimap to find_value
    template<typename SomeMultimap, typename SomeValue>
    bool mm_hasValue(const SomeMultimap &mm, const SomeValue lhs, const SomeValue rhs) {
        auto it = mm.equal_range(lhs);
        for (auto itr = it.first; itr != it.second; ++itr) {
            if (itr->second == rhs) {
                return true;
            }
        }
        return false;
    }


// Template for returning keys of a map/unordered_map from STL
    template<typename SomeMap>
    std::unordered_set<typename SomeMap::key_type> keys(const SomeMap &map) {
        std::unordered_set<typename SomeMap::key_type> r;
        r.reserve(map.size());
        for (const auto &tuple: map) {
            r.insert(tuple.first);
        }
        return r;
    }

// Template for returning values of a map/unordered_map from STL
    template<typename SomeMap>
    std::unordered_set<typename SomeMap::mapped_type> values(const SomeMap &map) {
        std::unordered_set<typename SomeMap::mapped_type> r;
        r.reserve(map.size());
        for (const auto &t: map) {
            r.insert(t.second);
        }
        return r;
    }


    template<typename SomeIterable>
    bool isIterableContains(const SomeIterable &iterable, const typename SomeIterable::key_type &key) {
        return iterable.find(key) != iterable.end();
    }
}