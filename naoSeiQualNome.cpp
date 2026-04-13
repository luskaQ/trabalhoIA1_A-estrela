#include <iostream>
#include <vector>
#include <map>
using namespace std;

class GrafoIp
{
private:
    int vertices;
    vector<vector<pair<int, double>>> arestas;
    map<int, string> indicesIps;

public:
    GrafoIp(int vertices)
    {
        this->vertices = vertices;

        arestas.resize(vertices);
    }

    void adicionarAresta(int to, int from, double peso)
    {
        arestas[to].push_back(make_pair(from, peso));
        arestas[from].push_back(make_pair(to, peso));
    }

    void popularGrafo()
    {
        for (int i = 0; i < vertices; i++)
        {
            cout << "Ip: " << indicesIps[i] << " associado ao indice " << i << " se liga com quais nos? Par (idx custo) (-1 para ir para o proximo nó)" << endl;
            int idx;
            double custo;
            bool sair;

            do
            {
                cin >> idx >> custo;
                sair = false;
                if ((idx == -1) || idx >= 0 && idx < vertices && custo > 0)
                    {
                        if (idx != -1)
                            adicionarAresta(i, idx, custo);
                        else
                            sair = true;
                    }
                else
                {
                    cout << "Entrada Invalida, tente novamente! \n";
                }
            } while (!sair);
        }
    }

    void popularIps()
    {
        for (int i = 0; i < vertices; i++)
        {
            string ip;
            cin >> ip;
            indicesIps[i] = ip;
        }
    }
    void exibirGrafo()
    {
        for (int i = 0; i < vertices; ++i)
        {
            cout << "Vértice " << i << " está conectado a:\n";
            for (auto const &aresta : arestas[i])
            {
                cout << "  -> " << aresta.first << " (peso: " << aresta.second << ")\n";
            }
        }
    }
};

int main()
{
    GrafoIp grafo = GrafoIp(3);
    grafo.popularIps();
    grafo.popularGrafo();
    grafo.exibirGrafo();
}