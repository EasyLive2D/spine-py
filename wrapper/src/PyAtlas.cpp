#include <PyAtlas.h>
#include <Log.hpp>

static PyObject *PyAtlas_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject *self = (PyObject *)PyObject_Malloc(sizeof(PyAtlas));
    PyObject_Init(self, type);
    return self;
}

static int PyAtlas_init(PyAtlas *self, PyObject *args, PyObject *kwargs)
{
    const char *path;
    if (!PyArg_ParseTuple(args, "s", &path))
        return -1;

    self->it = spAtlas_createFromFile(path, 0);
    return 0;
}

static void PyAtlas_dealloc(PyAtlas *self)
{
    spAtlas_dispose(self->it);
    PyObject_Free(self);
}

static PyGetSetDef getset[] = {
    {NULL}};

static PyMethodDef methods[] = {
    {NULL}};

static PyType_Slot PyAtlas_slots[] = {
    {Py_tp_new, (void *)PyAtlas_new},
    {Py_tp_init, (void *)PyAtlas_init},
    {Py_tp_dealloc, (void *)PyAtlas_dealloc},
    {Py_tp_methods, (void *)methods},
    {Py_tp_getset, (void *)getset},
    {0, NULL}};

PyType_Spec PyAtlas_spec = {
    "spine.Atlas",
    sizeof(PyAtlas),
    0,
    Py_TPFLAGS_DEFAULT,
    PyAtlas_slots,
};
