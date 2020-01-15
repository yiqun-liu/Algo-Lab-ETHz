#include <cstdio>
// ---------sample code-----------
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
    graph &G;

    public:
        explicit edge_adder(graph &G) : G(G) {}
        void add_edge(int from, int to, long capacity, long cost) {
            auto c_map = boost::get(boost::edge_capacity, G);
            auto r_map = boost::get(boost::edge_reverse, G);
            auto w_map = boost::get(boost::edge_weight, G); // new!
            const edge_desc e = boost::add_edge(from, to, G).first;
            const edge_desc rev_e = boost::add_edge(to, from, G).first;
            c_map[e] = capacity;
            c_map[rev_e] = 0; // reverse edge has no capacity!
            r_map[e] = rev_e;
            r_map[rev_e] = e;
            w_map[e] = cost;   // new assign cost
            w_map[rev_e] = -cost;   // new negative cost
        }
};
// ---------sample code end-----------

const int MAX_P = 20, BIAS = MAX_P;
void testcase()
{
    int n;
    scanf("%d", &n);

    const int source = n, target = n + 1, nNode = n + 2;
    // Build the graph. 3 layers: source, day nodes, sink
    graph G(nNode);
	edge_adder adder(G);

    // source to day nodes
    for (int i = 0; i < n; i++)
    {
        int a, c;
        scanf("%d%d", &a, &c);
        adder.add_edge(source, i, a, c);
    }

    int need = 0;
    // day nodes to sink
    for (int i = 0; i < n; i++)
    {
        int s, p;
        scanf("%d%d", &s, &p);
        // treat income negative cost s.t. we can use MCMF to maximize it
        // however, negative cost results in inefficient algo.
        // we make it nonnegative while keeping relative difference
        adder.add_edge(i, target, s, -p + BIAS);
        need += s;
    }

    // between day nodes (preserving food to next day)
    for (int i = 0; i < n - 1; i++)
    {
        int v, e;
        scanf("%d%d", &v, &e);
        adder.add_edge(i, i + 1, v, e);
    }

    // Solve MCMF
    boost::successive_shortest_path_nonnegative_weights(G, source, target);    
    
    // Get flow & cost value
    int cost = boost::find_flow_cost(G);
    int flow = 0;
    out_edge_it e, eend;
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G); e != eend; ++e)
        flow += c_map[*e] - rc_map[*e];
    
    if (flow == need)
        printf("possible");
    else
        printf("impossible");
    
    printf(" %d %d\n", flow, -(cost - flow * BIAS));
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}