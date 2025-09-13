#ifndef GRAFOS_LISTA_H
#define GRAFOS_LISTA_H

#include "Grafos.h"
#include <algorithm>

using namespace std;

class GrafoLista : public Grafos {
public:
    struct Aresta { int indiceDestino; float pesoAresta; };

private:
    vector<vector<Aresta>> listaAdjacencia; // lista de adjacência

public:
    GrafoLista(bool dirigido = false, bool ponderado = false)
        : Grafos(dirigido, ponderado) {}

    bool inserirVertice(const string& rotuloVertice) override {
        rotulosVertices.push_back(rotuloVertice);
        listaAdjacencia.emplace_back();
        return true;
    }

    bool removerVertice(int indiceVertice) override {
        int totalVertices = numVertices();
        if (indiceVertice < 0 || indiceVertice >= totalVertices) return false;
        rotulosVertices.erase(rotulosVertices.begin() + indiceVertice);
        listaAdjacencia.erase(listaAdjacencia.begin() + indiceVertice);
        for (int i = 0; i < (int)listaAdjacencia.size(); ++i) {
            auto &lista = listaAdjacencia[i];
            lista.erase(remove_if(lista.begin(), lista.end(),
                                  [&](const Aresta &aresta){ return aresta.indiceDestino == indiceVertice; }),
                        lista.end());
            for (auto &aresta : lista) if (aresta.indiceDestino > indiceVertice) --aresta.indiceDestino;
        }
        return true;
    }

    void imprimeGrafo() const override {
        cout << "Grafo (lista) - V=" << numVertices()
             << " Dirigido=" << grafoDirecionado << " Ponderado=" << grafoPonderado << "\n";

        for (int indiceVertice = 0; indiceVertice < numVertices(); ++indiceVertice) {
            cout << nomeVertice(indiceVertice) << " (" << rotuloVertice(indiceVertice) << ") -> ";
            const auto &lista = listaAdjacencia[indiceVertice];
            if (lista.empty()) {
                cout << "-";
            } else {
                for (size_t indiceAresta = 0; indiceAresta < lista.size(); ++indiceAresta) {
                    if (indiceAresta) cout << ", ";
                    int destino = lista[indiceAresta].indiceDestino;
                    if (grafoPonderado)
                        cout << nomeVertice(destino) << " (" << rotuloVertice(destino) << ", w=" << lista[indiceAresta].pesoAresta << ")";
                    else
                        cout << nomeVertice(destino) << " (" << rotuloVertice(destino) << ")";
                }
            }
            cout << "\n";
        }
    }

    bool inserirAresta(int indiceOrigem, int indiceDestino, float pesoAresta = 1.0f) override {
        int totalVertices = numVertices();
        if (indiceOrigem < 0 || indiceOrigem >= totalVertices || indiceDestino < 0 || indiceDestino >= totalVertices) return false;
        float pesoFinal = grafoPonderado ? pesoAresta : 1.0f;
        for (const auto &aresta : listaAdjacencia[indiceOrigem]) if (aresta.indiceDestino == indiceDestino) return false;
        listaAdjacencia[indiceOrigem].push_back({indiceDestino, pesoFinal});
        if (!grafoDirecionado) {
            // evita duplicata recíproca
            bool encontrado = false;
            for (const auto &aresta : listaAdjacencia[indiceDestino]) if (aresta.indiceDestino == indiceOrigem) { encontrado = true; break; }
            if (!encontrado) listaAdjacencia[indiceDestino].push_back({indiceOrigem, pesoFinal});
        }
        return true;
    }

    bool removerAresta(int indiceOrigem, int indiceDestino) override {
        int totalVertices = numVertices();
        if (indiceOrigem < 0 || indiceOrigem >= totalVertices || indiceDestino < 0 || indiceDestino >= totalVertices) return false;
        auto &listaOrigem = listaAdjacencia[indiceOrigem];
        listaOrigem.erase(remove_if(listaOrigem.begin(), listaOrigem.end(), [&](const Aresta &aresta){ return aresta.indiceDestino == indiceDestino; }), listaOrigem.end());
        if (!grafoDirecionado) {
            auto &listaDestino = listaAdjacencia[indiceDestino];
            listaDestino.erase(remove_if(listaDestino.begin(), listaDestino.end(), [&](const Aresta &aresta){ return aresta.indiceDestino == indiceOrigem; }), listaDestino.end());
        }
        return true;
    }

    bool existeAresta(int indiceOrigem, int indiceDestino) const override {
        int totalVertices = numVertices();
        if (indiceOrigem < 0 || indiceOrigem >= totalVertices || indiceDestino < 0 || indiceDestino >= totalVertices) return false;
        for (const auto &aresta : listaAdjacencia[indiceOrigem]) if (aresta.indiceDestino == indiceDestino) return true;
        return false;
    }

    float pesoAresta(int indiceOrigem, int indiceDestino) const override {
        if (!existeAresta(indiceOrigem, indiceDestino)) return -1.0f;
        for (const auto &aresta : listaAdjacencia[indiceOrigem]) if (aresta.indiceDestino == indiceDestino) return aresta.pesoAresta;
        return -1.0f;
    }

    vector<int> retornarVizinhos(int indiceVertice) const override {
        vector<int> vizinhos;
        int totalVertices = numVertices();
        if (indiceVertice < 0 || indiceVertice >= totalVertices) return vizinhos;
        for (const auto &aresta : listaAdjacencia[indiceVertice]) vizinhos.push_back(aresta.indiceDestino);
        return vizinhos;
    }
};

#endif // GRAFOS_LISTA_H
