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
// Função auxiliar que lê V, A, D, P e as arestas do arquivo.
// Retorna true se leu com sucesso; os dados lidos são passados por referência.
static bool lerCabecalhoEArestas(const string &arquivo, int &V, int &A, int &D, int &P, vector<tuple<int,int,float>> &arestas, bool expectPeso) {
    ifstream in(arquivo);
    if (!in) return false;
    if (!(in >> V >> A >> D >> P)) return false;
    arestas.clear();
    for (int e = 0; e < A; ++e) {
        int origem, destino;
        float peso = 1.0f;
        if (!(in >> origem >> destino)) return false;
        if (expectPeso) {
            if (!(in >> peso)) return false;
        }
        arestas.emplace_back(origem, destino, peso);
    }
    return true;
}
};

#endif