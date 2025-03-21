#include <PySkeleton.h>
#include <Log.hpp>

#include <PyAnimationStateData.h>
#include <PySkeletonData.h>
#include <PySkin.h>

static PyObject *PySkeleton_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject *self = (PyObject *)PyObject_Malloc(sizeof(PySkeleton));
    PyObject_Init(self, type);
    return self;
}

static int PySkeleton_init(PySkeleton *self, PyObject *args, PyObject *kwargs)
{
    PySkeletonData *skeletonData;
    PyAnimationStateData *animationStateData;
    if (!PyArg_ParseTuple(args, "OO", &skeletonData, &animationStateData))
    {
        PyErr_SetString(PyExc_TypeError, "argument must be a SkeletonData and a AnimationStateData");
    }
    self->it = spSkeletonDrawable_create(skeletonData->it, animationStateData->it);
    return 0;
}

static void PySkeleton_dealloc(PySkeleton *self)
{
    spSkeletonDrawable_dispose(self->it);
    PyObject_Free(self);
}

// getter and setter
// skeleton.usePremultipliedAlpha = True
// skeleton.x = 400
// skeleton.y = 500
// skeleton.canvasWidth = 800
// skeleton.canvasHeight = 500

static PyObject *PySkeleton_getUsePremultipliedAlpha(PySkeleton *self, void *closure)
{
    if (self->it->usePremultipliedAlpha)
    {
        Py_RETURN_TRUE;
    }
    else
    {
        Py_RETURN_FALSE;
    }
}

static int PySkeleton_setUsePremultipliedAlpha(PySkeleton *self, PyObject *value, void *closure)
{
    if (value == Py_True)
    {
        self->it->usePremultipliedAlpha = -1;
        return 0;
    }
    else if (value == Py_False)
    {
        self->it->usePremultipliedAlpha = 0;
        return 0;
    }
    else
    {
        PyErr_SetString(PyExc_TypeError, "argument must be a bool");
        return -1;
    }
}

static PyObject *PySkeleton_getX(PySkeleton *self, void *closure)
{
    return PyFloat_FromDouble(self->it->skeleton->x);
}

static int PySkeleton_setX(PySkeleton *self, PyObject *value, void *closure)
{
    self->it->skeleton->x = PyFloat_AsDouble(value);
    return 0;
}

static PyObject *PySkeleton_getY(PySkeleton *self, void *closure)
{
    return PyFloat_FromDouble(self->it->skeleton->y);
}

static int PySkeleton_setY(PySkeleton *self, PyObject *value, void *closure)
{
    self->it->skeleton->y = PyFloat_AsDouble(value);
    return 0;
}

static PyObject *PySkeleton_getCanvasWidth(PySkeleton *self, void *closure)
{
    return PyLong_FromLong(self->it->canvasWidth);
}

static int PySkeleton_setCanvasWidth(PySkeleton *self, PyObject *value, void *closure)
{
    self->it->canvasWidth = PyLong_AsLong(value);
    return 0;
}

static PyObject *PySkeleton_getCanvasHeight(PySkeleton *self, void *closure)
{
    return PyLong_FromLong(self->it->canvasHeight);
}

static int PySkeleton_setCanvasHeight(PySkeleton *self, PyObject *value, void *closure)
{
    self->it->canvasHeight = PyLong_AsLong(value);
    return 0;
}

static PyGetSetDef getset[] = {
    {"usePremultipliedAlpha", (getter)PySkeleton_getUsePremultipliedAlpha, (setter)PySkeleton_setUsePremultipliedAlpha, "use premultiplied alpha"},
    {"x", (getter)PySkeleton_getX, (setter)PySkeleton_setX, "x"},
    {"y", (getter)PySkeleton_getY, (setter)PySkeleton_setY, "y"},
    {"canvasWidth", (getter)PySkeleton_getCanvasWidth, (setter)PySkeleton_setCanvasWidth, "canvas width"},
    {"canvasHeight", (getter)PySkeleton_getCanvasHeight, (setter)PySkeleton_setCanvasHeight, "canvas height"},
    {NULL}};

// methods
static PyObject *PySkeleton_setSkin(PySkeleton *self, PyObject *args)
{
    PySkin *skin;
    if (!PyArg_ParseTuple(args, "O", &skin))
    {
        return NULL;
    }
    spSkeleton_setSkin(self->it->skeleton, skin->it);
    Py_RETURN_NONE;
}

static PyObject *PySkeleton_update(PySkeleton *self, PyObject *args)
{
    float delta;
    if (!PyArg_ParseTuple(args, "f", &delta))
    {
        return NULL;
    }
    spSkeletonDrawable_update(self->it, delta, SP_PHYSICS_UPDATE);
    Py_RETURN_NONE;
}

static PyObject *PySkeleton_setToSetupPose(PySkeleton *self, PyObject *args)
{
    spSkeleton_setToSetupPose(self->it->skeleton);
    Py_RETURN_NONE;
}

// setAnimationByName
static PyObject *PySkeleton_setAnimationByName(PySkeleton *self, PyObject *args)
{
    int trackIndex;
    char *name;
    bool loop;
    if (!PyArg_ParseTuple(args, "isb", &trackIndex, &name, &loop))
    {
        return NULL;
    }

    spAnimationState_setAnimationByName(self->it->animationState, trackIndex, name, loop ? -1 : 0);
    Py_RETURN_NONE;
}

// addAnimationByName

static PyObject *PySkeleton_addAnimationByName(PySkeleton *self, PyObject *args)
{
    int trackIndex;
    char *name;
    bool loop;
    float delay;
    if (!PyArg_ParseTuple(args, "isbf", &trackIndex, &name, &loop, &delay))
    {
        return NULL;
    }
    spAnimationState_addAnimationByName(self->it->animationState, trackIndex, name, loop ? -1 : 0, delay);
    Py_RETURN_NONE;
}

static PyObject *PySkeleton_draw(PySkeleton *self, PyObject *args, PyObject *kwds)
{
    spSkeletonDrawable_draw(self->it);
    Py_RETURN_NONE;
}

static PyMethodDef methods[] = {
    {"setSkin", (PyCFunction)PySkeleton_setSkin, METH_VARARGS, "set skin"},
    {"setToSetupPose", (PyCFunction)PySkeleton_setToSetupPose, METH_VARARGS, "set to setup pose"},
    {"update", (PyCFunction)PySkeleton_update, METH_VARARGS, "update"},
    {"setAnimationByName", (PyCFunction)PySkeleton_setAnimationByName, METH_VARARGS, "set animation by name"},
    {"addAnimationByName", (PyCFunction)PySkeleton_addAnimationByName, METH_VARARGS, "add animation by name"},
    {"draw", (PyCFunction)PySkeleton_draw, METH_VARARGS | METH_KEYWORDS, "draw skeleton"},
    {NULL}};

static PyType_Slot PySkeleton_slots[] = {
    {Py_tp_new, (void *)PySkeleton_new},
    {Py_tp_init, (void *)PySkeleton_init},
    {Py_tp_dealloc, (void *)PySkeleton_dealloc},
    {Py_tp_methods, (void *)methods},
    {Py_tp_getset, (void *)getset},
    {0, NULL}};

PyType_Spec PySkeleton_spec = {
    "spine.Skeleton",
    sizeof(PySkeleton),
    0,
    Py_TPFLAGS_DEFAULT,
    PySkeleton_slots,
};
