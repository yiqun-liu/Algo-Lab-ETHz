#include <cstdio>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int mstSum(const weighted_graph &G, const weight_map& weights)
{
    std::vector<edge_desc> mst;

    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    int weightSum = 0;
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it)
        weightSum += weights[ *it ];
    
    return weightSum;
}

int dijkDistMax(const weighted_graph &G, int nVerts, int source)
{
    std::vector<int> dist_map(nVerts);

    boost::dijkstra_shortest_paths(G, source,
        boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));

    int maxDist = 0;
    for (int i = 0; i < nVerts; i++)
        if (dist_map[i] > maxDist)
            maxDist = dist_map[i];
    
    return maxDist;
}

void testcase()
{
    int nVerts, nEdges;
    scanf("%d%d", &nVerts, &nEdges);

    weighted_graph G(nVerts);
    weight_map weights = boost::get(boost::edge_weight, G);

    for (int i = 0; i < nEdges; i++)
    {
        edge_desc e;
        int from, to, weight;
        scanf("%d%d%d", &from, &to, &weight);

        e = boost::add_edge(from, to, G).first;
        weights[e] = weight;
    }

    printf("%d %d\n", mstSum(G, weights), dijkDistMax(G, nVerts, 0));
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}
