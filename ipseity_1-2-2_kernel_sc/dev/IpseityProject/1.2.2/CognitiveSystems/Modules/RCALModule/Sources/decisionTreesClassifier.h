#ifndef DECISION_TREES_CLASSIFIER_H
#define DECISION_TREES_CLASSIFIER_H

//#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h> // Must appear first

#include <iostream>
#include <cstdio>

class DecisionTreesClassifier
{	
 private:
  int _dimension;
  PyObject* _classifier;

 public:
  DecisionTreesClassifier(int dimension);
  ~DecisionTreesClassifier();
  void classify(double * samples, int* labels, double * weights, int nb_samples);
  void predict(double * samples, int nb_samples, int* predicted_labels);
};


#endif
