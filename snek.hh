#pragma once

#include "pyconfig.h"
#include <Python.h>
#include <iterator>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>
#include <vector>


namespace snek {


std::set<std::string> _fmt_store;

template<typename T>
void _join(std::stringstream &ss, std::string const &, T arg) {
    ss << arg;
}

template<typename  T, typename... Ts>
void _join(std::stringstream &ss, std::string const &delim, T arg, Ts... args) {
    ss << arg << delim;
    _join(ss, delim, args...);
}


template<typename... Ts>
std::string join(std::string const &delim, Ts... args) {
    std::stringstream ss;
    _join(ss, delim, args...);
    return ss.str();
}


template<typename T>
const char *GetFmtStr(T) {
    static_assert(false, "Undefined type used!");
    return ""; // shush
};

constexpr const char *GetFmtStr() { return "()"; };

constexpr const char *GetFmtStr(const char *) { return "s"; };

constexpr const char *GetFmtStr(std::string const &) { return "s"; };

constexpr const char *GetFmtStr(int) { return "i"; };

constexpr const char *GetFmtStr(long) { return "l"; };

constexpr const char *GetFmtStr(float) { return "f"; };

constexpr const char *GetFmtStr(double) { return "d"; };


template<typename... Ts>
const char * _create_tuple(Ts... args) {
    std::stringstream key;
    key << "(" << join(", ", GetFmtStr(args)...) << ")";

    auto [new_it, _] = _fmt_store.insert(key.str());
    return new_it->c_str();
}

template<typename... Ts>
const char * _create_list(Ts... args) {
    std::stringstream key;
    key << "[" << join(", ", GetFmtStr(args)...) << "]";

    auto [new_it, _] = _fmt_store.insert(key.str());
    return new_it->c_str();
}

template<typename T, typename = void>
struct is_iterable {
    static constexpr bool value = false;
};

template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>()), std::end(std::declval<T>()))>> {
    static constexpr bool value = true;
};


// template<typename T, std::enable_if_t<is_iterable<T>::value, int> = 0>
// PyObject *ToPyValue(T arg) {
// }

template<typename T, std::enable_if_t<!is_iterable<T>::value, int> = 0>
PyObject *ToPyValue(T arg) {
    return Py_BuildValue(GetFmtStr(arg), arg);
}


template<typename... Ts>
const char *GetFmtStr(Ts... args) {
    return _create_tuple(args...);
};


template<typename... Ts>
PyObject *ToPyValue(Ts... args) {
    return Py_BuildValue(GetFmtStr(args...), args...);
}

template<typename... Ts>
PyObject *ToPyTuple(Ts... args) {
    return Py_BuildValue(_create_tuple(args...), args...);
}
template<typename... Ts>
PyObject *ToPyList(Ts... args) {
    return Py_BuildValue(_create_list(args...), args...);
}

PyObject *ToPyValue() {
    return Py_BuildValue(GetFmtStr());
}

}