/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "knapsack.h"

//calcula a solucao passada em tempo linear

int calcula_solucao(int d, int B, vector<int> p, vector<int> w, vector<int> c, vector<int> sol) {
    set<int> classes;
    int i;
    int total = 0;
    int peso = 0;
    for (i = 0, classes.insert(c[0]); i < sol.size(); i++) {
        if (sol[i] == 0) {
            continue;
        }
        //caso uma nova classe seja introduzida na solucao, gasta uma separacao e um de espaco na mochila
        if (classes.find(c[i]) != classes.end()) {
            classes.insert(c[i]);
            d--;
            B--;
            if (d < 0) {
                return -1; //resultado infactivel
            }
        }
        total += p[i];
        peso += w[i];
        if (peso > B) {
            return -1; //peso superou a capacidade da mochila, entao ja eh infactivel
        }
    }

    return total;
}

//auxiliar para imprimir a solucao enquanto eh gerada (remover quando submeter o lab)

void print_vector(vector<int> sol) {
    vector<int>::iterator it;
    for (it = sol.begin(); it != sol.end(); it++) {
        cout << *it << " ";
    }
    cout << "\n";
}

//funcao recursiva que executa o backtracking

bool bt_rec(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t, vector<int> sol_temp, int total) {
    // vai de i=n -> 0
    if (n > 0) {
        n--;
        int total_temp = calcula_solucao(d, B, p, w, c, sol_temp);
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
    vector<int> sol_temp(n);
    return bt_rec(n, d, B, p, w, c, sol, t, sol_temp, 0);
}

// funcao recursiva do branch-and-bound

bool bnb_rec(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t, vector<int> sol_temp, int total) {
    int j;
    print_vector(sol);
    if (n > 0) {
        n--;
        int total_temp = calcula_solucao(d, B, p, w, c, sol_temp);
        if (total_temp > total) {
            copy(sol_temp.begin(), sol_temp.end(), sol.begin());
            total = total_temp;
        }
        //corta a branch se ela ja passar do valor permitido - do tempo permititdo?
        if (total_temp >= 0) {
            bt_rec(n, d, B, p, w, c, sol, t, sol_temp, total);
            sol_temp[n] = 1;
            bt_rec(n, d, B, p, w, c, sol, t, sol_temp, total);
        }
    }

    return true;
}

int space_left(int i, int d, int B, vector<int> &w, vector<int> &c, set<int> &classes) {
    //Nova classe
    if (classes.find(c[i]) == classes.end()) {
        classes.insert(c[i]);
        if (classes.size() == 1) {
            return B - w[i];
        }
        return B - w[i] - d;
    }
    return B - w[i];
}

int calc_sol(int n, vector<int> &p, vector<int> &sol) {
    int sol_num = 0;
    for (int i = 0; i < n; i++) {
        if (sol[i] == 1)
            sol_num += p[i];
    }
    return sol_num;
}

void algo_guloso(int i, int d, int n, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, set<int> &classes_temp) {
    vector<int> sol_temp(n);
    int new_B;
    for (int k = 0; k < n; k++) {
        sol_temp[k] = sol[k];
    }

    for (int l = i + 1; l < n; l++) {
        //Testar se tem espaco suficiente para colocar o novo usuario      
        new_B = space_left(l, d, B, w, c, classes_temp);
        if (new_B >= 0) {
            sol_temp[l] = 1;
            //Se a nova solução é melhor que a antiga, adiciono a nova solucao
            if (calc_sol(n, p, sol) < calc_sol(n, p, sol_temp)) {
                sol[l] = 1;
                B = new_B;
            }
        }
    }
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

///
// Branch and Bound function
///

bool bnb(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t) {
    bool time_over = false;
    clock_t begin, end;
    double elapsed_secs;
    
    vector<int> new_positions(n);
    sort_vectors_positions(n, new_positions, p, w);

    vector<int> w2(n), p2(n), c2(n);
    sort_vectors(n, w, w2, new_positions);
    sort_vectors(n, p, p2, new_positions);
    sort_vectors(n, c, c2, new_positions);


    //Solucoes temporarias para o Xi = 0  e Xi = 1, respectivamente e solucao final
    vector<int> sol_temp1(n), sol_temp2(n), sol_final(n);
    set<int> classes, classes_1, classes_2;
    for (int i = 0; i < n; i++) {
        sol_temp1[i] = sol_temp2[i] = 0;
        sol_final[i] = 0;
    }

    begin = clock();

    int sol1, sol2, new_B = B, B_temp;
    for (int i = 0; i < n; i++) {
        sol1 = sol2 = 0;
        sol_temp1[i] = 1;
        //Usar algo guloso pra achar melhor sol com a primeira parte da solucao[0..i] fixa
        //verificar se possivel colocar proximo cliente, se possivel, proseguir, cc nao considerar.
        B_temp = space_left(i, d, new_B, w2, c2, classes_1);
        if (B_temp >= 0) {
            algo_guloso(i, d, n, B_temp, p2, w2, c2, sol_temp1, classes_1);
            sol1 = calc_sol(n, p2, sol_temp1);
        }

        sol_temp2[i] = 0;
        algo_guloso(i, d, n, new_B, p2, w2, c2, sol_temp2, classes_2);
        sol2 = calc_sol(n, p2, sol_temp2);

        //Comparo as duas solucoes
        if (sol1 > sol2) {
            //Copio com Xi=0 pra solucao final
            if (sol1 > calc_sol(n, p2, sol_final)) {
                copy_vector(n, i, sol_temp1, sol_final);
                copy_vector(n, i, sol_temp1, sol_temp2);
                classes.insert(c2[i]);
                new_B = B_temp;
            }

        } else {
            //Copio com Xi=1 pra solucao final
            if (sol2 > calc_sol(n, p2, sol_final)) {
                copy_vector(n, i, sol_temp2, sol_final);
                copy_vector(n, i, sol_temp2, sol_temp1);
            }
        }

        //Limpo as classes para estado da solucao encontrada ate o momento
        classes_1 = set<int> (classes);
        classes_2 = set<int> (classes);

        end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC ;
        if (elapsed_secs > double(t)) {
            time_over = true;
            break;
        }
    }

    //Reorganiza vetor de saida para ordem incial do problema
    vector<int> sol_aux(n);
    for (int i = 0; i < n; i++) {
        sol_aux[new_positions[i]] = sol_final[i];
    }
    copy_vector(n, n, sol_aux, sol);

    
    if (!time_over) {
        end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << "Time(s): " << elapsed_secs << "\n";
    } else {
        cout << "Time over " << elapsed_secs << "\n";
    }
    
    return true;

    //vector<int> sol_temp(n);
    //return bnb_rec(n, d, B, p, w, c, sol, t, sol_temp, 0);
}