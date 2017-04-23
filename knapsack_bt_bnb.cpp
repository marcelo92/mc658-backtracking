/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "knapsack.h"

//calcula a solucao passada em tempo linear
int calcula_solucao(int d, int B, vector<int> p, vector<int> w, vector<int> c, vector<int> sol){
  set<int> classes;
  int i;
  int total = 0;
  int peso = 0;
  for(i=0, classes.insert(c[0]); i<sol.size(); i++){
    if(sol[i]==0){
      continue;
    }
    //caso uma nova classe seja introduzida na solucao, gasta uma separacao e um de espaco na mochila
    if( classes.find(c[i]) != classes.end() ){
      classes.insert(c[i]);
      d--;
      B--;
      if(d<0){
	return -1; //resultado infactivel
      }
    }
    total += p[i];
    peso += w[i];
    if(peso>B){
      return -1; //peso superou a capacidade da mochila, entao ja eh infactivel
    }
  }
  
  return total;
}

//auxiliar para imprimir a solucao enquanto eh gerada (remover quando submeter o lab)
void print_vector(vector<int> sol){
  vector<int>::iterator it;
  for(it = sol.begin(); it!=sol.end(); it++){
    cout << *it << " ";
  }
  cout << "\n";
}

//funcao recursiva que executa o backtracking
bool bt_rec(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t, vector<int> sol_temp, int total){
	// vai de i=n -> 0
	if(n>0){
	  n--;
	  int total_temp = calcula_solucao(d, B, p, w, c, sol_temp);
	  //caso a solucao encontrada nesse ponto seja melhor, atualiza o vetor solucao e seu respectivo valor
	  if(total_temp>total){
	    copy(sol_temp.begin(), sol_temp.end(), sol.begin());
	    total = total_temp;
	  }
	  //faz duas chamadas recursivas, uma passando o mesmo vetor, outra trocando a posicao atual por 1
	  bt_rec(n, d, B, p, w, c, sol, t, sol_temp, total);
	  sol_temp[n] = 1;
	  bt_rec(n, d, B, p, w, c, sol, t, sol_temp, total);
	}
  
	return true;
}

///Preencher aqui para facilitar a correcao. 
// Nome1: Marcelo Teixeira
// RA1: 122128
// Nome2: Guilherme Sena
// RA2:	
///
// Bactracking function: 
///
bool bt(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){
	vector<int> sol_temp(n);
	return bt_rec(n, d, B, p, w, c, sol, t, sol_temp, 0);
}

// funcao recursiva do branch-and-bound
bool bnb_rec(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t, vector<int> sol_temp, int total){
	int j;
	print_vector(sol);
	if(n>0){
	  n--;
	  int total_temp = calcula_solucao(d, B, p, w, c, sol_temp);
	  if(total_temp>total){
	    copy(sol_temp.begin(), sol_temp.end(), sol.begin());
	    total = total_temp;
	  }
	  //corta a branch se ela ja passar do valor permitido
	  if(total_temp>=0){
	    bt_rec(n, d, B, p, w, c, sol, t, sol_temp, total);
	    sol_temp[n] = 1;
	    bt_rec(n, d, B, p, w, c, sol, t, sol_temp, total);
	  }
	}
  
	return true;
}

///
// Branch and Bound function
///
bool bnb(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){
	vector<int> sol_temp(n);
	return bnb_rec(n, d, B, p, w, c, sol, t, sol_temp, 0);
}