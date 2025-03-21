#pragma once
#include <spine-opengl.h>

#define Py_LIMITED_API
#include <Python.h>

struct PyAnimationStateData
{
    PyObject_HEAD

    spAnimationStateData* it;
};

extern PyType_Spec PyAnimationStateData_spec;
