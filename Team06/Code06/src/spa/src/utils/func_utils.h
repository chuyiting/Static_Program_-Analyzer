//
// Created by Stephen Tan  Hin Khai on 15/9/22.
//
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

#ifndef SPA_SRC_SPA_SRC_UTILS_FUNCTIONAL_ITERABLE_H_

namespace func_utils {
    template<class Container, class Function>
    auto apply(const Container &cont, Function fun) {
        std::vector<typename
        std::result_of<Function(const typename Container::value_type &)>::type> ret;
        ret.reserve(cont.size());
        for (const auto &v: cont) {
            ret.push_back(fun(v));
        }
        return ret;
    }
}
#define SPA_SRC_SPA_SRC_UTILS_FUNCTIONAL_ITERABLE_H_

#endif //SPA_SRC_SPA_SRC_UTILS_FUNCTIONAL_ITERABLE_H_
