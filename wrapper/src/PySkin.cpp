#include <PySkin.h>
#include <Log.hpp>

static PyObject *PySkin_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject* self = (PyObject*)PyObject_Malloc(sizeof(PySkin));
    PyObject_Init(self, type);
    return self;
}

static int PySkin_init(PySkin *self, PyObject *args, PyObject *kwargs)
{
    return 0;
}

static void PySkin_dealloc(PySkin *self)
{
    spSkin_dispose(self->it);
    PyObject_Free(self);
}

static PyGetSetDef getset[] = {
    {NULL}
};

static PyMethodDef methods[] = {
    {NULL}
};

static PyType_Slot PySkin_slots[] = {
    {Py_tp_new, (void*)PySkin_new},
    {Py_tp_init, (void*)PySkin_init},
    {Py_tp_dealloc, (void*)PySkin_dealloc},
    {Py_tp_methods, (void*)methods},
    {Py_tp_getset, (void*)getset},
    {0, NULL}
};

PyType_Spec PySkin_spec = {
    "spine.Skin",
    sizeof(PySkin),
    0,
    Py_TPFLAGS_DEFAULT,
    PySkin_slots,
};
