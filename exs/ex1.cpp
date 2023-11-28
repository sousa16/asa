#include <iostream>
#include <list>
#include <vector>

unsigned int _V, _E; // V -> Vertices, E -> Edges

std::list<int> *_fwrAdjList; // array de listas que guardam inteiros, out degree
std::list<int> *_revAdjList; // in degree

void readGraph()
{
    // scanf("%d, %d", &_V, &_E)         C code
    std::cin >> _V; // scan aos vertices e edges
    std::cin.ignore();
    std::cin >> _E;

    _fwrAdjList = new std::list<int>[_V]; // new array vazio com _V size
    _revAdjList = new std::list<int>[_V];

    for (size_t i = 0; i < _E; i++)
    {
        int u, v;

        // scanf("%d, %d", &u, &v);
        std::cin >> u >> v;

        _fwrAdjList[u - 1].push_front(v - 1); // colocar no inicio da lista u-1 do array de listas a ligação com v
        _revAdjList[v - 1].push_front(u - 1); // colocar no inicio da lista v-1 do arr de listas a ligacao rev com u
    }
}

void computeDegrees(std::list<int> *_adjList)
{
    std::vector<int> _hist;
    _hist.resize(_V); // hist do tamanho de _V
    for (size_t i = 0; i < _V; i++)
        _hist[i] = 0; // tudo em hist = 0

    for (size_t i = 0; i < _V; i++)
        _hist[_adjList[i].size()]++; // adiciona 1 à coluna do numero de vertices adjacentes

    for (size_t i = 0; i < _V; i++)
        std::cout << _hist[i] << std::endl; // print todos os hist
}

void commonFriends()
{
    for (size_t i = 0; i < _V; i++)
    {
        for (size_t j = 0; j < _V; j++)
        {
            int c = 0;

            for (std::list<int>::iterator it_i = _fwrAdjList[i].begin(); it_i != _fwrAdjList[i].end(); it_i++)
            {
                for (std::list<int>::iterator it_j = _fwrAdjList[j].begin(); it_i != _fwrAdjList[j].end(); it_j++)
                {
                    if ((*it_i) == (*it_j))
                        c++;
                }
            }

            std::cout << c << " ";
        }
        std::cout << std::endl;
    }
}
