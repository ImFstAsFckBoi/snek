#pragma once

#include "utility.hh"
#include "format.hh"

#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <modsupport.h>


namespace snek {

std::set<std::string> _fmt_store;
std::vector<PyMethodDef> _methods;


/*
    Create List/Tuple Format
 */

template<typename... Ts>
const char * _create_tuple_fmt(Ts... args) {
    std::stringstream key;
    key << "(" << join(", ", GetFmtStrV(args)...) << ")";

    auto [new_it, _] = _fmt_store.insert(key.str());
    return new_it->c_str();
}

template<typename... Ts>
const char * _create_tuple_fmt() {
    std::stringstream key;
    key << "(" << join(", ", GetFmtStrT<Ts>()...) << ")";

    auto [new_it, _] = _fmt_store.insert(key.str());
    return new_it->c_str();
}

template<typename... Ts>
const char *GetFmtStrV(Ts... args) {
    return _create_tuple_fmt(args...);
};

template<typename... Ts, typename = std::enable_if_t<(sizeof...(Ts) > 1)>>
const char *GetFmtStrT() {
    return _create_tuple_fmt<Ts...>();
};

template<typename... Ts>
const char * _create_list_fmt(Ts... args) {
    std::stringstream key;
    key << "[" << join(", ", GetFmtStrV(args)...) << "]";

    auto [new_it, _] = _fmt_store.insert(key.str());
    return new_it->c_str();
}

/*
    Create Parse Format
 */

template<typename T>
void _create_parse_fmt_inner(std::stringstream &ss) {
    ss << GetFmtStrT<T>();
}

template<typename T, typename... Ts, typename = std::enable_if_t<(sizeof...(Ts) > 0)>>
void _create_parse_fmt_inner(std::stringstream &ss) {
    ss << GetFmtStrT<T>();
    _create_parse_fmt_inner<Ts...>(ss);
}

template<typename... Ts, std::enable_if_t<sizeof...(Ts) == 0, int> = 0>
const char *_create_parse_fmt() {
    return "";
}

template<typename... Ts, std::enable_if_t<sizeof...(Ts) != 0, int> = 0>
const char *_create_parse_fmt() {
    std::stringstream fmt;
    _create_parse_fmt_inner<Ts...>(fmt);
    auto [new_it, _] = _fmt_store.insert(fmt.str());
    return new_it->c_str();
}


/*
    To Python value
*/

template<typename T, std::enable_if_t<!is_iterable<T>::value, int> = 0>
PyObject *ToPyValue(T arg) {
    return Py_BuildValue(GetFmtStrV(arg), arg);
}

template<typename... Ts>
PyObject *ToPyValue(Ts... args) {
    return Py_BuildValue(GetFmtStrV(args...), args...);
}

PyObject *ToPyValue() {
    return Py_BuildValue(GetFmtStrV());
}

template<typename... Ts>
PyObject *ToPyTuple(Ts... args) {
    return Py_BuildValue(_create_tuple_fmt(args...), args...);
}

template<typename... Ts>
PyObject *ToPyList(Ts... args) {
    return Py_BuildValue(_create_list_fmt(args...), args...);
}

}