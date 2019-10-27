#include <iostream>
#include <string>

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

const int N_ALPHABET = 26;
int count[N_ALPHABET];

void testcase()
{
    int h, w;
    std::string msg;
    
    std::cin >> h >> w;
    std:: cin >> msg;

    // source, character, alphabet, target
    int source = 0;
    int target = 1 + h * w + N_ALPHABET;

    graph G(1 + h * w + N_ALPHABET + 1);
    edge_adder adder(G);
    
    memset(count, 0, sizeof(count[0]) * 26);
    for (int i = 0; i < msg.length(); i++)
        count[msg[i] - 'A']++;
    
    std::string temp;

    // front side
    int idx = 1;
    for (int i = 0; i < h; i++)
    {
        std::cin >> temp;
        for (int j = 0; j < w; j++)
        {
            int charIdx = 1 + h * w + temp[j] - 'A';
            adder.add_edge(source, idx, 1);
            adder.add_edge(idx, charIdx, 1);
            idx++;
        }
    }
    
    // back side
    idx = 0;
    for (int i = 0; i < h; i++)
    {
        std::cin >> temp;
        idx += w;
        for (int j = 0; j < w; j++)
        {
            int charIdx = 1 + h * w + temp[j] - 'A';
            adder.add_edge(idx, charIdx, 1);
            idx--;
        }
        idx += w;
    }

    // alphabet to target
    for (int i = 0; i < N_ALPHABET; i++)
        if (count[i])
            adder.add_edge(1 + h * w + i, target, count[i]);
    
    long flow = boost::push_relabel_max_flow(G, source, target);
    if (flow == msg.length())
        std::cout << "Yes\n";
    else
        std::cout << "No\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while(t--) testcase();
    return 0;
}