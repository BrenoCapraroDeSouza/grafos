#ifndef GRAFOS_MATRIZ_H
#define GRAFOS_MATRIZ_H

#include "Grafos.h"
#include <algorithm>

using namespace std;

class GrafoMatriz : public Grafos {
private:
    // usamos -1.0f para indicar ausência de aresta
    vector<vector<float>> matriz;
    const float NO_EDGE = -1.0f;

public:
    GrafoMatriz(bool dirigido = false, bool ponderado = false)
        : Grafos(dirigido, ponderado) {}

    bool inserirVertice(const string& label) override {
        labels.push_back(label);
        int n = numVertices();
        matriz.resize(n);
        for (int i = 0; i < n; ++i) {
            matriz[i].resize(n, NO_EDGE);
        }
        return true;
    }

    bool removerVertice(int indice) override {
        int n = numVertices();
        if (indice < 0 || indice >= n) return false;
        labels.erase(labels.begin() + indice);
        matriz.erase(matriz.begin() + indice);
        for (auto &row : matriz) row.erase(row.begin() + indice);
        return true;
    }

    void imprimeGrafo() const override {
        cout << "Grafo (matriz) - V=" << numVertices()
             << " Dirigido=" << dirigido << " Ponderado=" << ponderado << "\n";

        for (int i = 0; i < numVertices(); ++i) {
            // nomeVertice(i) vem do Grafos.h e já devolve A, B, C...
            cout << nomeVertice(i) << " -> ";
            bool first = true;
            for (int j = 0; j < numVertices(); ++j) {
                if (matriz[i][j] != NO_EDGE) {
                    if (!first) cout << ", ";
                    first = false;
                    if (ponderado)
                        cout << nomeVertice(j) << "(w=" << matriz[i][j] << ")";
                    else
                        cout << nomeVertice(j);
                }
            }
            if (first) cout << "-";
            cout << "\n";
        }
    }

    bool inserirAresta(int origem, int destino, float peso = 1.0f) override {
        int n = numVertices();
        if (origem < 0 || origem >= n || destino < 0 || destino >= n) return false;
        float w = ponderado ? peso : 1.0f;
        matriz[origem][destino] = w;
        if (!dirigido) matriz[destino][origem] = w;
        return true;
    }

    bool removerAresta(int origem, int destino) override {
        int n = numVertices();
        if (origem < 0 || origem >= n || destino < 0 || destino >= n) return false;
        matriz[origem][destino] = NO_EDGE;
        if (!dirigido) matriz[destino][origem] = NO_EDGE;
        return true;
    }

    bool existeAresta(int origem, int destino) const override {
        int n = numVertices();
        if (origem < 0 || origem >= n || destino < 0 || destino >= n) return false;
        return matriz[origem][destino] != NO_EDGE;
    }

    float pesoAresta(int origem, int destino) const override {
        if (!existeAresta(origem, destino)) return -1.0f;
        return matriz[origem][destino];
    }

    vector<int> retornarVizinhos(int vertice) const override {
        vector<int> viz;
        int n = numVertices();
        if (vertice < 0 || vertice >= n) return viz;
        for (int j = 0; j < n; ++j)
            if (matriz[vertice][j] != NO_EDGE)
                viz.push_back(j);
        return viz;
    }
};

#endif // GRAFOS_MATRIZ_H
