#include <cstdio>
#include <vector>
#include <queue>
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
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;

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

const bool IN_S_SET = true, IN_T_SET = !IN_S_SET;
const int MAX_FIG = 200, MAX_LIMB = 5000, MAX_COST = 1000, 
    MAX_TOTAL_CUT = MAX_LIMB * MAX_COST;
void testcase()
{
    int n, m;
    scanf("%d%d", &n, &m);

    // Build the graph
    graph G(n);
	edge_adder adder(G);

    for (int i = 0; i < m; i++)
    {
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        adder.add_edge(a, b, c); // TODO: consider merge edges
    }

    // Choose pivot "randomly" for more robustness 
    int pair = -1, pivot = n / 2;
    bool belong = IN_S_SET;
    int minCut = MAX_TOTAL_CUT;

    // try all figures as source or sink to search the "global" min cut
    for (int i = 0; i < n; i++)
    {
        if (i == pivot)
            continue;
        
        int cut;
        // If pivot is in S
        cut = boost::push_relabel_max_flow(G, pivot, i);
        if (cut < minCut)
        {
            minCut = cut;
            belong = IN_S_SET;
            pair = i;
        }

        // If pivot is in T
        cut = boost::push_relabel_max_flow(G, i, pivot);
        if (cut < minCut)
        {
            minCut = cut;
            belong = IN_T_SET;
            pair = i;
        }  
    }
    
    printf("%d\n", minCut);

    // update residual flow map
    int source = belong == IN_S_SET ? pivot : pair;
    int target = belong == IN_S_SET ? pair : pivot;
    boost::push_relabel_max_flow(G, source, target);

    // Compute minimum vertex cover
    const auto rc_map = boost::get(boost::edge_residual_capacity, G);

    std::vector<bool> visited(n, false);
    int taken[MAX_FIG], count = 0; // record S memeber
    // BFS to find vertex set S
	std::queue<int> Q;
	visited[source] = true;
    taken[count++] = source;
	Q.push(source);
	while (!Q.empty()) 
    {
		int u = Q.front(); Q.pop();
		out_edge_it ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) 
        {
			const int v = boost::target(*ebeg, G);
			if (rc_map[*ebeg] == 0 || visited[v])
                continue;
			visited[v] = true;
			Q.push(v);
            taken[count++] = v;
		}
	}

    printf("%d", count);
    for (int i = 0; i < count; i++)
        printf(" %d", taken[i]);
    putchar('\n');
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}