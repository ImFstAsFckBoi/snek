#include <Python.h>
#include "snek.hh"

extern "C" {

PyObject* nothing(PyObject* self, PyObject* args) {
     (void)self, (void) args;
    return Py_None;
}


PyObject* hello(PyObject* self, PyObject* args) {
    (void)self, (void) args;
    return snek::ToPyValue("HELLO!");
}

PyObject* tup(PyObject* self, PyObject* args) {
    (void)self, (void) args;
    return snek::ToPyValue(1, 2, 3);
}

PyObject* lis(PyObject* self, PyObject* args) {
    (void)self, (void) args;
    return snek::ToPyList(1, 2, 3);
}

// Define the methods in the module
PyMethodDef Methods[] = {
    {"hello", hello, METH_NOARGS, nullptr},
    {"nothing", nothing, METH_NOARGS, nullptr},
    {"tup", tup, METH_NOARGS, nullptr},
    {"lis", lis, METH_NOARGS, nullptr},
    {nullptr, nullptr, 0, nullptr}  // Sentinel
};



static struct PyModuleDef testmod = {
    PyModuleDef_HEAD_INIT,
    "testmod",
    "Test module",
    -1,
    Methods,
    0,
    0,
    0,
    0,
};

PyMODINIT_FUNC PyInit_testmod(void) {
    return PyModule_Create(&testmod);
}
}