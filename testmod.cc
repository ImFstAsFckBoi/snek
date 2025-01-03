#include <Python.h>
// #include "snek.hh"


static PyObject* nothing(PyObject* self, PyObject* args) {
    return Py_None;
}


static PyObject* hello(PyObject* self, PyObject* args) {
    return Py_BuildValue("s", "HELLO!");
}

static PyObject* tup(PyObject* self, PyObject* args) {
    return Py_BuildValue("(i, i, i)", 1, 2, 3);
}

static PyObject* lis(PyObject* self, PyObject* args) {
    return Py_BuildValue("[i, i, i]", 1, 2, 3);
}

// Define the methods in the module
static PyMethodDef Methods[] = {
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
};

PyMODINIT_FUNC PyInit_testmod(void) {
    return PyModule_Create(&testmod);
}