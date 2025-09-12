
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
vector<string> labels; // rótulos dos vértices
bool dirigido;
bool ponderado;

public:
Grafos(bool dirigido = false, bool ponderado = false)
: dirigido(dirigido), ponderado(ponderado) {}
virtual ~Grafos() {}

bool ehDirecionado() const { return dirigido; }
bool ehPonderado() const { return ponderado; }

// operações 
virtual bool inserirVertice(const string& label) = 0;
virtual bool removerVertice(int indice) = 0;
virtual string labelVertice(int indice) const {
if (indice < 0 || indice >= (int)labels.size()) return "";
return labels[indice];
}
virtual void imprimeGrafo() const = 0;

virtual bool inserirAresta(int origem, int destino, float peso = 1.0f) = 0;
virtual bool removerAresta(int origem, int destino) = 0;
virtual bool existeAresta(int origem, int destino) const = 0;
virtual float pesoAresta(int origem, int destino) const = 0;
virtual vector<int> retornarVizinhos(int vertice) const = 0;

int numVertices() const { return (int)labels.size(); }


string nomeVertice(int indice) const {
    if (indice < 0) return "";
    if (indice < 26) {
        char c = 'A' + indice;
        return string(1, c);
    } else {
        return to_string(indice); // fallback quando > 'Z'
    }
}

// BFS: retorna a ordem de visita (índices) a partir de um vértice origem
vector<int> bfs(int origem) const {
    vector<int> ordem;
    int n = numVertices();
    if (origem < 0 || origem >= n) return ordem;
    vector<bool> vis(n, false);
    queue<int> q;
    vis[origem] = true;
    q.push(origem);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        ordem.push_back(u);
        for (int v : retornarVizinhos(u)) {
            if (!vis[v]) {
                vis[v] = true;
                q.push(v);
            }
        }
    }
    return ordem;
}

// DFS (iterativa): retorna a ordem de visita (pré-ordem)
vector<int> dfs(int origem) const {
    vector<int> ordem;
    int n = numVertices();
    if (origem < 0 || origem >= n) return ordem;
    vector<bool> vis(n, false);
    stack<int> st;
    st.push(origem);
    while (!st.empty()) {
        int u = st.top(); st.pop();
        if (vis[u]) continue;
        vis[u] = true;
        ordem.push_back(u);
        // para manter comportamento semelhante ao recursivo, empilhar vizinhos em ordem reversa
        auto viz = retornarVizinhos(u);
        for (auto it = viz.rbegin(); it != viz.rend(); ++it) st.push(*it);
    }
    return ordem;
}

// Dijkstra: retorna par(distancias, predecessor)
// dist = vetor de distância (infinito se inacessível), prev = anterior no caminho (-1 se nenhum)
pair<vector<float>, vector<int>> dijkstra(int origem) const {
    int n = numVertices();
    const float INF = numeric_limits<float>::infinity();
    vector<float> dist(n, INF);
    vector<int> prev(n, -1);
    if (origem < 0 || origem >= n) return {dist, prev};

    using P = pair<float,int>; // (dist, vert)
    priority_queue<P, vector<P>, greater<P>> pq;

    dist[origem] = 0.0f;
    pq.push({0.0f, origem});

    while (!pq.empty()) {
        auto[d,u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (int v : retornarVizinhos(u)) {
            float w = pesoAresta(u,v);
            if (!ponderado) w = 1.0f; // caso grafo não ponderado
            if (w < 0) continue; // aresta inexistente (defesa)
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }   
        }
    }
    return {dist, prev};
}

};

#endif 
