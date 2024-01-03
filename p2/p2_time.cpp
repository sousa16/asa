#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <limits>
#include <cstdio>
#include <cmath>
#include <chrono>

using namespace std;

const int NINF = -1e9;

// Depth-First Search for the original graph
void dfs(int start, const vector<vector<int>> &graph, vector<bool> &visited, vector<int> &node_order)
{
    stack<int> s;
    s.push(start);

    while (!s.empty())
    {
        int n = s.top();

        // Check if all neighbors are visited
        bool all_neighbors_visited = true;
        for (int neighbor : graph[n])
        {
            if (!visited[neighbor])
            {
                all_neighbors_visited = false;
                s.push(neighbor);
                visited[neighbor] = true; // Mark the neighbor as visited immediately
                break;
            }
        }

        // If all neighbors are visited, pop the current node
        if (all_neighbors_visited)
        {
            s.pop();
            node_order.push_back(n);
        }
    }
}

// Depth-First Search for the reversed graph to find Strongly Connected Components (SCCs)
void reverseDFS(int start, vector<int> &currentSCC, const vector<vector<int>> &reverseGraph, vector<bool> &vis)
{
    stack<int> s;
    s.push(start);

    while (!s.empty())
    {
        int n = s.top();
        s.pop();

        if (vis[n])
            continue; // If node is already visited

        int len = reverseGraph[n].size();
        currentSCC.push_back(n);
        vis[n] = true;

        for (int i = 0; i < len; i++)
        {
            int neighbor = reverseGraph[n][i];
            if (!vis[neighbor])
            {
                s.push(neighbor);
            }
        }
    }
}

// Kosaraju's Algorithm to find Strongly Connected Components (SCCs)
vector<vector<int>> kosarajuSCC(int n, const vector<vector<int>> &graph, const vector<vector<int>> &reverseGraph, vector<bool> &vis, vector<int> &node_order)
{
    vector<vector<int>> sccList;

    // First DFS to get the order of nodes for the second pass
    for (int i = 1; i <= n; i++)
    {
        if (vis[i] == false)
        {
            dfs(i, graph, vis, node_order);
        }
    }

    // Reset visited array for the second pass
    for (int i = 1; i <= n; i++)
    {
        vis[i] = false;
    }

    // Second DFS to find SCCs based on the reversed order
    for (int i = node_order.size() - 1; i >= 0; i--)
    {
        if (vis[node_order[i]] == false)
        {
            vector<int> currentSCC;
            reverseDFS(node_order[i], currentSCC, reverseGraph, vis);
            sccList.push_back(currentSCC);
        }
    }

    return sccList;
}

// Merge SCCs and create a new graph
vector<vector<int>> mergeSCC(int n, int m, const vector<vector<int>> &graph, const vector<pair<int, int>> &relationships, const vector<vector<int>> &sccList)
{
    unordered_map<int, int> minNodeMap;

    // Determine the minimum node in each SCC
    for (const auto &scc : sccList)
    {
        if (scc.size() > 1)
        {
            int minValue = *min_element(scc.begin(), scc.end());

            for (const int &node : scc)
            {
                minNodeMap[node] = minValue;
            }
        }
        else
        {
            minNodeMap[scc[0]] = scc[0];
        }
    }

    // Create a new graph by merging nodes in SCCs
    vector<vector<int>> mergedGraph(minNodeMap.size() + 1);

    for (const auto &relationship : relationships)
    {
        int a = minNodeMap[relationship.first];
        int b = minNodeMap[relationship.second];

        if (a != b)
        {
            mergedGraph[a].push_back(b);
        }
    }

    return mergedGraph;
}

// Utility function for Topological Sort
void topologicalSortUtil(int v, vector<bool> &visited, const vector<vector<int>> &graph, vector<int> &order)
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

// Topological Sort of a graph
vector<int> topologicalSort(const vector<vector<int>> &graph, int V)
{
    vector<bool> visited(V + 1, false); // Adjust for 1-based indexing
    vector<int> order;
    for (int i = 1; i <= V; ++i) // Start from 1
    {
        if (!visited[i])
        {
            topologicalSortUtil(i, visited, graph, order);
        }
    }
    reverse(order.begin(), order.end());
    return order;
}

// Longest path in a Directed Acyclic Graph (DAG)
int longestPath(int src, const vector<vector<int>> &graph, int n, const vector<int> &order)
{
    vector<int> dist(n + 1, NINF);
    dist[src] = 0;

    // Update the distance for each node based on topological order
    for (int i = 0; i < n; ++i)
    {
        int u = order[i];
        if (dist[u] != NINF)
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

// Find the longest path in the merged SCC graph
int findLongestPath(const vector<vector<int>> &graph, int n)
{
    vector<int> order = topologicalSort(graph, n);

    int maxPathLength = NINF;

    // Iterate over all nodes to find the longest path
    for (int i = 1; i <= n; ++i)
    {
        int pathLength = longestPath(i, graph, n, order);
        if (pathLength > maxPathLength)
            maxPathLength = pathLength;
    }

    return maxPathLength;
}

int main()
{
    // Measure the runtime of the algorithm
    auto start = std::chrono::high_resolution_clock::now();

    // Parse input and build the directed graph representation
    int n, m;

    scanf("%d", &n);
    scanf("%d", &m);

    vector<vector<int>> graph(n + 1); // Vertices are numbered from 1 to n
    vector<vector<int>> reverseGraph(n + 1);
    vector<pair<int, int>> relationships;
    vector<bool> vis(n + 1, false);
    vector<int> node_order;

    relationships.reserve(m);

    // Input validation and graph construction
    if (n >= 2 && m >= 0)
    {
        for (int i = 0; i < m; ++i)
        {
            int a, b;

            scanf("%d", &a);
            scanf("%d", &b);

            if (a > 0 && a <= n && b > 0 && b <= n)
            {
                graph[a].push_back(b);
                reverseGraph[b].push_back(a);
                relationships.emplace_back(a, b);
            }
        }
    }

    // Find Strongly Connected Components (SCCs)
    vector<vector<int>> sccList = kosarajuSCC(n, graph, reverseGraph, vis, node_order);

    // Merge SCCs to create a new graph
    vector<vector<int>> mergedGraph = mergeSCC(n, m, graph, relationships, sccList);

    // Find and print the longest path in the merged SCC graph
    int result = findLongestPath(mergedGraph, n);

    cout << result << endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Runtime: " << duration.count() << " ms" << std::endl;
}
