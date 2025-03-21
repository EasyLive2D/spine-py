#include <PySkeletonData.h>
#include <Log.hpp>

#include <PySkin.h>
#include <PySpine.h>

static PyObject *PySkeletonData_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject *self = (PyObject *)PyObject_Malloc(sizeof(PySkeletonData));
    PyObject_Init(self, type);
    return self;
}

static int PySkeletonData_init(PySkeletonData *self, PyObject *args, PyObject *kwargs)
{

    return 0;
}

static void PySkeletonData_dealloc(PySkeletonData *self)
{
    spSkeletonData_dispose(self->it);
    PyObject_Free(self);
}

static PyGetSetDef getset[] = {
    {NULL}};

static PyObject* PySkeleton_findSkin(PySkeletonData *self, PyObject* args)
{
    char* name;
    if (!PyArg_ParseTuple(args, "s", &name))
    {
        return NULL;
    }
    PyObject* pySkinClazz = PyObject_GetAttrString(PySpineModule, "Skin");
    PySkin *skin = (PySkin *)PyObject_CallFunctionObjArgs(pySkinClazz, NULL);
    skin->it = spSkeletonData_findSkin(self->it, name);

    return (PyObject*)skin;
}

static PyMethodDef methods[] = {
    {"findSkin", (PyCFunction)PySkeleton_findSkin, METH_VARARGS, "find skin"},
    {NULL}};

static PyType_Slot PySkeletonData_slots[] = {
    {Py_tp_new, (void *)PySkeletonData_new},
    {Py_tp_init, (void *)PySkeletonData_init},
    {Py_tp_dealloc, (void *)PySkeletonData_dealloc},
    {Py_tp_methods, (void *)methods},
    {Py_tp_getset, (void *)getset},
    {0, NULL}};

PyType_Spec PySkeletonData_spec = {
    "spine.SkeletonData",
    sizeof(PySkeletonData),
    0,
    Py_TPFLAGS_DEFAULT,
    PySkeletonData_slots,
};
