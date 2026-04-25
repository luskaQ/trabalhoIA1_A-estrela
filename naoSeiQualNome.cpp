#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <algorithm>
using namespace std;
constexpr double VEL_LUZ = 299792458.0;
class GrafoIp
{
private:
    struct ip
    {
        string ipEndereco;
        double longitude;
        double latitude;
        float g, f;
        bool visited;
        int parent;
    };

    int vertices;
    vector<vector<pair<int, double>>> arestas;
    map<int, ip> indicesIps;
    int noInicial;
    int noFinal;

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
            cout << "Ip: " << indicesIps[i].ipEndereco << " (Indice " << i << ")" << endl;
            cout << "Conectar com qual indice? (-1 para proximo no): ";
            int idx;

            while (cin >> idx && idx != -1)
            {
                if (idx >= 0 && idx < vertices && idx != i)
                {
                    bool ligacaoExiste = any_of(arestas[i].begin(), arestas[i].end(), [&](const pair<int, double> &aresta)
                                                { return aresta.first == idx; });

                    if (!ligacaoExiste)
                    {
                        double d = distanciaEuclidiana(indicesIps[i].longitude, indicesIps[i].latitude,
                                                       indicesIps[idx].longitude, indicesIps[idx].latitude);
                        adicionarAresta(i, idx, d / VEL_LUZ);
                        cout << "Ligacao criada com sucesso!\n";
                    }
                    else
                    {
                        cout << "A ligacao ja existe!\n";
                    }
                }
                else
                {
                    cout << "Indice invalido!\n";
                }
                cout << "Proximo indice para " << indicesIps[i].ipEndereco << " (-1 para sair): ";
            }
        }
    }

    void popularIps()
    {
        for (int i = 0; i < vertices; i++)
        {
            string ipEndereco;
            double longitude;
            double latitude;
            cin >> ipEndereco >> longitude >> latitude;
            ip aux;
            aux.ipEndereco = ipEndereco;
            aux.latitude = latitude;
            aux.longitude = longitude;
            aux.f = numeric_limits<float>::max();
            aux.g = numeric_limits<float>::max();
            aux.parent = -1;
            aux.visited = false;
            indicesIps[i] = aux;
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

    void definirNoInicialFinal()
    {
        int aux;
        cout << "digite o indice do nó INICIAL\n";
        cin >> aux;
        noInicial = aux;
        cout << "digite o indice do nó FINAL\n";
        cin >> aux;
        noFinal = aux;
    }

    double distanciaEuclidiana(double longi1, double lat1, double longi2, double lat2)
    {
        return hypot(abs(longi1 - longi2), abs(lat1 - lat2));
    }

    double calcular_h_de_n(int idx1, int idx2)
    {
        return distanciaEuclidiana(indicesIps[idx1].longitude, indicesIps[idx1].latitude,
                                   indicesIps[idx2].longitude, indicesIps[idx2].latitude) /
               VEL_LUZ;
    }

    void a_estrela()
    {
        for (int i = 0; i < vertices; i++)
        {
            indicesIps[i].g = numeric_limits<float>::max();
            indicesIps[i].f = numeric_limits<float>::max();
            indicesIps[i].visited = false;
            indicesIps[i].parent = -1;
        }

        int noAtual = noInicial;
        vector<int> lista_abertos;
        vector<int> lista_fechados;

        indicesIps[noInicial].g = 0.0;
        indicesIps[noInicial].f = calcular_h_de_n(noInicial, noFinal);
        lista_abertos.push_back(noInicial);

        while (!lista_abertos.empty() && noAtual != noFinal)
        {
            sort(lista_abertos.begin(), lista_abertos.end(), [&](int a, int b)
                 { return indicesIps[a].f < indicesIps[b].f; });
            noAtual = lista_abertos.front();
            lista_abertos.erase(lista_abertos.begin());

            if (noAtual == noFinal)
                break;

            lista_fechados.push_back(noAtual);
            indicesIps[noAtual].visited = true;

            for (auto const &[vizinho, peso] : arestas[noAtual])
            {
                if (indicesIps[vizinho].visited)
                    continue;

                float g_tentativo = indicesIps[noAtual].g + peso;

                bool estaAberto = any_of(lista_abertos.begin(), lista_abertos.end(), [&](int v)
                                         { return v == vizinho; });

                if (!estaAberto)
                    lista_abertos.push_back(vizinho);
                else if (g_tentativo >= indicesIps[vizinho].g)
                    continue;

                indicesIps[vizinho].parent = noAtual;
                indicesIps[vizinho].g = g_tentativo;
                indicesIps[vizinho].f = g_tentativo + calcular_h_de_n(vizinho, noFinal);
            }
        }

        if (!indicesIps[noFinal].visited && noAtual != noFinal)
        {
            cout << "Nenhum caminho encontrado!\n";
            return;
        }

        vector<string> caminho;
        int idxAtual = noFinal;
        while (idxAtual != -1)
        {
            caminho.push_back(indicesIps[idxAtual].ipEndereco);
            idxAtual = indicesIps[idxAtual].parent;
            if (idxAtual == noInicial)
            { 
                caminho.push_back(indicesIps[idxAtual].ipEndereco);
                break;
            }
        }
        reverse(caminho.begin(), caminho.end());

        cout << "\nCaminho encontrado:\n";
        for (int i = 0; i < (int)caminho.size(); i++)
        {
            cout << caminho[i];
            if (i + 1 < (int)caminho.size())
                cout << " -> ";
        }
        cout << "\nCusto total: " << indicesIps[noFinal].g << " segundos\n";
    }
};

int main()
{
    int n;
    cin >> n;
    GrafoIp grafo = GrafoIp(n);
    grafo.popularIps();
    grafo.definirNoInicialFinal(); 
    grafo.popularGrafo();
    grafo.exibirGrafo();
    grafo.a_estrela();
}