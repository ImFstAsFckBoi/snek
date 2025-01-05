#pragma once

#include "utility.hh"
#include "format.hh"
#include "runtime.hh"

#include <Python.h>
#include <tuple>
#include <type_traits>
#include <utility>


namespace snek {

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

/*
    Function wrapper
*/

template<typename Tuple>
void _ParseToTuple(PyObject *, const char *, Tuple&, std::index_sequence<>) { /* no args, do nothing */}

template<typename Tuple, std::size_t... Idxs>
void _ParseToTuple(PyObject *src_args, const char *fmt_str, Tuple& dest_args, std::index_sequence<Idxs...>) {
    // TODO: Type+Validity checking
    PyArg_ParseTuple(src_args, fmt_str, &std::get<Idxs>(dest_args)...);
}

template<typename Fn, typename Tuple, std::size_t... Idxs, std::enable_if_t<!has_void_return_v<Fn>, int> = 0>
PyObject *_InvokeWithTupleArgs(Fn&& fn, Tuple const &args, std::index_sequence<Idxs...>) {
    auto&& ret = fn(std::get<Idxs>(args)...); 
    return snek::ToPyValue(std::forward<typename get_fnptr_t<Fn>::return_type>(ret));
}

template<typename Fn, typename Tuple, std::size_t... Idxs, std::enable_if_t<has_void_return_v<Fn>, int> = 0>
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

/*
    Export function
*/

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

/*
    Export module
 */

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