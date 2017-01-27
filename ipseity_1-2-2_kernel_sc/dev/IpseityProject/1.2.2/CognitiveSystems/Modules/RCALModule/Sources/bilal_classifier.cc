// Dans ce fichier on implémente le constructeur et le déstructuer de la classe BilalClassifier qui permet de faire de la classification régularisée (c'est l'algorithme RCAL).
// On a aussi l'implémentation de deux méthodes de cette classe: classify et predict. La méthode classify permet d'entraîner une base d'arbre qui va permettre de pouvoir ensuite
// évaluer pour tout état s l'action à prendre a. Pour pouvoir entraîner cette base d'arbre, on a besoin de transitions (s,a,s') de l'expert et (si possible) de transitions (s,a,s')
// non expertes. La méthode predict quant à elle permet de prédire une fois que la base d'arbre est entraînée l'action à prendre en fonction de l'état en entrée.


#include "bilal_classifier.h"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <math.h>


#define PY_ARRAY_UNIQUE_SYMBOL my_ARRAY_API
#include <numpy/arrayobject.h>

#include "decisionTreesClassifier.h"


BilalClassifier::BilalClassifier()
{
    _import_array();
}

BilalClassifier::~BilalClassifier()
{
    for(unsigned int i = 0 ; i < weak_learners.size() ; ++i)
    {
        delete weak_learners[i];
    }
}


void BilalClassifier::classify(
        double* states_array_expert, int* actions_array_expert, double* next_states_array_expert,
        double* states_array_non_expert, int* actions_array_non_expert, double* next_states_array_non_expert,
        int n_a, int N_base,
        int NComponents_states_expert, int NComponents_states_non_expert, int NDimensions_states,
        double lambda, double gamma, double epsilon_tree )
{
 // variables doubles
  double buffer_y=0; 	
  // matrices pour l'algo
  gsl_vector * theta=gsl_vector_calloc (N_base);
  gsl_matrix * phi=gsl_matrix_calloc (NComponents_states_expert*n_a,N_base);
  gsl_matrix * phi_r=gsl_matrix_calloc (NComponents_states_non_expert,N_base);
  gsl_matrix * phi_r_next=gsl_matrix_calloc (NComponents_states_non_expert*n_a,N_base);
  gsl_vector * margin=gsl_vector_calloc (NComponents_states_expert*n_a);
  gsl_vector_set_all (margin, 0.5);
  gsl_matrix * m=gsl_matrix_calloc (NComponents_states_expert*n_a,NDimensions_states+1);
  gsl_matrix * m_r=gsl_matrix_calloc (NComponents_states_non_expert,NDimensions_states+1);
  gsl_matrix * m_r_next=gsl_matrix_calloc (NComponents_states_non_expert*n_a,NDimensions_states+1);
  gsl_vector * Q=gsl_vector_calloc (NComponents_states_expert*n_a);
  gsl_vector * Q_r=gsl_vector_calloc (NComponents_states_non_expert);
  gsl_vector * Q_r_next=gsl_vector_calloc (NComponents_states_non_expert*n_a);
  gsl_vector * Q_classif=gsl_vector_calloc (NComponents_states_expert*n_a);
  int * a_max=new int[NComponents_states_expert];
  int * a_max_r=new int[NComponents_states_non_expert];
  gsl_matrix * input=gsl_matrix_calloc (2*NComponents_states_expert+2*NComponents_states_non_expert,NDimensions_states+1);
  int * label=new int[2*NComponents_states_expert+2*NComponents_states_non_expert];
  double * weight=new double[2*NComponents_states_expert+2*NComponents_states_non_expert];  	

  //utilisation de vecteurs tampons
  gsl_vector * vector_input=gsl_vector_calloc (NDimensions_states+1);
  gsl_vector * vector_input1=gsl_vector_calloc (NDimensions_states+1);
  gsl_vector * vector_action=gsl_vector_calloc (n_a);

  // delete  weak learners
    for(unsigned int i = 0 ; i < weak_learners.size() ; ++i)
    {
        delete weak_learners[i];
    }


  // initialisation des matrices et des tableaux
 // initialisation de la matrice m
  for(int i = 0 ; i < NComponents_states_expert ; ++i)
    {
	for(int k = 0 ; k < NDimensions_states ; ++k)
	{
	gsl_vector_set (vector_input,k,states_array_expert[k+i*NDimensions_states]);
	}
      for(int j = 0 ; j < n_a ; ++j)
  	{
	gsl_vector_set (vector_input,NDimensions_states,j+1);
	gsl_matrix_set_row (m,i+j*NComponents_states_expert,vector_input);
	}
    }
 // initialisation de la matrice m_r et m_r_next
  for(int i = 0 ; i < NComponents_states_non_expert ; ++i)
    {
	for(int k = 0 ; k < NDimensions_states ; ++k)
	{
	gsl_vector_set (vector_input,k,states_array_non_expert[k+i*NDimensions_states]);
	gsl_vector_set (vector_input1,k,next_states_array_non_expert[k+i*NDimensions_states]);
	}
	gsl_vector_set (vector_input,NDimensions_states,actions_array_non_expert[i]);
	gsl_matrix_set_row (m_r,i,vector_input);
      for(int j = 0 ; j < n_a ; ++j)
  	{
	gsl_vector_set (vector_input1,NDimensions_states,j+1);
	gsl_matrix_set_row (m_r_next,i+j*NComponents_states_expert,vector_input1);
	}
    }
 // initialisation du vecteur de marge
  for(int i = 0 ; i < NComponents_states_expert ; ++i)
    {
	gsl_vector_set (margin,i+NComponents_states_expert*(actions_array_expert[i]-1),0);
     }

// Corps de functions
int counter=1;
for(int p = 0 ; p < N_base; ++p)
{
// initialisation de Q_classif
gsl_vector_memcpy (Q_classif, Q);
gsl_vector_add (Q_classif, margin);
// initialisation de a_max et a_max_r
	for(int j = 0 ; j < NComponents_states_expert ; ++j)
	{
		for(int k = 0 ; k < n_a ; ++k)
		{
		gsl_vector_set(vector_action,k,gsl_vector_get (Q_classif,j+k*NComponents_states_expert));         
		}
	a_max[j]=gsl_vector_max_index(vector_action)+1;

	}

	for(int j = 0 ; j < NComponents_states_non_expert ; ++j)
	{
		for(int k = 0 ; k < n_a ; ++k)
		{
		gsl_vector_set(vector_action,k,gsl_vector_get (Q_r_next,j+k*NComponents_states_non_expert));         
		}
	a_max_r[j]=gsl_vector_max_index(vector_action)+1;
	}
//on remplit notre base de données pour entrainer l'arbre

int count=0;
	for(int i = 0 ; i < NComponents_states_expert ; ++i)
	{
		if (a_max[i]==actions_array_expert[i]){}
		else
		{
		// remplir le vecteur d'input
		for(int k = 0 ; k < NDimensions_states ; ++k)
		{
			gsl_vector_set (vector_input,k,states_array_expert[k+i*NDimensions_states]);
		}
		gsl_vector_set (vector_input,NDimensions_states,actions_array_expert[i]);

		gsl_matrix_set_row (input,count,vector_input);
		weight[count]=1;
		label[count]=1;
		++count;
		label[count]=-1;
		weight[count]=1;
		gsl_vector_set (vector_input,NDimensions_states,a_max[i]);
		gsl_matrix_set_row (input,count,vector_input);
		++count;
		}
	}

	for(int i = 0 ; i < NComponents_states_non_expert ; ++i)
	{

		buffer_y=gamma*gsl_vector_get(Q_r_next,i+NComponents_states_non_expert*(a_max_r[i]-1))-gsl_vector_get(Q_r,i);
		if (buffer_y<epsilon_tree){}
		else
		{
		weight[count]=2*lambda*NComponents_states_expert/NComponents_states_non_expert*abs(buffer_y);
		if (buffer_y>0)
		{label[count]=1;}
		else
		{label[count]=-1;}
		for(int k = 0 ; k < NDimensions_states ; ++k)
		{
			gsl_vector_set (vector_input,k,states_array_non_expert[k+i*NDimensions_states]);
		}
		gsl_vector_set (vector_input,NDimensions_states,actions_array_non_expert[i]);
		gsl_matrix_set_row (input,count,vector_input);
		++count;
	
		weight[count]=2*gamma*lambda*NComponents_states_expert/NComponents_states_non_expert*abs(buffer_y);
		if (buffer_y>0)
		{label[count]=-1;}
		else
		{label[count]=1;}

		for(int k = 0 ; k < NDimensions_states ; ++k)
		{
			gsl_vector_set (vector_input,k,next_states_array_non_expert[k+i*NDimensions_states]);
		}
		gsl_vector_set (vector_input,NDimensions_states,a_max_r[i]);
		gsl_matrix_set_row (input,count,vector_input);
		++count;
		}
	}

// on trouve notre arbre
if (count==0){}
else
{
DecisionTreesClassifier* decisionTrees_classifier = new DecisionTreesClassifier(NDimensions_states+1);
decisionTrees_classifier->classify(input->data, label, weight, count);
weak_learners.push_back(decisionTrees_classifier);

int* f = new int[NComponents_states_expert*n_a];
decisionTrees_classifier->predict(m->data, NComponents_states_expert*n_a,f);
int* f_r = new int[NComponents_states_non_expert];
decisionTrees_classifier->predict(m_r->data,NComponents_states_non_expert,f_r);
int* f_r_next = new int[NComponents_states_non_expert*n_a];
decisionTrees_classifier->predict(m_r_next->data, NComponents_states_non_expert*n_a,f_r_next);


for(int i = 0 ; i < NComponents_states_expert ; ++i)
{
	for(int j = 0 ; j < n_a ; ++j)	
	{
		gsl_matrix_set(phi,i+NComponents_states_expert*j,p,f[i+NComponents_states_expert*j]);	
	}

}

for(int i = 0 ; i < NComponents_states_non_expert ; ++i)
{
	gsl_matrix_set(phi_r,i,p,f_r[i]);
	for(int j = 0 ; j < n_a ; ++j)	
	{
		gsl_matrix_set(phi_r_next,i+NComponents_states_non_expert*j,p,f_r_next[i+NComponents_states_non_expert*j]);
	}

}
	
gsl_vector_set(theta,p,1.0/(double)counter);
gsl_blas_dgemv (CblasNoTrans,1.0, phi, theta, 0, Q);
gsl_blas_dgemv (CblasNoTrans,1.0, phi_r, theta, 0, Q_r);
gsl_blas_dgemv (CblasNoTrans,1.0, phi_r_next, theta, 0, Q_r_next);
++counter;

delete [] f;
delete [] f_r;
delete [] f_r_next;
}

}

gsl_vector_free (theta);
gsl_matrix_free (m);
gsl_matrix_free (m_r);
gsl_matrix_free (m_r_next);
gsl_vector_free (margin);
gsl_vector_free (Q);
gsl_vector_free (Q_r);
gsl_vector_free (Q_r_next);
gsl_vector_free (Q_classif);
gsl_matrix_free (input);
gsl_matrix_free (phi);
gsl_matrix_free (phi_r);
gsl_matrix_free (phi_r_next);
gsl_vector_free (vector_input);
gsl_vector_free (vector_input1);
gsl_vector_free (vector_action);
delete[] label;
delete[] weight;
delete[] a_max;
delete[] a_max_r;

}


void BilalClassifier::predict(double * states_array,int nb_samples,int n_a,int N_base,int  NDimensions_states, int * results_prediction)
{

gsl_vector * theta=gsl_vector_calloc (N_base);
gsl_matrix * evaluation=gsl_matrix_calloc (nb_samples*n_a,N_base);
gsl_vector * evalue=gsl_vector_calloc (nb_samples*n_a);
int * evalue_tab=new int[nb_samples*n_a];
double * examples_array=new double[nb_samples*n_a*(NDimensions_states+1)];
gsl_vector * Q=gsl_vector_calloc (nb_samples*n_a);
gsl_vector * vector_action=gsl_vector_calloc (n_a);
for(int i = 0 ; i < nb_samples ; ++i)
{

	for(int j = 0 ; j < n_a ; ++j)
	{

		for(int k = 0 ; k < NDimensions_states ; ++k)
		{
			examples_array[k+i*(NDimensions_states+1)+j*(NDimensions_states+1)*nb_samples]=states_array[k+i*(NDimensions_states)];
		}
		examples_array[NDimensions_states+i*(NDimensions_states+1)+j*(NDimensions_states+1)*nb_samples]=j+1;
		
	}


}
for(unsigned int i = 0 ; i < weak_learners.size() ; ++i)
{

	weak_learners[i]->predict(examples_array,nb_samples*n_a,evalue_tab);
	for(int j = 0 ; j < nb_samples*n_a; ++j)
	gsl_vector_set(evalue,j,evalue_tab[j]);

gsl_matrix_set_col(evaluation,i,evalue);
gsl_vector_set(theta,i,1.0/(double)(i+1));
}
gsl_blas_dgemv (CblasNoTrans,1,evaluation, theta, 0, Q);
for(int j = 0 ; j < nb_samples ; ++j)
{
	for(int k = 0 ; k < n_a ; ++k)
	{
	gsl_vector_set(vector_action,k,gsl_vector_get (Q,j+k*nb_samples));         
	}
results_prediction[j]=gsl_vector_max_index (vector_action)+1;

}
gsl_vector_free (theta);
gsl_matrix_free (evaluation);
gsl_vector_free (Q);
gsl_vector_free (evalue);
gsl_vector_free (vector_action);
delete[] evalue_tab;
delete[] examples_array;
}
