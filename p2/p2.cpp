#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <limits>

using namespace std;

vector<vector<int>> graph;
vector<vector<int>> reverseGraph;
vector<pair<int, int>> relationships;
vector<bool> vis;                // Store node visit status for DFS
vector<int> node_order;          // Store nodes in order of their finish time in DFS
vector<vector<int>> sccList;     // Store SCCs here
vector<vector<int>> mergedGraph; // Merge SCCs here

const int INF = numeric_limits<int>::min();

// Travel the whole graph and put nodes in a node_order according to their finish time
void dfs(int n, const vector<vector<int>> &graph, vector<bool> &visited, vector<int> &node_order)
{
    if (visited[n])
        return; // If node is already visited

    visited[n] = true;

    for (int neighbor : graph[n])
    {
        dfs(neighbor, graph, visited, node_order);
    }

    node_order.push_back(n);
}

// Traverse the reverseGraph in order to find SCCs
void reverseDFS(int n, vector<int> &currentSCC)
{
    if (vis[n] == true)
        return; // If node is already visited

    int len = reverseGraph[n].size();
    currentSCC.push_back(n);
    vis[n] = true;

    for (int i = 0; i < len; i++)
    {
        reverseDFS(reverseGraph[n][i], currentSCC);
    }
}

// N nodes, find every SCC (single nodes are SCCs)
vector<vector<int>> kosarajuSCC(int n)
{
    for (int i = 1; i <= n; i++)
    {
        if (vis[i] == false)
        {
            dfs(i, graph, vis, node_order);
        }
    }

    for (int i = 1; i <= n; i++)
    {
        vis[i] = false;
    }

    for (int i = node_order.size() - 1; i >= 0; i--)
    {
        if (vis[node_order[i]] == false)
        {
            vector<int> currentSCC;
            reverseDFS(node_order[i], currentSCC);
            sccList.push_back(currentSCC);
        }
    }

    return sccList;
}

// Function to merge SCCs into a new graph
vector<vector<int>> mergeSCC(int n, int m, const vector<vector<int>> &graph, const vector<vector<int>> &sccList)
{
    // Map each node to its minimum value within the SCC
    unordered_map<int, int> minNodeMap;

    for (const auto &scc : sccList)
    {
        // Check if the size of the SCC is greater than 1
        if (scc.size() > 1)
        {
            // Find the minimum value in the SCC
            int minValue = scc[0];

            for (size_t i = 1; i < scc.size(); ++i)
            {
                // Update the minimum value if the current element is smaller
                if (scc[i] < minValue)
                {
                    minValue = scc[i];
                }
            }

            // Map each node to the minimum value within the SCC
            for (const int &node : scc)
            {
                minNodeMap[node] = minValue;
            }
        }
        else
        {
            // If SCC has size 1, map the single node to itself
            minNodeMap[scc[0]] = scc[0];
        }
    }

    // Replace nodes in relationships using the mapping
    for (auto &relationship : relationships)
    {
        relationship.first = minNodeMap[relationship.first];
        relationship.second = minNodeMap[relationship.second];
    }

    // Build mergedGraph using the updated relationships
    vector<vector<int>> mergedGraph(minNodeMap.size() + 1); // Vertices are numbered from 1 to minNodeMap.size()

    for (const auto &relationship : relationships)
    {
        int a = relationship.first;
        int b = relationship.second;

        // Check if a and b are equal
        if (a != b)
        {
            mergedGraph[a].push_back(b);
        }
    }

    return mergedGraph;
}

void topologicalSortUtil(int v, vector<bool> &visited, vector<vector<int>> &graph, vector<int> &order)
{
    visited[v] = true;
    for (int u : graph[v])
    {
        if (!visited[u])
        {
            topologicalSortUtil(u, visited, graph, order);
        }
    }
    order.push_back(v);
}

vector<int> topologicalSort(vector<vector<int>> &graph, int V)
{
    vector<bool> visited(V, false);
    vector<int> order;
    for (int i = 0; i < V; ++i)
    {
        if (!visited[i])
        {
            topologicalSortUtil(i, visited, graph, order);
        }
    }
    reverse(order.begin(), order.end());
    return order;
}

int longestPath(int src, vector<vector<int>> &graph, vector<int> &order)
{
    int V = graph.size();
    vector<int> dist(V, INF);
    dist[src] = 0;

    for (int i = 0; i < V; ++i)
    {
        int u = order[i];
        if (dist[u] != INF)
        {
            for (int v : graph[u])
            {
                if (dist[u] + 1 > dist[v])
                {
                    dist[v] = dist[u] + 1;
                }
            }
        }
    }

    return *max_element(dist.begin(), dist.end());
}

int findLongestPath(vector<vector<int>> &graph)
{
    int V = graph.size();
    vector<int> order = topologicalSort(graph, V);

    int maxPathLength = INF;

    for (int i = 0; i < V; ++i)
    {
        int pathLength = longestPath(i, graph, order);
        maxPathLength = max(maxPathLength, pathLength);
    }

    return maxPathLength;
}

int main()
{
    // Parse input and build the directed graph representation
    int n, m;

    scanf("%d", &n);
    scanf("%d", &m);

    graph.resize(n + 1); // Vertices are numbered from 1 to n
    reverseGraph.resize(n + 1);
    vis.resize(n + 1, false);

    // Input relationships (between a and b) and build the graph
    if (n >= 2 && m >= 0)
    {
        for (int i = 0; i < m; ++i)
        {
            int a, b;

            scanf("%d", &a);
            scanf("%d", &b);

            // Check if the input is valid
            if (a > 0 && a <= n && b > 0 && b <= n)
            {
                // Build the graph and reverseGraph directly
                graph[a].push_back(b);
                reverseGraph[b].push_back(a);
                relationships.push_back({a, b});
            }
        }
    }

    // Get SCCs in sccList
    sccList = kosarajuSCC(n);

    // Merge SCCs
    vector<vector<int>> mergedGraph = mergeSCC(n, m, graph, sccList);

    int result = findLongestPath(mergedGraph);

    cout << result << endl;
}
