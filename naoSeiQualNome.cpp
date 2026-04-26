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
        double g, f;
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

    // O nosso A estrela funciona ao criar ligacoes de nos pais que vao do inicio ate achar o no final, com base no caminho curto encontrado
    //Para construir o caminho mais curto, basta fazer "Uma viagem de ponteiro" do no final e seus pais ate chegar no inicial

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
                 { return indicesIps[a].f < indicesIps[b].f; }); // ao inves de comparar os valores da lista em si, ele os utiliza para comparar em indicesIp
            noAtual = lista_abertos.front();
            lista_abertos.erase(lista_abertos.begin()); // removo o no aberto da lista de abertos

            if (noAtual == noFinal) // cheguei no resultado?
                break;

            lista_fechados.push_back(noAtual);  // Ja vou verificar esse no, ele pode ir para a lista de fechados
            indicesIps[noAtual].visited = true; // faco ele ter sido visitado

            for (auto const &[vizinho, peso] : arestas[noAtual]) // iterar sob todas as arestas do no atual, [idx do vizinho, custo], arestas é um vetor de vetor de pares, lembrando
            {
                if (indicesIps[vizinho].visited)
                    continue; // Se iterei um no ja visitado, posso pular

                float g_tentativo = indicesIps[noAtual].g + peso; // somo o quanto eu ja andei para chegar no noAtual (g) com o peso da aresta que estou verificando,
                                                                  // o que resultara no quanto eu vou andar para chegar no vizinho

                bool estaAberto = any_of(lista_abertos.begin(), lista_abertos.end(), [&](int v)
                                         { return v == vizinho; }); // se algo indice da lista de abertos for igual o do vizinho, retorna true, isto é, s eo vizinho estiver aberto

                if (!estaAberto)
                    lista_abertos.push_back(vizinho);          // se ele nao esta aberto, abra ele
                else if (g_tentativo >= indicesIps[vizinho].g) // se ja foi aberto e se o caminho que testei agora vai me fazer andar a mesma quantia ou
                    continue;                                  // mais doque andei no g do vizinho, vou para a proxima iteração
                                                                //essa verificacao é importante para nao sobrescrever um caminho curto ja encontrado em outra iteração

                indicesIps[vizinho].parent = noAtual; // Se nao, o pai do vizinho se torna o noAtual
                indicesIps[vizinho].g = g_tentativo;  // caminho que vou percorrer para chegar no vizinho
                indicesIps[vizinho].f = g_tentativo + calcular_h_de_n(vizinho, noFinal); // minha funcao heuristica recebe o quanto ja andei mais o quanto ainda falta
            }
        }

        if (!indicesIps[noFinal].visited && noAtual != noFinal) //da forma que foi programado, o no final deve construir um caminho ate o inicial a partir dos pais
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