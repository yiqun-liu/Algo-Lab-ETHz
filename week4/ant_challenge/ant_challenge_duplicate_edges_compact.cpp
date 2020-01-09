#include <cstdio>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;

void testcase()
{
    int nVerts, nEdges, nSpecies, source, target;
    scanf("%d%d%d%d%d", &nVerts, &nEdges, &nSpecies, &source, &target);

    // Construct one graph for each species
    std::vector<weighted_graph> G_species(nSpecies, weighted_graph(nVerts));
    std::vector<weight_map> W_species(nSpecies);
    for (int i = 0; i < nSpecies; i++)
        W_species[i] = boost::get(boost::edge_weight, G_species[i]);

    for (int i = 0; i < nEdges; i++)
    {
        int v1, v2;
        scanf("%d%d", &v1, &v2);
        for (int j = 0; j < nSpecies; j++)
        {
            int weight;
            scanf("%d", &weight);
            boost::add_edge(v1, v2, weight, G_species[j]);
        }
    }

    // The union of private networks of each species
    weighted_graph G(nVerts);

    // Explore private networks, keep all edges of MST in final network
    for (int i = 0; i < nSpecies; i++)
    {
        int hive;
        scanf("%d", &hive); // not needed because MST is unique

        std::vector<edge_desc> mst; // edges of MST
        mst.reserve(nVerts - 1);

        boost::kruskal_minimum_spanning_tree(G_species[i], std::back_inserter(mst));

        for (int j = 0; j < mst.size(); j++)
        {
            int v1 = boost::source(mst[j], G_species[i]), v2 = boost::target(mst[j], G_species[i]);
            edge_desc e = boost::add_edge(v1, v2, W_species[i][ mst[j] ], G).first;
        }
    }

    // Run Dijkstra
    std::vector<int> dist_map(nVerts);

    boost::dijkstra_shortest_paths(G, source,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));

    printf("%d\n", dist_map[target]);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}