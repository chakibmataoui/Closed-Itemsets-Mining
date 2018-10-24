#pragma once
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

using namespace std;

class context {
 public:
  context();
  //On donne le nom de fichier csv pour l'ouvrir
  context(char* context_file);
  //On prend un context déja existant et on en sort une sous matrice
  context(context &mat,int deb,int fin);
  //La matrice de context
  vector<vector<bool>> context_matrix;
  //La taille i j de la matrice
  int size_i,size_j;
  //Algorithme close simple
  vector<vector<int>> close(double minsup);
  //Calcule de support en donnant les colones considérés en entrée
  double support(vector<int> cols);
  //Intersection de 2 fermeture
  vector<vector<int>> intersect(vector<vector<int>> fermeture1,vector<vector<int>> fermeture2);
  //Union de 2 fermeture
  vector<vector<int>> unif(vector<vector<int>> fermeture1,vector<vector<int>> fermeture2);
  int support_simple(vector<int> cols);
  void generation_regles_assoc(vector<vector<int>> fermeture);
};
