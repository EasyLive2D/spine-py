#pragma once
#include <spine-opengl.h>

#define Py_LIMITED_API
#include <Python.h>

struct PySkeleton
{
    PyObject_HEAD

    spSkeletonDrawable* it;
};

extern PyType_Spec PySkeleton_spec;
