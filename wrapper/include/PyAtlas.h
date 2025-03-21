#pragma once
#include <spine-opengl.h>

#define Py_LIMITED_API
#include <Python.h>

struct PyAtlas
{
    PyObject_HEAD

    spAtlas* it;
};

extern PyType_Spec PyAtlas_spec;
