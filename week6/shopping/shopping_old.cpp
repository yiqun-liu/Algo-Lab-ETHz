#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor > > > > graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// customized sample code
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_two_edges(int e1, int e2, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(e1, e2, G).first;
    const auto rev_e = boost::add_edge(e2, e1, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = capacity; // reverse edge in undirected class has capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase()
{
    int n, m, s;
    std::cin >> n >> m >> s;

    std::vector<int> nStore(n, 0);
    for (int i = 0; i < s; i++)
    {
        int pos;
        std::cin >> pos;
        nStore[pos]++;
    }

    graph G(n + 1); // plus artificial node as target
    int target = n;
    int source = 0;
    edge_adder adder(G);

    for (int i = 0; i < n; i++)
        if (nStore[i]) adder.add_two_edges(i, target, nStore[i]);

    for (int i = 0; i < m; i++)
    {
        int a, b;
        std::cin >> a >> b;
        adder.add_two_edges(a, b, 1);
    }

    long flow = boost::push_relabel_max_flow(G, source, target);

    if (flow == s)
        std::cout << "yes\n";
    else
        std::cout << "no\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while(t--) testcase();
    return 0;
}