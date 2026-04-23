#include <iostream>
#include <vector>
#include <map>
#include <cmath>
using namespace std;
constexpr double VEL_LUZ = 299792458.0;
class GrafoIp
{
private:

    struct ip
    {
        string ip;
        double longitude;
        double latitude;
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
            cout << "Ip: " << indicesIps[i].ip << " associado ao indice " << i << " se liga com quais nos? Par (idx custo) (-1 para ir para o proximo nó)" << endl;
            int idx;
            double custo;
            bool sair;

            do
            {
                cin >> idx >> custo;
                sair = false;
                if ((idx == -1) || (idx >= 0 && idx < vertices && custo > 0))
                {
                    if (idx != -1)
                        adicionarAresta(i, idx, distanciaEuclidiana(indicesIps[i].longitude, indicesIps[i].latitude, indicesIps[idx].longitude, indicesIps[idx].latitude)/VEL_LUZ);
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
            string ip_aux;
            double longitude;
            double latitude;
            cin >> ip_aux >> longitude >> latitude;
            ip aux;
            aux.ip = ip_aux;
            aux.latitude = latitude;
            aux.longitude = longitude;
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

    double distanciaEuclidiana(double longi1, double lat1, double longi2, double lat2){
        return hypot(longi1 - longi2, lat1 - lat2);
    }

    double calcular_h_de_n(int idx1, int idx2){
        return distanciaEuclidiana(indicesIps[idx1].longitude, indicesIps[idx1].latitude, indicesIps[idx2].longitude, indicesIps[idx2].latitude);
    }

    void a_estrela()
    {
        vector<vector<pair<int, double>>> lista_abertos;
        vector<vector<pair<int, double>>> lista_fechados = this->arestas;
    }
};

int main()
{
    GrafoIp grafo = GrafoIp(3);
    grafo.popularIps();
    grafo.popularGrafo();
    grafo.exibirGrafo();
}