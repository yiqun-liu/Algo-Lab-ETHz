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

const int MAX_M = 1000;
const int MAX_N = 200;

int score[MAX_N];

void testcase()
{
    int n, m;
    std::cin >> n >> m;

    graph g(n); // at least n players
    edge_adder adder(g);

    memset(score, 0, sizeof(score[0]) * n);
    int roundIdx = n;
    for (int i = 0; i < m; i++)
    {
        int a, b, c;
        std::cin >> a >> b >> c;
        switch (c)
        {
        case 0: // not recorded
            adder.add_edge(roundIdx, a, 1);
            adder.add_edge(roundIdx, b, 1);
            roundIdx++;
            break;
        case 1:
            score[a]++; break;
        case 2:
            score[b]++; break;
        }
    }
    const vertex_desc v_source = boost::add_vertex(g);
    const vertex_desc v_target = boost::add_vertex(g);
    int unrecorded = roundIdx - n;

    bool possible = true, needRun = false;
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        int record;
        std::cin >> record;
        score[i] = record - score[i]; // to score
        sum += score[i];
        if (score[i] < 0) possible = false;
        if (score[i] > 0) needRun = true;
    }

    if (!possible || sum != unrecorded)
    {
        std::cout << "no\n";
        return;
    }
    if (!needRun)
    {
        std::cout << "yes\n";
        return;
    }

    // TODO: consider scenario: only a small portion of the game is not recorded
    // Add super source & target
    for (int i = 0; i < n; i++)
        adder.add_edge(i, v_target, score[i]);
    for (int i = n; i < roundIdx; i++)
        adder.add_edge(v_source, i, 1);
    
    long flow = boost::push_relabel_max_flow(g, v_source, v_target);
    if (flow == unrecorded)
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
}