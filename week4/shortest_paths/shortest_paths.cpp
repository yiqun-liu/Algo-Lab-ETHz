#include <cstdio>
#include <vector>
#include <limits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;

const int MAX_N = 500000;
std::vector<int> dist_map[MAX_N];

// with memoization
int dijkstra_dist(int n, const weighted_graph &G, int s, int t) 
{
    if (dist_map[s].size())
        return dist_map[s][t];
    
    dist_map[s].resize(n);

    boost::dijkstra_shortest_paths(G, s,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map[s].begin(), boost::get(boost::vertex_index, G))));

    return dist_map[s][t];
}

int main()
{
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);

    // Init graph
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);

    // Load vertex positions
    for (int i = 0; i < n; i++) // Not relevant, ignored here
    {
        int lon, lat;
        scanf("%d%d", &lon, &lat);
    }
    
    // Load edges
    for (int i = 0; i < m; i++)
    {
        int source, target, weight;
        scanf("%d%d%d", &source, &target, &weight);
        edge_desc e = boost::add_edge(source, target, G).first;
        weights[e] = weight;
    }

    // Answer queries
    for (int i = 0; i < q; i++)
    {
        static const int MAX_INT = std::numeric_limits<int>::max();
        int source, target;
        scanf("%d%d", &source, &target);
        
        int dist = dijkstra_dist(n, G, source, target);
        if (dist == MAX_INT)
            printf("unreachable\n");
        else
            printf("%d\n", dist);
    }

    return 0;
}