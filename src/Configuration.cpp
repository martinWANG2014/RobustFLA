//
// Created by chenghaowang on 14/07/17.
//
#include "../include/Configuration.h"
#include "../include/Network.h"

double getSigma1(double alpha, double beta, double gamma) {
    return (beta + alpha) / (2 * sqrt(2) * boost::math::erf_inv(gamma));
}

double getSigma2(double alpha, double beta, double gamma) {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
    PyObject *pModule = PyImport_ImportModule("inverse_sigma");
    if (pModule != NULL) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, "inverse_var_sigma_1");
        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject *pArgs = PyTuple_New(3);
            PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(alpha));
            PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(beta));
            PyTuple_SetItem(pArgs, 2, PyFloat_FromDouble(gamma));
            PyObject *pResult = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pFunc);
            Py_DECREF(pArgs);
            double sigma = PyFloat_AsDouble(pResult);
            Py_DECREF(pResult);
            Py_Finalize();
            return sigma;
        } else {
            Py_DECREF(pFunc);
            std::cout << "[ERROR] Can't call the inverse_var_sigma_1 function!" << std::endl;
            abort();
        }
    } else {
        Py_XDECREF(pModule);
        Py_Finalize();
        std::cerr << "[ERROR] Can't load the inverse_sigma module, please check the inverse_sigma.py file!"
                  << std::endl;
        abort();
    }
}

double getSigma3(double alpha, double beta, double gamma, double w1, double w2, double p) {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
    PyObject *pModule = PyImport_ImportModule("inverse_sigma");
    if (pModule != NULL) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, "inverse_var_sigma_2");
        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject *pArgs = PyTuple_New(6);
            PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(alpha));
            PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(beta));
            PyTuple_SetItem(pArgs, 2, PyFloat_FromDouble(gamma));
            PyTuple_SetItem(pArgs, 3, PyFloat_FromDouble(w1));
            PyTuple_SetItem(pArgs, 4, PyFloat_FromDouble(w2));
            PyTuple_SetItem(pArgs, 5, PyFloat_FromDouble(p));
            PyObject *pResult = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pFunc);
            Py_DECREF(pArgs);
            double sigma = PyFloat_AsDouble(pResult);
            Py_DECREF(pResult);
            Py_Finalize();
            return sigma;
        } else {
            Py_DECREF(pFunc);
            std::cout << "[ERROR] Can't call the inverse_var_sigma_1 function!" << std::endl;
            abort();
        }
    } else {
        Py_XDECREF(pModule);
        Py_Finalize();
        std::cerr << "[ERROR] Can't load the inverse_sigma module, please check the inverse_sigma.py file!"
                  << std::endl;
        abort();
    }
}


double getA_Bar() {
    return 0.25 * (Network::PERIOD_LENGTH);
}
