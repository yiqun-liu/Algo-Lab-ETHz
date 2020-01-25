#include <cstdio>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

class edge_adder {
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, int capacity) 
    {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0;
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

const int MAX_N_CLUES= 500;

void testcase()
{
    int nVerts, nEdges, nStations, nClues;
    scanf("%d%d%d%d", &nVerts, &nEdges, &nStations, &nClues);
    
    // General idea: build two mirror network and split the clue nodes
    const int base = nVerts, source = nVerts * 2, target = source + 1;
    graph G(target + 1);
    edge_adder adder(G);

    for (int i = 0; i < nStations; i++)
    {
        int station;
        scanf("%d", &station);
        adder.add_edge(source, station, 1); // INIFITIY
        adder.add_edge(base + station, target, 1);
    }

    for (int i = 0; i < nClues; i++)
    {
        int clue;
        scanf("%d", &clue);
        adder.add_edge(clue, base + clue, 1); // in vertex to out vertex
    }

    for (int i = 0; i < nEdges; i++)
    {
        int from, to;
        scanf("%d%d", &from, &to);
        adder.add_edge(from, to, MAX_N_CLUES); // INIFITIY
        adder.add_edge(base + from, base + to, 1); // asking for edge disjoint paths
    }

    int flow = boost::push_relabel_max_flow(G, source, target);
    printf("%d\n", flow);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}