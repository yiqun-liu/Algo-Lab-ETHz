#include <cstdio>

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

const int MAX_N_GUIDES = 5000, MAX_N_ELEPHANTS_PER_GUIDE = 1000;
const int MAX_N_ELEPHANTS = MAX_N_GUIDES * MAX_N_ELEPHANTS_PER_GUIDE;

void testcase()
{
    int nCities, nGuides, budget, start, end;
    scanf("%d%d%d%d%d", &nCities, &nGuides, &budget, &start, &end);
    // printf("\t%d %d\n", nCities, nGuides);

    int nVertices = nCities + 1, superSrc = nCities;
    graph G(nVertices);
    edge_adder adder(G);
    for (int i = 0; i < nGuides; i++)
    {
        int from, to, cost, nElephants;
        scanf("%d%d%d%d", &from, &to, &cost, &nElephants);
        adder.add_edge(from, to, nElephants, cost);
    }
    adder.add_edge(superSrc, start, MAX_N_ELEPHANTS, 0);

    // In this case we are sure there is only one edge leaving the source
    out_edge_it e, eend;
    boost::tie(e, eend) = boost::out_edges(boost::vertex(superSrc, G), G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    int l = 0, r = MAX_N_ELEPHANTS, currentBest = 0;
    while (l <= r)
    {
        int middle = (l + r) / 2;
        c_map[*e] = middle;

        boost::successive_shortest_path_nonnegative_weights(G, superSrc, end);
        int cost = boost::find_flow_cost(G);
        int flow = c_map[*e] - rc_map[*e];

        if (cost == budget)
        {
            currentBest = flow;
            break;
        }
        if (cost < budget)
        {
            currentBest = flow > currentBest ? flow : currentBest;
            l = middle + 1;
        }
        else
            r = middle - 1;
    }
    printf("%d\n", currentBest);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}