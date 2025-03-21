#pragma once
#include <spine-opengl.h>

#define Py_LIMITED_API
#include <Python.h>

struct PySkeletonData
{
    PyObject_HEAD

    spSkeletonData* it;
};

extern PyType_Spec PySkeletonData_spec;
