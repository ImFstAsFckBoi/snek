#pragma once

#include "utility.hh"

#include <Python.h>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include <set>
#include <sstream>
namespace snek {


std::set<std::string> _fmt_store;
std::vector<PyMethodDef> _methods;


template<typename T>
const char *GetFmtStrV(T) {
    static_assert(false, "Undefined type used!");
    return ""; // shush
};

constexpr const char *GetFmtStrV() { return "()"; };

constexpr const char *GetFmtStrV(const char *) { return "s"; };

constexpr const char *GetFmtStrV(std::string const &) { return "s"; };

constexpr const char *GetFmtStrV(int) { return "i"; };

constexpr const char *GetFmtStrV(long) { return "l"; };

constexpr const char *GetFmtStrV(float) { return "f"; };

constexpr const char *GetFmtStrV(double) { return "d"; };

constexpr const char *GetFmtStrV(PyObject *) { return "O"; };

template<typename... Ts>
const char * _create_tuple_fmt(Ts... args) {
    std::stringstream key;
    key << "(" << join(", ", GetFmtStrV(args)...) << ")";

    auto [new_it, _] = _fmt_store.insert(key.str());
    return new_it->c_str();
}



template<typename... Ts>
const char *GetFmtStrV(Ts... args) {
    return _create_tuple_fmt(args...);
};

template<typename T>
constexpr const char *GetFmtStrT() {
    static_assert(false, "Undefined type used!");
    return ""; // shush
};

template<>
constexpr const char *GetFmtStrT<void>() { return "()"; };

template<>
constexpr const char *GetFmtStrT<const char *>() { return "s"; };

template<>
constexpr const char *GetFmtStrT<std::string const &>() { return "s"; };

template<>
constexpr const char *GetFmtStrT<int>() { return "i"; };

template<>
constexpr const char *GetFmtStrT<long>() { return "l"; };

template<>
constexpr const char *GetFmtStrT<float>() { return "f"; };

template<>
constexpr const char *GetFmtStrT<double>() { return "d"; };

template<>
constexpr const char *GetFmtStrT<PyObject *>() { return "O"; };


template<typename... Ts, typename = std::enable_if_t<(sizeof...(Ts) > 1)>>
const char *GetFmtStrT() {
    return _create_tuple_fmt<Ts...>();
};



template<typename... Ts>
const char * _create_tuple_fmt() {
    std::stringstream key;
    key << "(" << join(", ", GetFmtStrT<Ts>()...) << ")";

    auto [new_it, _] = _fmt_store.insert(key.str());
    return new_it->c_str();
}


template<typename... Ts>
const char * _create_list_fmt(Ts... args) {
    std::stringstream key;
    key << "[" << join(", ", GetFmtStrV(args)...) << "]";

    auto [new_it, _] = _fmt_store.insert(key.str());
    return new_it->c_str();
}



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



// template<typename T, std::enable_if_t<is_iterable<T>::value, int> = 0>
// PyObject *ToPyValue(T arg) {
// }

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



template<typename Tuple>
void _ParseToTuple(PyObject *, const char *, Tuple&, std::index_sequence<>) { /* no args, do nothing */}

template<typename Tuple, std::size_t... Idxs>
void _ParseToTuple(PyObject *src_args, const char *fmt_str, Tuple& dest_args, std::index_sequence<Idxs...>) {
    // TODO: Type+Validity checking
    PyArg_ParseTuple(src_args, fmt_str, &std::get<Idxs>(dest_args)...);
}

template<typename Fn, typename Tuple, std::size_t... Idxs, std::enable_if_t<!std::is_void_v<void_return<Fn>>, int> = 0>
PyObject *_InvokeWithTupleArgs(Fn&& fn, Tuple const &args, std::index_sequence<Idxs...>) {
    auto&& ret = fn(std::get<Idxs>(args)...); 
    return snek::ToPyValue(std::forward<typename get_fnptr_t<Fn>::return_type>(ret));
}

template<typename Fn, typename Tuple, std::size_t... Idxs, std::enable_if_t<std::is_void_v<void_return<Fn>>, int> = 0>
PyObject *_InvokeWithTupleArgs(Fn&& fn, Tuple const &args, std::index_sequence<Idxs...>) {
    fn(std::get<Idxs>(args)...); 
    return Py_None;
}


template<typename T, typename... Args>
PyObject *_FuncWrapperInvoke(fnptr_t<T, Args...> fn, PyObject *self, PyObject *args) {
    (void)self;
    std::tuple<Args...> t_args;
    _ParseToTuple(args, _create_parse_fmt<Args...>(), t_args, std::index_sequence_for<Args...>());
    return _InvokeWithTupleArgs(fn, t_args, std::index_sequence_for<Args...>());
}


template<auto Fn>
PyObject *FuncWrapper(PyObject *self, PyObject *args) {
    return _FuncWrapperInvoke(Fn, self, args);
}

template<auto Fn, std::enable_if_t<std::is_same_v<decltype(Fn), PyCFunction>, int> = 0>
void ExportFn(const char *name, const char *doc = nullptr) {
    constexpr int call_method = get_fnptr<decltype(Fn)>::type::n_args == 0? METH_NOARGS : METH_VARARGS;

    _methods.push_back(
        PyMethodDef{name, Fn, call_method, doc}
    );
}

template<auto Fn, std::enable_if_t<!std::is_same_v<decltype(Fn), PyCFunction>, int> = 0>
void ExportFn(const char *name, const char *doc = nullptr) {
    constexpr int call_method = get_fnptr<decltype(Fn)>::type::n_args == 0? METH_NOARGS : METH_VARARGS;

    _methods.push_back(
        PyMethodDef{name, FuncWrapper<Fn>, call_method, doc}
    );
}

#define SNEK_ExportMod(name, doc) \
struct PyModuleDef _mod_##name;\
void _mod_inject();\
PyMODINIT_FUNC PyInit_##name(void) { \
    _mod_inject();\
    ::snek::_methods.push_back({nullptr, nullptr, 0, nullptr});\
    _mod_##name = {PyModuleDef_HEAD_INIT, #name, doc, -1, snek::_methods.data(), 0, 0, 0, 0};\
    return PyModule_Create(&_mod_##name);\
}\
void _mod_inject()\

}