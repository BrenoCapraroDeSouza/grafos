
#ifndef GRAFOS_H
#define GRAFOS_H

#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <stack>
#include <limits>
#include <utility>
#include <functional>

using namespace std;

class Grafos {
protected:
vector<string> rotulosVertices; // rótulos dos vértices
bool grafoDirecionado;
bool grafoPonderado;

public:
Grafos(bool direcionado = false, bool ponderado = false)
: grafoDirecionado(direcionado), grafoPonderado(ponderado) {}
virtual ~Grafos() {}

bool ehDirecionado() const { return grafoDirecionado; }
bool ehPonderado() const { return grafoPonderado; }

// operações 
virtual bool inserirVertice(const string& rotulo) = 0;
virtual bool removerVertice(int indiceVertice) = 0;
virtual string rotuloVertice(int indiceVertice) const {
    if (indiceVertice < 0 || indiceVertice >= (int)rotulosVertices.size()) return "";
    return rotulosVertices[indiceVertice];
}
virtual void imprimeGrafo() const = 0;

virtual bool inserirAresta(int indiceOrigem, int indiceDestino, float pesoAresta = 1.0f) = 0;
virtual bool removerAresta(int indiceOrigem, int indiceDestino) = 0;
virtual bool existeAresta(int indiceOrigem, int indiceDestino) const = 0;
virtual float pesoAresta(int indiceOrigem, int indiceDestino) const = 0;
virtual vector<int> retornarVizinhos(int indiceVertice) const = 0;

int numVertices() const { return (int)rotulosVertices.size(); }


string nomeVertice(int indiceVertice) const {
    if (indiceVertice < 0) return "";
    if (indiceVertice < 26) {
        char letra = 'A' + indiceVertice;
        return string(1, letra);
    } else {
        return to_string(indiceVertice); // fallback quando > 'Z'
    }
}

// BFS: retorna a ordem de visita (índices) a partir de um vértice origem
vector<int> bfs(int indiceOrigem) const {
    vector<int> ordemVisita;
    int totalVertices = numVertices();
    if (indiceOrigem < 0 || indiceOrigem >= totalVertices) return ordemVisita;
    vector<bool> visitado(totalVertices, false);
    queue<int> fila;
    visitado[indiceOrigem] = true;
    fila.push(indiceOrigem);
    while (!fila.empty()) {
        int verticeAtual = fila.front(); fila.pop();
        ordemVisita.push_back(verticeAtual);
        for (int vizinho : retornarVizinhos(verticeAtual)) {
            if (!visitado[vizinho]) {
                visitado[vizinho] = true;
                fila.push(vizinho);
            }
        }
    }
    return ordemVisita;
}

// DFS (iterativa): retorna a ordem de visita (pré-ordem)
vector<int> dfs(int indiceOrigem) const {
    vector<int> ordemVisita;
    int totalVertices = numVertices();
    if (indiceOrigem < 0 || indiceOrigem >= totalVertices) return ordemVisita;
    vector<bool> visitado(totalVertices, false);
    stack<int> pilha;
    pilha.push(indiceOrigem);
    while (!pilha.empty()) {
        int verticeAtual = pilha.top(); pilha.pop();
        if (visitado[verticeAtual]) continue;
        visitado[verticeAtual] = true;
        ordemVisita.push_back(verticeAtual);
        auto vizinhos = retornarVizinhos(verticeAtual);
        for (auto it = vizinhos.rbegin(); it != vizinhos.rend(); ++it) pilha.push(*it);
    }
    return ordemVisita;
}

// Dijkstra: retorna par(distancias, predecessor)
// dist = vetor de distância (infinito se inacessível), prev = anterior no caminho (-1 se nenhum)

pair<vector<float>, vector<int>> dijkstra(int indiceOrigem) const {
    int totalVertices = numVertices();
    const float INFINITO = numeric_limits<float>::infinity();
    vector<float> distancia(totalVertices, INFINITO);
    vector<int> anterior(totalVertices, -1);
    if (indiceOrigem < 0 || indiceOrigem >= totalVertices) return {distancia, anterior};

    using ParDistVertice = pair<float,int>; // (distancia, vertice)
    priority_queue<ParDistVertice, vector<ParDistVertice>, greater<ParDistVertice>> filaPrioridade;

    distancia[indiceOrigem] = 0.0f;
    filaPrioridade.push({0.0f, indiceOrigem});

    while (!filaPrioridade.empty()) {
        auto [distAtual, verticeAtual] = filaPrioridade.top(); filaPrioridade.pop();
        if (distAtual > distancia[verticeAtual]) continue;
        for (int vizinho : retornarVizinhos(verticeAtual)) {
            float peso = pesoAresta(verticeAtual, vizinho);
            if (!grafoPonderado) peso = 1.0f; // caso grafo não ponderado
            if (peso < 0) continue; // aresta inexistente (defesa)
            if (distancia[verticeAtual] + peso < distancia[vizinho]) {
                distancia[vizinho] = distancia[verticeAtual] + peso;
                anterior[vizinho] = verticeAtual;
                filaPrioridade.push({distancia[vizinho], vizinho});
            }   
        }
    }
    return {distancia, anterior};
}

};

#endif 
