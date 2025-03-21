#pragma once
#include <spine-opengl.h>

#define Py_LIMITED_API
#include <Python.h>

struct PySkeletonJson
{
    PyObject_HEAD

    spSkeletonJson* it;
};

extern PyType_Spec PySkeletonJson_spec;
