#include <cstdio>
#include <vector>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;

const int MAX_N_VERTS = 500;

struct MyEdgeNode
{
    int smaller; // vertex index
    int greater;
};
typedef struct MyEdgeNode MyEdge;

class EdgeHasher
{
public:
    std::size_t operator()(const MyEdge& k) const
    {
        return k.smaller * MAX_N_VERTS + k.greater; // no collision
    }
};

bool operator==(const MyEdge& e1, const MyEdge& e2) 
{
    return e1.smaller == e2.smaller && e1.greater == e2.greater;
}

MyEdge desc2MyEdge(const edge_desc& e, const weighted_graph& G)
{
    MyEdge myEdge;

    myEdge.smaller = boost::source(e, G);
    myEdge.greater = boost::target(e, G);

    if (myEdge.smaller > myEdge.greater) // swap to make suer myEdge.smaller <= myEdge.greater
    {
        int temp = myEdge.smaller;
        myEdge.smaller = myEdge.greater;
        myEdge.greater = temp;
    }

    return myEdge;
}

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

    // hash map: edge --> minimum weight
    std::unordered_map<MyEdge, int, EdgeHasher> network;

    // Explore private networks
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
            MyEdge key = desc2MyEdge(mst[j], G_species);
            int value = weights[ mst[j] ];

            std::unordered_map<MyEdge, int, EdgeHasher>::iterator it = network.find(key);
            if (it == network.end()) // not found
                network[key] = value; // insert by assignment
            else if ( it->second > value)
                it->second = value;
        }
    }

    // Construct the final graph
    weighted_graph G(nVerts);
    weight_map minWeights = boost::get(boost::edge_weight, G);
    for (std::unordered_map<MyEdge, int, EdgeHasher>::iterator it = network.begin(); it != network.end(); it++)
    {
        const MyEdge& edge = it->first;
        const int& weight = it->second;

        edge_desc e = boost::add_edge(edge.smaller, edge.greater, G).first;
        minWeights[e] = weight;
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