#include <cstdio>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// ---- Customized Sample Code ----
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

class edge_adder {
    graph &G;
 public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, int capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
    void add_undirected_edge(int from, int to, int capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = capacity; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};
// ---- Sample Code End ----

void testcase()
{
    int nVerts, nEdges, nStores;
    scanf("%d%d%d", &nVerts, &nEdges, &nStores);

    const int source = 0, target = nVerts; // super sink
    graph G(nVerts + 1);
    edge_adder adder(G);

    // number of stores on each intersection, helpful when there are many stores overlap, but not necessary
    std::vector<int> stores(nVerts, 0); 
    for (int i = 0; i < nStores; i++)
    {
        int pos;
        scanf("%d", &pos);
        stores[pos]++;
    }
    for (int i = 0; i < nVerts; i++)
        if (stores[i])
            adder.add_edge(i, target, stores[i]); // sink -> super sink

    for (int i = 0; i < nEdges; i++) // original graph
    {
        int from, to;
        scanf("%d%d", &from, &to);
        adder.add_undirected_edge(from, to, 1);
    }
    
    int flow = boost::push_relabel_max_flow(G, source, target); // maximum flow
    if (flow == nStores)
        printf("yes\n");
    else
        printf("no\n");
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}