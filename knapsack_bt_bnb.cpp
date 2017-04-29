/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "knapsack.h"
#include <stdio.h>

/****************************Funcoes auxiliares**********************************************/

//calcula a solucao passada em tempo linear
int calcula_solucao(int d, int B, vector<int> p, vector<int> w, vector<int> c, vector<int> sol) {
    set<int> classes;
    int i;
    int total = 0;
    int peso = 0;
    for (i = 0; i < sol.size(); i++) {
        if (sol[i] == 0) {
            continue;
        }
	classes.insert(c[i]);
	peso += w[i];
        total += p[i];
    }
    if(peso>B){
      return -1;
    }
    int used_classes = classes.size();
    if ( peso+(used_classes-1)*d > B ){
	return -1;
    }
    return total;
}

int calc_sol(int n, vector<int> &p, vector<int> &sol) {
    int sol_num = 0;
    for (int i = 0; i < n; i++) {
        if (sol[i] == 1)
            sol_num += p[i];
    }
    return sol_num;
}

int calc_peso(int n, vector<int> &w, vector<int> &sol) {
    int sol_num = 0;
    for (int i = 0; i < n; i++) {
        if (sol[i] == 1)
            sol_num += w[i];
    }
    return sol_num;
}


bool compare(float *i, float *j) {
    return (*i>*j);
}

void sort_vectors_positions(int n, vector<int> &new_positions, vector<int> &p, vector<int> &w) {
    vector<float*> w_ref(n);
    vector<float> valor(n);

    for (int i = 0; i < n; i++) {
        valor[i] = float(p[i]) / float(w[i]);
        w_ref[i] = &valor[i];
    }

    sort(w_ref.begin(), w_ref.end(), compare);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (*w_ref[i] == (float(p[j]) / float(w[j]))) {
                new_positions[i] = j;
                break;
            }
        }
    }
}

void sort_vectors(int n, vector<int> &v1, vector<int> &v2, vector<int> &new_positions) {
    for (int i = 0; i < n; i++) {
        v2[i] = v1[new_positions[i]];
    }
}

// avalia se a soma dos precos do resto dos usuarios pode ser melhor que o melhor valor que ja temos
bool existe_melhor_possibilidade(vector<int> p, int n, int melhor_valor){
  int soma = 0;
  for(int i = n; i<p.size(); i++){
    soma += p[i];
  }
  if(soma>melhor_valor){
    return true;
  }
  return false;
}

//auxiliar para imprimir a solucao enquanto eh gerada (remover quando submeter o lab)
void print_vector(vector<int> sol) {
    vector<int>::iterator it;
    for (it = sol.begin(); it != sol.end(); it++) {
        cout << *it << " ";
    }
    cout << "\n";
}

void copy_vector(int n, int l, vector<int> &origem, vector<int> &dest) {
    for (int i = 0; i < n; i++) {
        if (i <= l) {
            dest[i] = origem[i];
        } else {
            dest[i] = 0;
        }
    }
}



/****************************[FIM]Funcoes auxiliares**********************************************/

//funcao recursiva que executa o backtracking
bool bt_rec(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t, vector<int> sol_temp, int total) {
    // vai de i=n -> 0
    if (n > 0) {
        n--;
        int total_temp = calcula_solucao(d, B, p, w, c, sol_temp);
	if(total_temp<0){
	  return false;
	}
        //caso a solucao encontrada nesse ponto seja melhor, atualiza o vetor solucao e seu respectivo valor
        if (total_temp > total) {
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
// RA2:	122924
///
// Bactracking function: 
///

bool bt(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t) {
    vector<int> sol_temp(n, 0);
    bool b = bt_rec(n, d, B, p, w, c, sol, t, sol_temp, 0);
    return b;
}

// funcao recursiva do branch-and-bound
bool bnb_rec(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t, vector<int> sol_temp, int total) {
    int j;

    if (n<p.size()) {
	sol[n] = 1;
        int total_temp = calcula_solucao(d, B, p, w, c, sol_temp);
	if(total_temp<0){
	  return false;
	}
        //caso a solucao encontrada nesse ponto seja melhor, atualiza o vetor solucao e seu respectivo valor
        if (total_temp > total) {
            copy(sol_temp.begin(), sol_temp.end(), sol.begin());
            total = total_temp;
        }
        
        for(int i = n; i<p.size(); i++){
	  if(existe_melhor_possibilidade(p, n, total)){
	    bnb_rec(n+1,d,B, p, w, c, sol, t, sol_temp, total);
	  }
	}
    }

    return true;
}

bool bnb(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t) {
    vector<int> new_positions(n);
    sort_vectors_positions(n, new_positions, p, w);

    vector<int> w2(n), p2(n), c2(n);
    sort_vectors(n, w, w2, new_positions);
    sort_vectors(n, p, p2, new_positions);
    sort_vectors(n, c, c2, new_positions);
    
    vector<int> sol_temp(n, 0);
    bnb_rec(0,d,B, p2,w2,c2,sol,t,sol_temp,0);
    
    //Reorganiza vetor de saida para ordem incial do problema
    vector<int> sol_aux(n);
    for (int i = 0; i < n; i++) {
        sol_aux[new_positions[i]] = sol[i];
    }
    sol = sol_aux;

    return true;
}