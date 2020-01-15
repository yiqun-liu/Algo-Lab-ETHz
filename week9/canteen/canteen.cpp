#include <cstdio>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, int> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

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
        c_map[rev_e] = 0;
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;
        w_map[rev_e] = -cost;
    }
};

const int MAX_P = 20, BIAS = MAX_P;
void testcase()
{
    int n;
    scanf("%d", &n);

    // [0,n-1] day, source, target
    const int source = n, target = source + 1, nVerts = target + 1;
    graph G(nVerts);
    edge_adder adder(G);

    for (int i = 0; i < n; i++) // source -- production ->
    {
        int amount, cost;
        scanf("%d%d", &amount, &cost);
        adder.add_edge(source, i, amount, cost);
    }

    int nStudents = 0;
    for (int i = 0; i < n; i++) // -- consume -> target
    {
        int demand, gain;
        scanf("%d%d", &demand, &gain);
        adder.add_edge(i, target, demand, -gain + BIAS);
        nStudents += demand;
    }

    for (int i = 0; i < n - 1; i++) // internal edges: preserve
    {
        int amount, cost;
        scanf("%d%d", &amount, &cost);
        adder.add_edge(i, i + 1, amount, cost);
    }

    boost::successive_shortest_path_nonnegative_weights(G, source, target);    
    
    // Get flow & cost value
    int cost = boost::find_flow_cost(G);
    int flow = 0;
    out_edge_it e, eend;
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    for(boost::tie(e, eend) = boost::out_edges(source, G); e != eend; ++e)
        flow += c_map[*e] - rc_map[*e];

    if (flow == nStudents)
        printf("possible");
    else
        printf("impossible");
    printf(" %d %d\n", flow, -cost + flow * BIAS);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}