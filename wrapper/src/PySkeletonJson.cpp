#include <PySkeletonJson.h>
#include <Log.hpp>

#include <PyAtlas.h>
#include <PySkeletonData.h>
#include <PySpine.h>

static PyObject *PySkeletonJson_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject* self = (PyObject*)PyObject_Malloc(sizeof(PySkeletonJson));
    PyObject_Init(self, type);
    return self;
}

static int PySkeletonJson_init(PySkeletonJson *self, PyObject *args, PyObject *kwargs)
{
    PyAtlas *atlas;
    if (!PyArg_ParseTuple(args, "O", &atlas))
    {
        return -1;
    }

    self->it = spSkeletonJson_create(atlas->it);
    return 0;
}

static void PySkeletonJson_dealloc(PySkeletonJson *self)
{
    spSkeletonJson_dispose(self->it);
    PyObject_Free(self);
}

static PyGetSetDef getset[] = {
    {NULL}
};

static PyObject* PySkeletonJson_readSkeletonDataFile(PySkeletonJson *self, PyObject *args)
{
    char *path;
    if (!PyArg_ParseTuple(args, "s", &path))
    {
        return NULL;
    }

    PyObject* skdClass = PyObject_GetAttrString(PySpineModule, "SkeletonData");
    PySkeletonData* skeletonData = (PySkeletonData*)PyObject_CallObject(skdClass, NULL);

    skeletonData->it = spSkeletonJson_readSkeletonDataFile(self->it, path);
    return (PyObject*)skeletonData;
}

static PyMethodDef methods[] = {
    {"readSkeletonDataFile", (PyCFunction)PySkeletonJson_readSkeletonDataFile, METH_VARARGS, ""},
    {NULL}
};

static PyType_Slot PySkeletonJson_slots[] = {
    {Py_tp_new, (void*)PySkeletonJson_new},
    {Py_tp_init, (void*)PySkeletonJson_init},
    {Py_tp_dealloc, (void*)PySkeletonJson_dealloc},
    {Py_tp_methods, (void*)methods},
    {Py_tp_getset, (void*)getset},
    {0, NULL}
};

PyType_Spec PySkeletonJson_spec = {
    "spine.SkeletonJson",
    sizeof(PySkeletonJson),
    0,
    Py_TPFLAGS_DEFAULT,
    PySkeletonJson_slots,
};
