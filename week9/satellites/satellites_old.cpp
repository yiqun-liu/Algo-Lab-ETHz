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

const int UNPAIRED = -1;
void testcase()
{
    int nGronud, nSatellite, nLink;
    scanf("%d%d%d", &nGronud, &nSatellite, &nLink);

    // layout
    const int baseG = 0, baseS = nGronud; // node id = id within type + type base
    const int source = nGronud + nSatellite, target = source + 1, nNode = target + 1;

    graph G(nNode);
	edge_adder adder(G);

    for (int i = 0; i < nLink; i++)
    {
        int gIdx, sIdx;
        scanf("%d%d", &gIdx, &sIdx);
        adder.add_edge(baseG + gIdx, baseS + sIdx, 1);
    }
    for (int i = 0; i < nGronud; i++)
        adder.add_edge(source, baseG + i, 1);
    for (int i = 0; i < nSatellite; i++)
        adder.add_edge(baseS + i, target, 1);
    
    // Get maximum matching
    int max_flow = boost::push_relabel_max_flow(G, source, target);

    if (max_flow == 0) // no need to do extra work
    {
        printf("0 0\n");
        return;
    }
    
    // Compute minimum vertex cover
    // Residual capacity map: In this case, we can judge whether a node is 
    // matched using merely rc_map
    const auto rc_map = boost::get(boost::edge_residual_capacity, G);

    std::vector<bool> visited(nNode, false);
    int visitedSatellite = 0;
    // BFS to find vertex set S
	std::queue<int> Q;
	visited[source] = true;
	Q.push(source);
	while (!Q.empty()) 
    {
		int u = Q.front(); Q.pop();
		out_edge_it ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) 
        {
			const int v = boost::target(*ebeg, G);
            // help us alternate between matched edges and unmatched edges
			if (rc_map[*ebeg] == 0 || visited[v])
                continue;
			visited[v] = true;
			Q.push(v);
            if (baseS <= v && v < baseS + nSatellite)
                visitedSatellite++;
		}
	}

    printf("%d %d\n", max_flow - visitedSatellite, visitedSatellite);
    for (int i = baseG; i < baseG + nGronud; i++)
        if (!visited[i])
            printf("%d ", i - baseG);
    for (int i = baseS; i < baseS + nSatellite; i++)
        if (visited[i])
            printf("%d ", i - baseS);
    putchar('\n');
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}