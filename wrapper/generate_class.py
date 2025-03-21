header_template = """#pragma once
#include <spine-opengl.h>

#define Py_LIMITED_API
#include <Python.h>

struct Py@@NAME@@
{
    PyObject_HEAD

    sp@@NAME@@* it;
};

extern PyType_Spec Py@@NAME@@_spec;
"""

source_template = """#include <Py@@NAME@@.h>
#include <Log.hpp>

static PyObject *Py@@NAME@@_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject* self = (PyObject*)PyObject_Malloc(sizeof(Py@@NAME@@));
    PyObject_Init(self, type);
    return self;
}

static int Py@@NAME@@_init(Py@@NAME@@ *self, PyObject *args, PyObject *kwargs)
{
    return 0;
}

static void Py@@NAME@@_dealloc(Py@@NAME@@ *self)
{
    sp@@NAME@@_dispose(self->it);
    PyObject_Free(self);
}

static PyGetSetDef getset[] = {
    {NULL}
};

static PyMethodDef methods[] = {
    {NULL}
};

static PyType_Slot Py@@NAME@@_slots[] = {
    {Py_tp_new, (void*)Py@@NAME@@_new},
    {Py_tp_init, (void*)Py@@NAME@@_init},
    {Py_tp_dealloc, (void*)Py@@NAME@@_dealloc},
    {Py_tp_methods, (void*)methods},
    {Py_tp_getset, (void*)getset},
    {0, NULL}
};

PyType_Spec Py@@NAME@@_spec = {
    "spine.@@NAME@@",
    sizeof(Py@@NAME@@),
    0,
    Py_TPFLAGS_DEFAULT,
    Py@@NAME@@_slots,
};
"""

import os

cd = os.path.split(__file__)[0]

names = [
    "Atlas",
    "SkeletonJson", 
    "AnimationStateData", 
    "Skeleton",
    "Skin",
    "SkeletonData"
]

for i in names:
    with open(os.path.join(cd, "include", f"Py{i}.h"), 'w', encoding='utf-8') as f:
        f.write(header_template.replace("@@NAME@@", i))
    with open(os.path.join(cd, "src", f"Py{i}.cpp"), 'w', encoding='utf-8') as f:
        f.write(source_template.replace("@@NAME@@", i))

includes = [f"#include <Py{i}.h>\n" for i in names]
includes += "#include <PySpine.h>\n"
includes += "#define Py_LIMITED_API\n"
includes += "#include <Python.h>\n\n"

body = """PyMODINIT_FUNC PyInit_spine(void)
{
    printf("spine-py (built with %%s)\\n", PY_VERSION);
    auto m = PyModule_Create(&PySpine);

    PyObject* type;
     
%s

    return m;
}
""" % "".join(["""    type = PyType_FromSpec(&Py{0}_spec);
    PyModule_AddObject(m, "{0}", type);
""".format(i) for i in names])

with open(os.path.join(cd, "src", "spine-py.cpp"), 'w', encoding='utf-8') as f:
    f.write("".join(includes) + body)