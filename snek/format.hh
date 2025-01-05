#pragma once

#include <Python.h>
#include <string>

namespace snek {

/*
    Get format string from value
*/

template<typename T>
const char *GetFmtStrV(T) {
    static_assert(false, "Undefined type used!");
    return ""; // shush
};

constexpr const char *GetFmtStrV() { return "()"; };

constexpr const char *GetFmtStrV(const char *) { return "s"; };

constexpr const char *GetFmtStrV(int) { return "i"; };

constexpr const char *GetFmtStrV(long) { return "l"; };

constexpr const char *GetFmtStrV(float) { return "f"; };

constexpr const char *GetFmtStrV(double) { return "d"; };

constexpr const char *GetFmtStrV(PyObject *) { return "O"; };

/*
    Get format string from type
*/

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
constexpr const char *GetFmtStrT<int>() { return "i"; };

template<>
constexpr const char *GetFmtStrT<long>() { return "l"; };

template<>
constexpr const char *GetFmtStrT<float>() { return "f"; };

template<>
constexpr const char *GetFmtStrT<double>() { return "d"; };

template<>
constexpr const char *GetFmtStrT<PyObject *>() { return "O"; };

}