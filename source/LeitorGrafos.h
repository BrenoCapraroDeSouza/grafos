#ifndef LEITOR_GRAFOS_H
#define LEITOR_GRAFOS_H

#include <fstream>
#include <sstream>
#include <memory>
#include <stdexcept>
#include "GrafosMatriz.h"
#include "GrafosLista.h"

using namespace std;

class LeitorGrafos {
public:
// Função auxiliar que apenas lê V e A (e as linhas das arestas) do arquivo.
// Retorna true se leu com sucesso; os dados lidos são passados por referência.
static bool lerCabecalhoEArestas(const string &arquivo, int &V, int &A, vector<tuple<int,int,float>> &arestas, bool expectPeso) {
    ifstream in(arquivo);
    if (!in) return false;
    int D, P;
    if (!(in >> V >> A >> D >> P)) return false;
    arestas.clear();
    for (int e = 0; e < A; ++e) {
        int ao, ad;
        float ap = 1.0f;
        if (!(in >> ao >> ad)) return false;
        if (expectPeso) {
            if (!(in >> ap)) return false;
        }
        arestas.emplace_back(ao, ad, ap);
    }
    return true;
}
};

#endif