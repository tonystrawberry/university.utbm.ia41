#include "decisionTreesClassifier.h"
#include "exception.h"

#define PY_ARRAY_UNIQUE_SYMBOL my_ARRAY_API
#define NO_IMPORT_ARRAY
#include <numpy/arrayobject.h>

#include <iostream>

using namespace std;


DecisionTreesClassifier::DecisionTreesClassifier(int dimension): _dimension(dimension)
{
}

DecisionTreesClassifier::~DecisionTreesClassifier()
{
}


void DecisionTreesClassifier::classify(double* data, int* labels, double *weights, int nb_samples)
{
//    cout << "classify dt" << endl;

  PyObject *pModule, *resValue;
  PyObject *pDict, *pFunc;

  npy_intp data_dim[2] = {nb_samples, _dimension};
  npy_intp labels_dim[1] = {nb_samples};
  PyArrayObject *samples_array = (PyArrayObject *) PyArray_SimpleNewFromData ( 2, data_dim , NPY_DOUBLE, data) ;
  PyArrayObject *labels_array = (PyArrayObject *) PyArray_SimpleNewFromData ( 1, labels_dim , NPY_INT, labels) ;
  PyArrayObject *weights_array = (PyArrayObject *) PyArray_SimpleNewFromData ( 1, labels_dim , NPY_DOUBLE, weights) ;

  pModule = PyImport_ImportModule("sklearn.tree"); // new ref
  if(pModule == NULL) {
    // If the module was not loaded successfully
    PyErr_Print();
    PyErr_Clear();
    std::cerr << "Failed to import sklearn.tree" << std::endl;
    throw MyException("Cannot import the python module sklearn.tree");
  }

  pDict = PyModule_GetDict(pModule);  // borrowed ref
  pFunc = PyDict_GetItemString(pDict, "DecisionTreeClassifier"); // borrowed ref

  _classifier = NULL;

  /////////////////////////  // Create the classifier

  _classifier = PyObject_CallObject(pFunc, NULL);
  //Don't use :  PyInstance_New(pFunc, NULL, arglist);
  // will raise an internal function error

  // Just handle the case an exception occured within Python
  // and display the stack trace
  if(_classifier == NULL)
    {
      std::cerr << " Failed to create the classifier !!" << std::endl;
      PyErr_Print();
      throw MyException("Unable to instantiate sklearn.tree.DecisionTreeClassifier");
    }

  /////////////////////////
  // Learn the classifier
  if(weights == NULL)
    // We suppose uniform weights
    resValue = PyObject_CallMethod(_classifier, (char*)"fit", (char*)"(OO)", samples_array, labels_array);
  else
    resValue = PyObject_CallMethod(_classifier, (char*)"fit", (char*)"(OOOOiO)", samples_array, labels_array, Py_None, Py_None, true, weights_array);
  // Increments the reference counter, we need to keep this object alive
  Py_INCREF(_classifier);

  // Just handle the case an exception occured within Python
  // and display the stack trace
  if(resValue == NULL)
    {
      PyErr_Print();
      throw MyException("Error while fitting sklearn.tree.DecisionTreeClassifier");
    }

  Py_DECREF(pModule);
  Py_DECREF(resValue);

  Py_CLEAR(samples_array);
  Py_CLEAR(labels_array);
  Py_CLEAR(weights_array);

//  cout << "done" << endl;
}


void DecisionTreesClassifier::predict(double * samples, int nb_samples, int* predicted_labels)
{
//    cout << "predict dt" << endl;

  PyObject *resValue;
  PyArrayObject *samples_array;

  npy_intp data_dim[2] = {nb_samples,_dimension};
  samples_array = (PyArrayObject *) PyArray_SimpleNewFromData ( 2, data_dim , NPY_DOUBLE, samples) ;
  
  resValue = PyObject_CallMethod(_classifier, (char*)"predict", (char*)"(O)", samples_array);

  // Just handle the case an exception occured within Python
  // and display the stack trace
  if(resValue == NULL)
    {
      std::cerr << " Error while trying to predict with DecisionTrees !!" << std::endl;
      PyErr_Print();

      throw MyException("Error while trying to predict with DecisionTrees !!");
    }
  for(int i = 0 ; i < nb_samples; ++i)
     predicted_labels[i] = ((int*)(PyArray_DATA((PyArrayObject*) resValue)))[i];
  
  
  Py_CLEAR(resValue);
  Py_CLEAR(samples_array);

//  cout << "done" << endl;
}
