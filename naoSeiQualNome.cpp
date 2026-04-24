#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
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
        ip *parent;
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
            cout << "Ip: " << indicesIps[i].ipEndereco << " associado ao indice " << i << " se liga com quais nos? Par (idx custo) (-1 para ir para o proximo nó)" << endl;
            int idx;
            double custo;
            bool sair;

            do
            {
                cin >> idx >> custo;
                sair = false;
                if ((idx == -1) || (idx >= 0 && idx < vertices && custo > 0))
                {
                    if (idx != -1) {
                        bool ligacaoExiste = any_of(arestas[i].begin(), arestas[i].end(), [&](const pair<int,double>& aresta) {
                            return aresta.first == idx;
                        });
                        if(!ligacaoExiste)
                            adicionarAresta(i, idx, distanciaEuclidiana(indicesIps[i].longitude, indicesIps[i].latitude, indicesIps[idx].longitude, indicesIps[idx].latitude)/VEL_LUZ);
                        else
                            cout << "A ligacao ja existe na rede!\n";
                    }
                    else {
                        sair = true;
                    }
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
            aux.parent = nullptr;
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

    void definirNoInicialFinal() {
        int aux;
        cout << "digite o indice do nó INICIAL\n";
        cin >> aux;
        noInicial = aux;
        cout << "digite o indice do nó FINAL\n";
        cin >> aux;
        noFinal = aux;
    }

    double distanciaEuclidiana(double longi1, double lat1, double longi2, double lat2){
        return hypot(abs(longi1 - longi2), abs(lat1 - lat2));
    }

    double calcular_h_de_n(int idx1, int idx2){
        return distanciaEuclidiana(indicesIps[idx1].longitude, indicesIps[idx1].latitude, indicesIps[idx2].longitude, indicesIps[idx2].latitude);
    }

    void a_estrela()
{
        // Reinicia os nós
        for (int i = 0; i < vertices; i++) {
            indicesIps[i].g       = numeric_limits<float>::max();
            indicesIps[i].f       = numeric_limits<float>::max();
            indicesIps[i].visited = false;
            indicesIps[i].parent  = nullptr;
        }

        int noAtual = noInicial;
        vector<int> lista_abertos;
        vector<int> lista_fechados;

        indicesIps[noInicial].g = 0.0;
        indicesIps[noInicial].f = calcular_h_de_n(noInicial, noFinal);
        lista_abertos.push_back(noInicial);

        while (!lista_abertos.empty() && noAtual != noFinal) {
            // Ordena por f crescente — menor f sai primeiro
            sort(lista_abertos.begin(), lista_abertos.end(), [&](int a, int b) {
                return indicesIps[a].f < indicesIps[b].f;
            });

            // Pega e remove o melhor nó
            noAtual = lista_abertos.front();
            lista_abertos.erase(lista_abertos.begin());

            if (noAtual == noFinal) break;

            lista_fechados.push_back(noAtual);
            indicesIps[noAtual].visited = true;

            // Expande vizinhos
            for (auto const &[vizinho, peso] : arestas[noAtual]) {
                if (indicesIps[vizinho].visited) continue;

                float g_tentativo = indicesIps[noAtual].g + peso;

                bool estaAberto = any_of(lista_abertos.begin(), lista_abertos.end(),
                    [&](int v) { return v == vizinho; });

                if (!estaAberto)
                    lista_abertos.push_back(vizinho);
                else if (g_tentativo >= indicesIps[vizinho].g)
                    continue; // caminho atual não é melhor

                // Atualiza com o melhor caminho encontrado
                indicesIps[vizinho].parent = &indicesIps[noAtual];
                indicesIps[vizinho].g      = g_tentativo;
                indicesIps[vizinho].f      = g_tentativo + calcular_h_de_n(vizinho, noFinal);
            }
        }

        // Exibe resultado
        if (!indicesIps[noFinal].visited && noAtual != noFinal) {
            cout << "Nenhum caminho encontrado!\n";
            return;
        }

        // Reconstrói caminho andando pelos parents
        vector<string> caminho;
        ip *atual = &indicesIps[noFinal];
        while (atual != nullptr) {
            caminho.push_back(atual->ipEndereco);
            atual = atual->parent;
        }
        reverse(caminho.begin(), caminho.end());

        cout << "\nCaminho encontrado:\n";
        for (int i = 0; i < (int)caminho.size(); i++) {
            cout << caminho[i];
            if (i + 1 < (int)caminho.size()) cout << " -> ";
        }
        cout << "\nCusto total: " << indicesIps[noFinal].g << " segundos\n";
    }
};

int main()
{
    GrafoIp grafo = GrafoIp(3);
    grafo.popularIps();
    grafo.definirNoInicialFinal(); // <- adicionar isso
    grafo.popularGrafo();
    grafo.exibirGrafo();
    grafo.a_estrela();             // <- e isso
}