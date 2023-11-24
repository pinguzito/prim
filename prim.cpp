#include <iostream>
#include <vector>
#define N 9
using namespace std;

class aresta{
public:
    int v1, v2, peso;
};

class vertice{
public:
    int valor, chave, pai;
};

class grafo{
public:
    vector<vertice> vertices;
    vector<aresta> arestas;
};

grafo criaGrafo(int m[][N]){
    grafo G;
    vertice V;
    aresta A;
    for(int i = 0; i < N; i++){
        V.valor = i + 1;
        for(int j = 0; j < N; j++)
        {
            if(m[i][j] != 0)
            {
                A.v1 = i + 1;
                A.v2 = j + 1;
                A.peso = m[i][j];
                G.arestas.push_back(A);
            }
        }
        G.vertices.push_back(V);
    }
    return G;
}

vector<vertice> verticesAdj(grafo G, vertice V){
    vector<vertice> Adj;
    for(int i = 0; i < G.arestas.size(); i++){
        if(G.arestas.at(i).v1 == V.valor || G.arestas.at(i).v2 == V.valor)
        {
            int achou = 0;
            for(int j = 0; j < Adj.size(); j++)
            {
                if(Adj.at(j).valor == V.valor)
                {
                    achou = 1;
                }
            }

            if(achou == 0)
            {
                if(G.arestas.at(i).v1 == V.valor)
                {
                    Adj.push_back(G.vertices.at(G.arestas.at(i).v2 - 1));
                }
                else
                {
                    Adj.push_back(G.vertices.at(G.arestas.at(i).v1 - 1));
                }
            }
        }
    }
    return Adj;
}

int posMin(vector<vertice> Q){
    vertice aux;
    int min;
    aux.chave = 999999;
    for(int i = 0; i < Q.size(); i++){
        if(Q.at(i).chave < aux.chave)
        {
            aux = Q.at(i);
            min = i;
        }
    }
    return min;
}

aresta arestaUV(vector<aresta> A, vertice U, vertice V){
    aresta UV;
    for(int i = 0; i < A.size(); i++){
        if((A.at(i).v1 == U.valor && A.at(i).v2 == V.valor) || (A.at(i).v2 == U.valor && A.at(i).v1 == V.valor))
        {
            UV = A.at(i);
            break;
        }
    }
    return UV;
}

void printAGM(grafo G, int pos){
    vector<aresta> ArvGenMin;

    vertice aux;
    aux = G.vertices.at(0);
    G.vertices.at(0) = G.vertices.at(pos);
    G.vertices.at(pos) = aux;

    for(int i = 0; i < G.vertices.size(); i++){
        vector<vertice> Adj = verticesAdj(G, G.vertices.at(i));
        for(int j = 0; j < Adj.size(); j++)
        {
            if(Adj.at(j).pai == G.vertices.at(i).valor)
            {
                // cout << "Adicionada: ";
                // cout << arestaUV(G.arestas, G.vertices.at(i), Adj.at(j)).v1 << " " << arestaUV(G.arestas, G.vertices.at(i), Adj.at(j)).v2 << " " << arestaUV(G.arestas, G.vertices.at(i), Adj.at(j)).peso << "\n";
                ArvGenMin.push_back(arestaUV(G.arestas, G.vertices.at(i), Adj.at(j)));
            }
            /*else
            {
                cout << "Rejeitada: ";
                cout << arestaUV(G.arestas, G.vertices.at(i), Adj.at(j)).v1 << " " << arestaUV(G.arestas, G.vertices.at(i), Adj.at(j)).v2 << " " << arestaUV(G.arestas, G.vertices.at(i), Adj.at(j)).peso << "\n";
                cout << "Vadj valor: " << Adj.at(j).valor << " | Vatual valor: " << G.vertices.at(i).valor << "\n";
                cout << "Vadj pai: " << Adj.at(j).pai << "\n";
            }*/
        }
    }

    int pesoM = 0;
    cout << "Arvore Geradora Minima: ";
    for(const aresta &a : ArvGenMin){
        cout << "{" << a.v1 << ", " << a.v2 << "} ";
        pesoM = pesoM + a.peso;
    }
    cout << "\nPeso minimo: " << pesoM << "\n";
}

void prim(grafo G, int pos){
    for(int i = 0; i < G.vertices.size(); i++){
        G.vertices.at(i).chave = 999999;
        G.vertices.at(i).pai = -1;
    }
    G.vertices.at(pos).chave = 0;
    G.vertices.at(pos).pai = -1;

    vector<vertice> Q;
    for(int i = 0; i < G.vertices.size(); i++){
        Q.push_back(G.vertices.at(i));
    }

    while (!Q.empty()){
        int min = posMin(Q);
        vertice U = Q.at(min);
        Q.erase(Q.begin() + min);

        vector<vertice> Adj = verticesAdj(G, U);
        for(int i = 0; i < Adj.size(); i++)
        {
            for(int j = 0; j < Q.size(); j++)
            {
                aresta aUV = arestaUV(G.arestas, U, Adj.at(i));
                if(Q.at(j).valor == Adj.at(i).valor && aUV.peso < Adj.at(i).chave)
                {
                    for(int j = 0; j < G.vertices.size(); j++)
                    {
                        if(G.vertices.at(j).valor == Adj.at(i).valor)
                        {
                            G.vertices.at(j).pai = U.valor;
                            G.vertices.at(j).chave = aUV.peso;
                            break;
                        }
                    }
                    Q.at(j).pai = U.valor;
                    Q.at(j).chave = aUV.peso;
                }
            }
        }
    }

    /*for(int i = 0; i < G.vertices.size(); i++){
         cout << "V: " << G.vertices.at(i).valor << " | Chave: " << G.vertices.at(i).chave << " | Pai: " << G.vertices.at(i).chave << "\n";
    }*/
    printAGM(G, pos);
}

int matriz[][N] = {{0, 4, 0, 0, 0, 0, 0, 0, 8},
                   {0, 0, 8, 0, 0, 0, 0, 0, 11},
                   {0, 0, 0, 7, 0, 4, 0, 2, 0},
                   {0, 0, 0, 0, 9, 14, 0, 0, 0},
                   {0, 0, 0, 0, 0, 10, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 2, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 6, 1},
                   {0, 0, 0, 0, 0, 0, 0, 0, 7},
                   {0, 0, 0, 0, 0, 0, 0, 0, 0}};

/*
int matriz[][N]={{0,1,0,4,0,0,0},       12 23 34 36 38 45 67 79
                 {0,0,2,6,4,0,0},
                 {0,0,0,0,5,6,0},
                 {0,0,0,0,3,0,4},
                 {0,0,0,0,0,8,7},
                 {0,0,0,0,0,0,3},
                 {0,0,0,0,0,0,0}};
*/

int main(){

    grafo G = criaGrafo(matriz);
    int pos = 0;
    prim(G, pos);

    return 0;
}
