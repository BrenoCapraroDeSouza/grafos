#include <iostream>
#include <memory>
#include <fstream>
#include "LeitorGrafos.h"
#include "Grafos.h"
//re221sa
using namespace std;

int main(int argc, char** argv) {
    cout << "Projeto Grafos - carregamento interativo" << endl;

    cout << "Escolha representacao: 1 - Matriz | 2 - Lista : ";
    int tipoRepresentacao = 1;
    cin >> tipoRepresentacao;
    if (tipoRepresentacao != 1 && tipoRepresentacao != 2) {
        cout << "Escolha invalida. Usando Matriz por padrao.";
        tipoRepresentacao = 1;
    }

    string nomeArquivo = "slides.txt";

    int quantidadeVertices = 0, quantidadeArestas = 0, grafoDirecionado = 0, grafoPonderado = 0;
    vector<tuple<int, int, float>> listaArestas;
    if (!LeitorGrafos::lerCabecalhoEArestas(nomeArquivo, quantidadeVertices, quantidadeArestas, grafoDirecionado, grafoPonderado, listaArestas, true)) {
        cerr << "Erro ao ler arquivo ou formato invalido.";
        return 1;
    }

    unique_ptr<Grafos> grafo;
    if (tipoRepresentacao == 1) grafo = make_unique<GrafoMatriz>(grafoDirecionado, grafoPonderado);
    else grafo = make_unique<GrafoLista>(grafoDirecionado, grafoPonderado);

    for (int indiceVertice = 0; indiceVertice < quantidadeVertices; ++indiceVertice) grafo->inserirVertice(to_string(indiceVertice));

    for (auto &aresta : listaArestas) {
        int indiceOrigem, indiceDestino; float pesoAresta;
        tie(indiceOrigem, indiceDestino, pesoAresta) = aresta;
        if (!grafo->inserirAresta(indiceOrigem, indiceDestino, pesoAresta)) {
            cout << "Aviso: falha ao inserir aresta " << indiceOrigem << " -> " << indiceDestino << " (talvez duplicata).";
        }
    }

    cout << "Grafo carregado com sucesso:";
    cout << "\n";
    grafo->imprimeGrafo();
    cout << "\n";

    if (grafo->numVertices() > 0) {
        cout << "BFS a partir do vertice :" << grafo->nomeVertice(0) << endl;
        auto ordemBFS = grafo->bfs(0);
        for (int indiceVertice : ordemBFS) cout << grafo->nomeVertice(indiceVertice) << "-> ";
        cout << "\n\n";

        cout << "DFS a partir do vertice :" << grafo->nomeVertice(0) << endl;
        auto ordemDFS = grafo->dfs(0);
        for (int indiceVertice : ordemDFS) cout << grafo->nomeVertice(indiceVertice) << "-> ";
        cout << "\n\n";

        cout << "Dijkstra a partir do vertice:" << grafo->nomeVertice(4) << endl;
        auto resultadoDijkstra = grafo->dijkstra(4);
        auto &distancias = resultadoDijkstra.first;
        auto &anteriores = resultadoDijkstra.second;
        for (int indiceVertice = 0; indiceVertice < grafo->numVertices(); ++indiceVertice) {
            cout << grafo->nomeVertice(indiceVertice) << " (" << grafo->rotuloVertice(indiceVertice) << ") : ";
            if (distancias[indiceVertice] == numeric_limits<float>::infinity()) cout << "INF";
            else cout << distancias[indiceVertice];
            cout << "\n\n";
        }
    }

    return 0;
}