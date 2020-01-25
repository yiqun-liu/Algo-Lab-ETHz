/**
 * Attention: this program can only score 90/100. I keep it as a separate file because it
 * already express the core idea. Mixing with further optimizations would only make code review harder.
 * 
 * This problem shares some similarity with "carsharing"
 * The structure of this network is really tricky. It takes some thoughts to accept its correctness.
 */
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
    void add_edge(int from, int to, int capacity, int cost) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G);
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

const int MAX_N_STOPS = 300, MAX_VALUE = 1 << 7, UNIT_COST = MAX_VALUE;
int capacity[MAX_N_STOPS];

void testcase()
{
    int nStops, nItems;
    scanf("%d%d", &nStops, &nItems);

    for (int i = 0; i < nStops - 1; i++)
        scanf("%d", &capacity[i]);
    
    const int source = nStops, target = source + 1, nVerts = target + 1;
    graph G(nVerts);
    edge_adder adder(G);

    // To make all costs non-negative, we add offsets to edges.
    // All flows from source to sink by default are of cost (nStops * UNIT_COST)
    int flow = capacity[0]; // source alone, and V - {target} are both all min cuts
    adder.add_edge(source, 0, capacity[0], 0);
    adder.add_edge(nStops - 1, target, capacity[nStops - 2], UNIT_COST);
    for (int i = 1; i < nStops - 1; i++)
    {
        adder.add_edge(i - 1, i, capacity[i - 1], UNIT_COST);
        // "delta" edges
        if (capacity[i] > capacity[i - 1])
        {
            adder.add_edge(source, i, capacity[i] - capacity[i - 1], i * UNIT_COST);
            flow += capacity[i] - capacity[i - 1];
        }
        if (capacity[i] < capacity[i - 1])
            adder.add_edge(i, target, capacity[i - 1] - capacity[i], (nStops - i) * UNIT_COST);
    }
    adder.add_edge(nStops - 2, nStops - 1, capacity[nStops - 2], UNIT_COST);

    for (int i = 0; i < nItems; i++)
    {
        int from, to, value;
        scanf("%d%d%d", &from, &to, &value);
        adder.add_edge( from, to, 1, (to - from) * UNIT_COST - value );
    }

    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost = boost::find_flow_cost(G);
    int bias = flow * nStops * UNIT_COST;
    printf("%d\n", bias - cost);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}