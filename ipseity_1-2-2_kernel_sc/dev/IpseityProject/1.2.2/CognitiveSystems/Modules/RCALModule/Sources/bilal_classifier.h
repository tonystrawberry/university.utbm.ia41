#ifndef BILAL_CLASSIFIER_H
#define BILAL_CLASSIFIER_H

#include <Python.h>
#include <vector>


class DecisionTreesClassifier;


// La classe BilalClassifier contient 1 déstructeur, 1 constructeur et 2 méthodes. La méthode classify permet d'entrainer le classifier qui est composé de weak_learners.
// La méthode predict permet de prédire l'action à choisir en fonction de l'état en entrée.

class BilalClassifier
{
// Les weak_learners sont les classifieurs faible dont la somme pondérée va nous donner notre classifieur final
private:
	std::vector<DecisionTreesClassifier*> weak_learners;

public:
//constructeur
	BilalClassifier();
//destructeur
	~BilalClassifier(); 
// méthode classify qui entraîne notre classifieur. En entrée, on a les états experts, états non experts, actions experts, actions non experts, états suivants expert et non expert,
// ainsi que le nombre d'actions, Nbase correspond au nombre de weak_learners, le nombre d'exemples NComponents_states, la dimension de l'espace d'état NDimensions_states.
void classify(double* states_array_expert, int* actions_array_expert, double* next_states_array_expert, double* states_array_non_expert, int* actions_array_non_expert, double* next_states_array_non_expert, int n_a, int N_base, int NComponents_states_expert, int NComponents_states_non_expert, int NDimensions_states, double lambda, double gamma = 0.99, double epsilon_tree = 0.01 );
// La méthode predict prend en entrée un tableau d'états states_array, la taille du tableau nb_samples, le nombre d'action possibles, le nombre de classifieurs faible et la dimension de l'espace d'état et renvoie les actions optimales correspondant au tableau states_array
void predict(double* states_array,int nb_samples,int n_a,int N_base,int NDimensions_states,int * results_prediction);
	
};


#endif
