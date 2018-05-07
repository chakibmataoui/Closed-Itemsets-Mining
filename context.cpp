#include "context.h"

context::context(char* context_file){
  string file_name(context_file);
  fstream filter(file_name);
  vector<string> grid;
  string line;
  while(getline(filter,line)){
    grid.push_back(line);
  }
  size_i = grid.size();
  size_j = grid[0].length() - ((grid[0].length()-1)/2);
  for (std::size_t row = 0; row < size_i; ++row)
    {
      string res = grid[row];
      context_matrix.push_back(vector<bool>());
      for (std::size_t col = 0; col < size_j; ++col)
      {
	context_matrix[row].push_back(stoi(res.substr((col)*2,1)));
      }
    }
  return;
  //Affich part
  cout << "size i : " << size_i << " size_j : " << size_j << endl;
  for(int i = 0; i < size_i;++i){
    cout << i << " : " ;
    for(int j = 0; j < size_j;++j)
      cout << context_matrix[i][j] << " " ;
    cout << endl;
  }
}

vector<vector<int>> context::close(double minsup){
  cout << "Demmarage close" << endl;
  //On demmare par les items singletons
  vector<vector<int>> generateurs;
  vector<int> ffc1;
  for(int j = 0; j < size_j;j++){
    generateurs.push_back(vector<int>());
    generateurs[j].push_back(j);
    ffc1.push_back(j);
  }
  bool generer = false;
  vector<vector<int>> fermeture;
  cout << "Debut de génération " << endl;
  do{
    generer = false;
    
    for(int gen = 0; gen < generateurs.size(); ++gen){
      //calculer le sup
      vector<int> cols(generateurs[gen]);
      double sup = this->support(cols);
      if(sup < minsup){
        if(generateurs[gen].size() == 1){
	  ffc1.erase(find(ffc1.begin(),ffc1.end(),generateurs[gen][0]));
	}
	  
	generateurs.erase(generateurs.begin()+gen);
	gen--;
	continue;
      }
      //calculer la fermeture
      vector<int> ferme(generateurs[gen]);
      for(int col = 0; col < size_j;++col){
	if(find(cols.begin(),cols.end(),col) != cols.end()){
	  continue;
	}
	//On vérifie si on peut ajouter l'item a la fermeture ou non
	bool ajout = true;
	for(int i = 0; i < size_i;++i){
	  bool verif = true;
	  //On vérifie que les cases des items du meme generateurs sont 1
	  for(int j = 0; j < cols.size();++j){
	    if(context_matrix[i][cols[j]] == false){
	      verif = false;
	      break;
	    }
	  }
	  //Verifier et continuer
	  if(verif){
	    if(context_matrix[i][col] == false)
	      ajout = false;
	  }
	  
	}
	if(ajout)
	  ferme.push_back(col);
      }
      sort(ferme.begin(),ferme.end());
      if(find(fermeture.begin(),fermeture.end(),ferme) == fermeture.end())
	fermeture.push_back(ferme);
    }
   

    //ajouter les nouveau generateur 
    int fc_size = generateurs.size();
    for(int i = 0; i < fc_size;i++){
      //On copie le générateur
      vector<int> g(generateurs[i]);
      //On ajoute pour un générateur toutes les possibilités
      for(int j = 0; j < ffc1.size();j++){
	if(find(g.begin(),g.end(),ffc1[j]) != g.end())
	  continue;
        g.push_back(ffc1[j]);
	sort(g.begin(),g.end());
	//On cherche si il n'existe pas déja dans une fermeture
	bool exist = false;
	for(int f = 0; f < fermeture.size();++f){
	  bool found = true;
	  for(int z = 0; z < g.size();++z)
	    if(find(fermeture[f].begin(),fermeture[f].end(),g[z]) == fermeture[f].end())
	      found = false;
	  if(found)
	    exist = true;
	}
	if(!exist){
	  bool found = false;
	  if(find(generateurs.begin(),generateurs.end(),g) == generateurs.end()){
	    generateurs.push_back(g);
	    generer = true;
	  }
	}
	g.erase(find(g.begin(),g.end(),ffc1[j]));
      }
    }
    generateurs.erase(generateurs.begin(),generateurs.begin()+fc_size);
  }while(generer);
  
  return fermeture;
}

double context::support(vector<int> cols){
  int nbr = 0;
  for(int i = 0; i < size_i; ++i){
    bool is_true = true;
    for(int j = 0; j < cols.size(); ++j){
      if(context_matrix[i][cols[j]] == false){
	is_true = false;
	break;
      }
    }
    if(is_true)
      nbr++;
  }
  //cout << nbr << "/" << size_i << endl;
  return (double)nbr/size_i;
}

context::context(context &mat,int deb,int fin){
  size_i = fin - deb ;
  size_j = mat.size_j;
  for(int i = deb; i < fin;++i)
    context_matrix.push_back(mat.context_matrix[i]);
}

vector<vector<int>> context::intersect(vector<vector<int>> fermeture1,vector<vector<int>> fermeture2){
  vector<vector<int>> fermeture_large = (fermeture1.size() > fermeture2.size()?fermeture1:fermeture2);
  vector<vector<int>> fermeture_small = (fermeture1.size() > fermeture2.size()?fermeture2:fermeture1);
  vector<vector<int>> intersection;
  for(int i = 0; i < fermeture_small.size();++i){
    if(find(fermeture_large.begin(),fermeture_large.end(),fermeture_small[i]) != fermeture_large.end()){
      intersection.push_back(fermeture_small[i]);
    }
  }
  return intersection;
}

vector<vector<int>> context::unif(vector<vector<int>> fermeture1,vector<vector<int>> fermeture2){
  vector<vector<int>> fermeture_large = (fermeture1.size() > fermeture2.size()?fermeture1:fermeture2);
  vector<vector<int>> fermeture_small = (fermeture1.size() > fermeture2.size()?fermeture2:fermeture1);
  vector<vector<int>> union_f(fermeture_large);
  for(int i = 0; i < fermeture_small.size();++i){
    if(find(fermeture_large.begin(),fermeture_large.end(),fermeture_small[i]) == fermeture_large.end()){
      union_f.push_back(fermeture_small[i]);
    }
  }
  return union_f;
}
