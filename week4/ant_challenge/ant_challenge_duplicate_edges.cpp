#include <cstdio>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;

const int MAX_N_VERTS = 500;

void setWeights(weighted_graph& G, const std::vector< edge_desc >& edges, std::vector<int>& newWeights)
{
    weight_map weights = boost::get(boost::edge_weight, G);
    for (int i = 0; i < edges.size(); i++)
        weights[ edges[i] ] = newWeights[i];
}

void testcase()
{
    int nVerts, nEdges, nSpecies, source, target;
    scanf("%d%d%d%d%d", &nVerts, &nEdges, &nSpecies, &source, &target);

    weighted_graph G_species(nVerts);
    weight_map weights = boost::get(boost::edge_weight, G_species);

    std::vector< std::vector<int> > W_species( nSpecies );
    std::vector< edge_desc > edges(nEdges);

    for (int i = 0; i < nEdges; i++)
    {
        int v1, v2;
        scanf("%d%d", &v1, &v2);
        edge_desc e = boost::add_edge(v1, v2, G_species).first;
        
        edges[i] = e;
        for (int j = 0; j < nSpecies; j++)
        {
            int weight;
            scanf("%d", &weight);
            W_species[j].push_back(weight);
        }
    }

    // final network
    weighted_graph G(nVerts);
    weight_map minWeights = boost::get(boost::edge_weight, G);

    // Explore private networks, keep all edges of MST in final network
    for (int i = 0; i < nSpecies; i++)
    {
        int hive;
        scanf("%d", &hive); // not needed because MST is unique

        std::vector<edge_desc> mst; // edges of MST
        mst.reserve(nVerts - 1);

        setWeights(G_species, edges, W_species[i]);
        boost::kruskal_minimum_spanning_tree(G_species, std::back_inserter(mst));

        for (int j = 0; j < mst.size(); j++)
        {
            int v1 = boost::source(mst[j], G_species), v2 = boost::target(mst[j], G_species);
            int weight = weights[ mst[j] ];
            edge_desc e = boost::add_edge(v1, v2, G).first;
            minWeights[e] = weight;
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