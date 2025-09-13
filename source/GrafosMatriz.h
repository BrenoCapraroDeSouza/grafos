#ifndef GRAFOS_MATRIZ_H
#define GRAFOS_MATRIZ_H

#include "Grafos.h"
#include <algorithm>

using namespace std;

class GrafoMatriz : public Grafos {
private:
    // usamos -1.0f para indicar ausência de aresta
    vector<vector<float>> matrizAdjacencia;
    const float SEM_ARESTA = -1.0f;

public:
    GrafoMatriz(bool direcionado = false, bool ponderado = false)
        : Grafos(direcionado, ponderado) {}

    bool inserirVertice(const string& rotuloVertice) override {
        rotulosVertices.push_back(rotuloVertice);
        int totalVertices = numVertices();
        matrizAdjacencia.resize(totalVertices);
        for (int indiceLinha = 0; indiceLinha < totalVertices; ++indiceLinha) {
            matrizAdjacencia[indiceLinha].resize(totalVertices, SEM_ARESTA);
        }
        return true;
    }

    bool removerVertice(int indiceVertice) override {
        int totalVertices = numVertices();
        if (indiceVertice < 0 || indiceVertice >= totalVertices) return false;
        rotulosVertices.erase(rotulosVertices.begin() + indiceVertice);
        matrizAdjacencia.erase(matrizAdjacencia.begin() + indiceVertice);
        for (auto &linha : matrizAdjacencia) linha.erase(linha.begin() + indiceVertice);
        return true;
    }

    void imprimeGrafo() const override {
        cout << "Grafo (matriz) - V= " << numVertices()
             << " Dirigido= " << grafoDirecionado << " Ponderado=" << grafoPonderado << "\n";

        for (int indiceVertice = 0; indiceVertice < numVertices(); ++indiceVertice) {
            cout << nomeVertice(indiceVertice) << " -> ";
            bool primeiroVizinho = true;
            for (int indiceVizinho = 0; indiceVizinho < numVertices(); ++indiceVizinho) {
                if (matrizAdjacencia[indiceVertice][indiceVizinho] != SEM_ARESTA) {
                    if (!primeiroVizinho) cout << ", ";
                    primeiroVizinho = false;
                    if (grafoPonderado)
                        cout << nomeVertice(indiceVizinho) << "(w=" << matrizAdjacencia[indiceVertice][indiceVizinho] << ")";
                    else
                        cout << nomeVertice(indiceVizinho);
                }
            }
            if (primeiroVizinho) cout << "-";
            cout << "\n";
        }

        // Impressão da matriz de adjacência
        cout << "\nMatriz de adjacência:" << endl;
        cout << "  ";
        for (int indiceColuna = 0; indiceColuna < numVertices(); ++indiceColuna) {
            cout << nomeVertice(indiceColuna) << " ";
        }
        cout << endl;
        for (int indiceLinha = 0; indiceLinha < numVertices(); ++indiceLinha) {
            cout << nomeVertice(indiceLinha) << " ";
            for (int indiceColuna = 0; indiceColuna < numVertices(); ++indiceColuna) {
                float valor = matrizAdjacencia[indiceLinha][indiceColuna];
                if (grafoDirecionado) {
                    // Grafo direcionado: imprime matriz normal
                    cout << (valor == SEM_ARESTA ? 0 : valor) << " ";
                } else {
                    // Grafo não direcionado: espelha parte de baixo
                    if (valor == SEM_ARESTA && matrizAdjacencia[indiceColuna][indiceLinha] != SEM_ARESTA)
                        cout << matrizAdjacencia[indiceColuna][indiceLinha] << " ";
                    else
                        cout << (valor == SEM_ARESTA ? 0 : valor) << " ";
                }
            }
            cout << endl;
        }
    }

    bool inserirAresta(int indiceOrigem, int indiceDestino, float pesoAresta = 1.0f) override {
        int totalVertices = numVertices();
        if (indiceOrigem < 0 || indiceOrigem >= totalVertices || indiceDestino < 0 || indiceDestino >= totalVertices) return false;
        float pesoFinal = grafoPonderado ? pesoAresta : 1.0f;
        matrizAdjacencia[indiceOrigem][indiceDestino] = pesoFinal;
        if (!grafoDirecionado) matrizAdjacencia[indiceDestino][indiceOrigem] = pesoFinal;
        return true;
    }

    bool removerAresta(int indiceOrigem, int indiceDestino) override {
        int totalVertices = numVertices();
        if (indiceOrigem < 0 || indiceOrigem >= totalVertices || indiceDestino < 0 || indiceDestino >= totalVertices) return false;
        matrizAdjacencia[indiceOrigem][indiceDestino] = SEM_ARESTA;
        if (!grafoDirecionado) matrizAdjacencia[indiceDestino][indiceOrigem] = SEM_ARESTA;
        return true;
    }

    bool existeAresta(int indiceOrigem, int indiceDestino) const override {
        int totalVertices = numVertices();
        if (indiceOrigem < 0 || indiceOrigem >= totalVertices || indiceDestino < 0 || indiceDestino >= totalVertices) return false;
        return matrizAdjacencia[indiceOrigem][indiceDestino] != SEM_ARESTA;
    }

    float pesoAresta(int indiceOrigem, int indiceDestino) const override {
        if (!existeAresta(indiceOrigem, indiceDestino)) return -1.0f;
        return matrizAdjacencia[indiceOrigem][indiceDestino];
    }

    vector<int> retornarVizinhos(int indiceVertice) const override {
        vector<int> vizinhos;
        int totalVertices = numVertices();
        if (indiceVertice < 0 || indiceVertice >= totalVertices) return vizinhos;
        for (int indiceVizinho = 0; indiceVizinho < totalVertices; ++indiceVizinho)
            if (matrizAdjacencia[indiceVertice][indiceVizinho] != SEM_ARESTA)
                vizinhos.push_back(indiceVizinho);
        return vizinhos;
    }
};

#endif // GRAFOS_MATRIZ_H
