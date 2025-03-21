#include <PyAnimationStateData.h>
#include <Log.hpp>

#include <PySkeletonData.h>

static PyObject *PyAnimationStateData_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject *self = (PyObject *)PyObject_Malloc(sizeof(PyAnimationStateData));
    PyObject_Init(self, type);
    return self;
}

static int PyAnimationStateData_init(PyAnimationStateData *self, PyObject *args, PyObject *kwargs)
{
    PySkeletonData *skeletonData;
    if (!PyArg_ParseTuple(args, "O", &skeletonData))
    {
        return -1;
    }

    self->it = spAnimationStateData_create(skeletonData->it);
    return 0;
}

static void PyAnimationStateData_dealloc(PyAnimationStateData *self)
{
    spAnimationStateData_dispose(self->it);
    PyObject_Free(self);
}

static PyGetSetDef getset[] = {
    {NULL}};

static PyMethodDef methods[] = {
    {NULL}};

static PyType_Slot PyAnimationStateData_slots[] = {
    {Py_tp_new, (void *)PyAnimationStateData_new},
    {Py_tp_init, (void *)PyAnimationStateData_init},
    {Py_tp_dealloc, (void *)PyAnimationStateData_dealloc},
    {Py_tp_methods, (void *)methods},
    {Py_tp_getset, (void *)getset},
    {0, NULL}};

PyType_Spec PyAnimationStateData_spec = {
    "spine.AnimationStateData",
    sizeof(PyAnimationStateData),
    0,
    Py_TPFLAGS_DEFAULT,
    PyAnimationStateData_slots,
};
