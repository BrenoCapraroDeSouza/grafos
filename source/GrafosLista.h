#ifndef GRAFOS_LISTA_H
#define GRAFOS_LISTA_H

#include "Grafos.h"
#include <algorithm>

using namespace std;

class GrafoLista : public Grafos {
public:
    struct Aresta { int destino; float peso; };

private:
    vector<vector<Aresta>> adj; // lista de adjacencia

public:
    GrafoLista(bool dirigido = false, bool ponderado = false)
        : Grafos(dirigido, ponderado) {}

    bool inserirVertice(const string& label) override {
        labels.push_back(label);
        adj.emplace_back();
        return true;
    }

    bool removerVertice(int indice) override {
        int n = numVertices();
        if (indice < 0 || indice >= n) return false;
        labels.erase(labels.begin() + indice);
        adj.erase(adj.begin() + indice);
        for (int i = 0; i < (int)adj.size(); ++i) {
            auto &lista = adj[i];
            lista.erase(remove_if(lista.begin(), lista.end(),
                                  [&](const Aresta &a){ return a.destino == indice; }),
                        lista.end());
            for (auto &a : lista) if (a.destino > indice) --a.destino;
        }
        return true;
    }

    void imprimeGrafo() const override {
        cout << "Grafo (lista) - V=" << numVertices()
             << " Dirigido=" << dirigido << " Ponderado=" << ponderado << "\n";

        for (int i = 0; i < numVertices(); ++i) {
            cout << nomeVertice(i) << " (" << labelVertice(i) << ") -> ";
            const auto &lista = adj[i];
            if (lista.empty()) {
                cout << "-";
            } else {
                for (size_t k = 0; k < lista.size(); ++k) {
                    if (k) cout << ", ";
                    int dest = lista[k].destino;
                    if (ponderado)
                        cout << nomeVertice(dest) << " (" << labelVertice(dest) << ", w=" << lista[k].peso << ")";
                    else
                        cout << nomeVertice(dest) << " (" << labelVertice(dest) << ")";
                }
            }
            cout << "\n";
        }
    }

    bool inserirAresta(int origem, int destino, float peso = 1.0f) override {
        int n = numVertices();
        if (origem < 0 || origem >= n || destino < 0 || destino >= n) return false;
        float w = ponderado ? peso : 1.0f;
        for (const auto &a : adj[origem]) if (a.destino == destino) return false;
        adj[origem].push_back({destino, w});
        if (!dirigido) {
            // evita duplicata recíproca
            bool found = false;
            for (const auto &a : adj[destino]) if (a.destino == origem) { found = true; break; }
            if (!found) adj[destino].push_back({origem, w});
        }
        return true;
    }

    bool removerAresta(int origem, int destino) override {
        int n = numVertices();
        if (origem < 0 || origem >= n || destino < 0 || destino >= n) return false;
        auto &L = adj[origem];
        L.erase(remove_if(L.begin(), L.end(), [&](const Aresta &a){ return a.destino == destino; }), L.end());
        if (!dirigido) {
            auto &L2 = adj[destino];
            L2.erase(remove_if(L2.begin(), L2.end(), [&](const Aresta &a){ return a.destino == origem; }), L2.end());
        }
        return true;
    }

    bool existeAresta(int origem, int destino) const override {
        int n = numVertices();
        if (origem < 0 || origem >= n || destino < 0 || destino >= n) return false;
        for (const auto &a : adj[origem]) if (a.destino == destino) return true;
        return false;
    }

    float pesoAresta(int origem, int destino) const override {
        if (!existeAresta(origem, destino)) return -1.0f;
        for (const auto &a : adj[origem]) if (a.destino == destino) return a.peso;
        return -1.0f;
    }

    vector<int> retornarVizinhos(int vertice) const override {
        vector<int> viz;
        int n = numVertices();
        if (vertice < 0 || vertice >= n) return viz;
        for (const auto &a : adj[vertice]) viz.push_back(a.destino);
        return viz;
    }
};

#endif // GRAFOS_LISTA_H
