/**
 * The word "block" reminds me of minimum cut. As we are blocking on vertices,
 * it seems natural to split the vertices. Obvious any cut on internal edges (
 * edges which connect in-vert & out-vert) is a cut on the network, so maximum
 * flow <= answer. Also it can be proved that all optimal cuts can be 
 * transformed to a "vertex cut" without changing the total flow, so that's it.
 */
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
};

void testcase()
{
    int n, nEdges, nSrcs, nDsts;
    scanf("%d%d%d%d", &n, &nEdges, &nSrcs, &nDsts);

    const int baseOut = n, source = n + n, target = source + 1, nVerts = target + 1;
    graph G(nVerts);
    edge_adder adder(G);

    for (int i = 0; i < n; i++)
        adder.add_edge(i, baseOut + i, 1); // in-vert to out-vert
    for (int i = 0; i < nEdges; i++)
    {
        int from, to;
        scanf("%d%d", &from, &to);
        adder.add_edge(baseOut + from, to, 1);
    }
    for (int i = 0; i < nSrcs; i++)
    {
        int src;
        scanf("%d", &src);
        adder.add_edge(source, src, 1);
    }
    for (int i = 0; i < nDsts; i++)
    {
        int dst;
        scanf("%d", &dst);
        adder.add_edge(baseOut + dst, target, 1);
    }

    int flow = boost::push_relabel_max_flow(G, source, target);
    printf("%d\n", flow);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}