#pragma once
#include <spine-opengl.h>

#define Py_LIMITED_API
#include <Python.h>

struct PySkin
{
    PyObject_HEAD

    spSkin* it;
};

extern PyType_Spec PySkin_spec;
