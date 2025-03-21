#include <PyAtlas.h>
#include <PySkeletonJson.h>
#include <PyAnimationStateData.h>
#include <PySkeleton.h>
#include <PySkin.h>
#include <PySkeletonData.h>
#include <PySpine.h>
#define Py_LIMITED_API
#include <Python.h>

PyMODINIT_FUNC PyInit_spine(void)
{
    printf("spine-py (built with %s)\n", PY_VERSION);
    auto m = PyModule_Create(&PySpine);
    PySpineModule = m;
    
    PyObject* type;
     
    type = PyType_FromSpec(&PyAtlas_spec);
    PyModule_AddObject(m, "Atlas", type);
    type = PyType_FromSpec(&PySkeletonJson_spec);
    PyModule_AddObject(m, "SkeletonJson", type);
    type = PyType_FromSpec(&PyAnimationStateData_spec);
    PyModule_AddObject(m, "AnimationStateData", type);
    type = PyType_FromSpec(&PySkeleton_spec);
    PyModule_AddObject(m, "Skeleton", type);
    type = PyType_FromSpec(&PySkin_spec);
    PyModule_AddObject(m, "Skin", type);
    type = PyType_FromSpec(&PySkeletonData_spec);
    PyModule_AddObject(m, "SkeletonData", type);

    return m;
}
