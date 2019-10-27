#include <iostream>

// ---------sample code-----------
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

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
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
// ---------sample code end-----------

inline int inVertex(int i){ return 2 * i; }
inline int outVertex(int i){ return 2 * i + 1; }

void testcase()
{
    int w, n;
    std::cin >> w >> n;

    graph G(2 * w + 1);
    edge_adder adder(G);

    int source = outVertex(0), target = inVertex(w);
    for (int i = 1; i < w; i++)
        adder.add_edge(inVertex(i), outVertex(i), 1); // vertex disjoint
    
    for (int i = 0; i < n; i++)
    {
        int a, b;
        std::cin >> a >> b;
        int left = a < b ? a : b;
        int right = a > b ? a : b;
        // what will happen for redundant edges?
        adder.add_edge(outVertex(left), inVertex(right), 1);
    }
    
    long flow = boost::push_relabel_max_flow(G, source, target);
    std::cout << flow << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while(t--) testcase();
    return 0;
}