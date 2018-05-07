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
  context(char* context_file);
  context(context &mat,int deb,int fin);
  vector<vector<bool>> context_matrix;
  int size_i,size_j;
  vector<vector<int>> close(double minsup);
  double support(vector<int> cols);
  vector<vector<int>> intersect(vector<vector<int>> fermeture1,vector<vector<int>> fermeture2);
  vector<vector<int>> unif(vector<vector<int>> fermeture1,vector<vector<int>> fermeture2);
  
};
