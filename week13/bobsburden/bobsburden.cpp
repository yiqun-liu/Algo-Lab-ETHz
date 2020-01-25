/**
 * Tutorial problem
 * 
 * proof sketch of the observation introduced in the slides
 * 0. def "center": a vertex, from whom paths to 3 apices are disjoint
 * 
 * 1. optimal spanning subgraph has no cycle --> tree
 * 2. Assume there is a optimal solution, from any chosen vertex there is only
 *    one simple path to 3 apices. 3 paths disjoint? done. otherwise, 
 *    two (three must be not optimal) non-disjoint paths must share a common 
 *    leading sub-path(path between two fixed vertex are unique on a tree). 
 *    move the vertex in that direction until they're disjoint. (number of 
 *    edges are limited and loop-free, so this procedure always terminate)
 * 3. now the total weights is the sum of 3 disjoint paths and its own weight. 
 *    Obviously each must be minimized.
 * 
 * tricks: 
 * 1. when running Dijkstra on undirected graph: source and target are interchangable
 * 2. modeling: weighted vertex are usually split into two vertices, with interior
 *    weight
 */

#include <cstdio>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using std::vector;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    boost::no_property, boost::property<boost::edge_weight_t, int> >    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;

void dijkstra_map(vector<int>& dist_map, const weighted_graph &G, int nVerts, int source) 
{
    boost::dijkstra_shortest_paths(G, source,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));
}

void testcase()
{
    int h;
    scanf("%d", &h);

    const int n = h * (h + 1) / 2, nVerts = 2 * n, baseOut = n; // two for each vertex
    weighted_graph G(nVerts);
    weight_map weights = boost::get(boost::edge_weight, G);
    
    vector<int> values;
    values.reserve(n);
    edge_desc e;
    for (int idx = 0, i = 1; i <= h; i++) // layer
    {
        for (int j = 0; j < i; j++)
        {
            int value;
            scanf("%d", &value);
            values.push_back(value);

            e = boost::add_edge(idx, baseOut + idx, G).first; weights[e] = value;

            if (j) // add edges in interval (j-1, j)
            {
                // in-line edges
                e = boost::add_edge(baseOut + idx - 1, idx, G).first; weights[e] = 0;
                e = boost::add_edge(baseOut + idx, idx - 1, G).first; weights[e] = 0;

                // cross-line edges
                if (i != 1)
                {
                    int ancestor = idx - i;
                    e = boost::add_edge(baseOut + ancestor, idx, G).first; weights[e] = 0;
                    e = boost::add_edge(baseOut + idx, ancestor, G).first; weights[e] = 0;
                    e = boost::add_edge(baseOut + ancestor, idx - 1, G).first; weights[e] = 0;
                    e = boost::add_edge(baseOut + idx - 1, ancestor, G).first; weights[e] = 0;
                }
            }

            idx++;
        }
    }

    const int apex1 = 0, apex2 = n - h, apex3 = n - 1; // in vertex
    vector<int> dist_map1(nVerts), dist_map2(nVerts), dist_map3(nVerts);
    dijkstra_map(dist_map1, G, nVerts, apex1);
    dijkstra_map(dist_map2, G, nVerts, apex2);
    dijkstra_map(dist_map3, G, nVerts, apex3);

    int optimalSum = 2000 * 1000; // initialized with maximum total value
    for (int i = 0; i < n; i++) // check all vertex
    {
        // own weight + shortest paths to in-vertex
        int sum = values[i] + dist_map1[i] + dist_map2[i] + dist_map3[i];
        optimalSum = sum < optimalSum ? sum : optimalSum;
    }
    printf("%d\n", optimalSum);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}
