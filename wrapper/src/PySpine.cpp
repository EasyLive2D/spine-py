#include <spine-opengl.h>
#include <glad/glad.h>
#include "Log.hpp"
#include <PySpine.h>

#define Py_LIMITED_API
#include <Python.h>

static PyObject *spine_glInit(PyObject *self, PyObject *args)
{
    if (!gladLoadGL())
    {
        PyErr_SetString(PyExc_RuntimeError, "failed to initialize gl.");
    }
    Py_RETURN_NONE;
}

static PyObject *spine_clearBuffer(PyObject *self, PyObject *args, PyObject *kwargs)
{
    float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

    static char *clear_buffer_keywords[] = {"r", "g", "b", "a", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|ffff", clear_buffer_keywords, &r, &g, &b, &a))
    {
        PyErr_SetString(PyExc_RuntimeError, "invalid args");
    }

    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
    Py_RETURN_NONE;
}

static PyObject *spine_viewport(PyObject *self, PyObject *args)
{
    int x = 0, y = 0, w = 0, h = 0;
    if (!PyArg_ParseTuple(args, "iiii", &x, &y, &w, &h))
    {
        PyErr_SetString(PyExc_RuntimeError, "invalid args");
    }
    glViewport(x, y, w, h);
    Py_RETURN_NONE;
}

static PyMethodDef spine_methods[] = {
    {"glInit", (PyCFunction)spine_glInit, METH_KEYWORDS | METH_VARARGS, "init inner gl functions"},
    {"clearBuffer", (PyCFunction)spine_clearBuffer, METH_KEYWORDS | METH_VARARGS, "clear canvas"},
    {"viewport", (PyCFunction)spine_viewport, METH_KEYWORDS | METH_VARARGS, "set viewport"},
    {NULL, NULL, 0, NULL}};

struct PyModuleDef PySpine = {
    PyModuleDef_HEAD_INIT,
    "spine",
    NULL,
    -1,
    spine_methods};

PyObject *PySpineModule = nullptr;