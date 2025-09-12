#include <iostream>
#include <memory>
#include <fstream>
#include "LeitorGrafos.h"
#include "Grafos.h"

using namespace std;

int main(int argc, char** argv) {
    cout << "Projeto Grafos - carregamento interativo" << endl;

    // Perguntar ao usuário se o grafo é direcionado
    char c;
    cout << "O grafo e direcionado? (s/n): ";
    cin >> c;
    bool dirigido = (c == 's' || c == 'S');

    cout << "O grafo e ponderado? (s/n): ";
    cin >> c;
    bool ponderado = (c == 's' || c == 'S');

    cout << "Escolha representacao: 1 - Matriz | 2 - Lista : ";
    int escolha = 1;
    cin >> escolha;
    if (escolha != 1 && escolha != 2) {
        cout << "Escolha invalida. Usando Matriz por padrao.";
        escolha = 1;
    }

    string filename = "teste.txt";

    int V=0, A=0;
    vector<tuple<int,int,float>> arestas;
    if (!LeitorGrafos::lerCabecalhoEArestas(filename, V, A, arestas, ponderado)) {
        cerr << "Erro ao ler arquivo ou formato invalido (verifique se o arquivo tem pesos se voce marcou ponderado).";
        return 1;
    }

    unique_ptr<Grafos> grafo;
    if (escolha == 1) grafo = make_unique<GrafoMatriz>(dirigido, ponderado);
    else grafo = make_unique<GrafoLista>(dirigido, ponderado);

    for (int i = 0; i < V; ++i) grafo->inserirVertice(to_string(i));

    for (auto &t : arestas) {
        int ao, ad; float ap;
        tie(ao, ad, ap) = t;
        if (!grafo->inserirAresta(ao, ad, ap)) {
            cout << "Aviso: falha ao inserir aresta " << ao << " -> " << ad << " (talvez duplicata).";
        }
    }

    cout << "Grafo carregado com sucesso:";
    grafo->imprimeGrafo();

    if (grafo->numVertices() > 0) {
        cout << "BFS a partir do vertice 0:";
        auto bfs_ordem = grafo->bfs(0);
        for (int v : bfs_ordem) cout << v << " ";
        cout << endl;

        cout << "DFS a partir do vertice 0:";
        auto dfs_ordem = grafo->dfs(0);
        for (int v : dfs_ordem) cout << v << " ";
        cout << endl;

        cout << "Dijkstra a partir do vertice 0 :" << endl;
        auto pr = grafo->dijkstra(0);
        auto &dist = pr.first;
        auto &prev = pr.second;
        for (int i = 0; i < grafo->numVertices(); ++i) {
        cout << grafo->nomeVertice(i) << " (" << grafo->labelVertice(i) << ") : ";
        if (dist[i] == numeric_limits<float>::infinity()) cout << "INF";
        else cout << dist[i];
        cout << endl;
        }
    }

    return 0;
}