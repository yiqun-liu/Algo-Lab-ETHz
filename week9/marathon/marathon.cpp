#include <cstdio>
#include <vector>
#include <queue>
#include <limits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using std::vector; using std::queue;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    boost::no_property, boost::property<boost::edge_weight_t, int> >    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> flow_graph;

// for flow graph
class edge_adder {
    flow_graph &G;

 public:
    explicit edge_adder(flow_graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0;
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

struct NeighborNode
{
    int from; // not to
    int width;
    int length;
};
typedef struct NeighborNode Neighbor;
typedef vector<Neighbor> VertList;
typedef vector<VertList> AdjList;


void testcase()
{
    int nVerts, nEdges, source, target;
    scanf("%d%d%d%d", &nVerts, &nEdges, &source, &target);

    AdjList adjList(nVerts);
    weighted_graph G1(nVerts); // Dijkstra
    weight_map weights = boost::get(boost::edge_weight, G1);

    traits::edge_descriptor e;
    for (int i = 0; i < nEdges; i++)
    {
        int from, to, width, length;
        scanf("%d%d%d%d", &from, &to, &width, &length);
        e = boost::add_edge(from, to, G1).first; weights[e] = length;
        e = boost::add_edge(to, from, G1).first; weights[e] = length;
        
        Neighbor temp; temp.from = from; temp.width = width, temp.length = length;
        adjList[to].push_back(temp);
        temp.from = to;
        adjList[from].push_back(temp);

    }

    std::vector<int> dist_map(nVerts);
    int INFINITE = std::numeric_limits<int>::max();

    boost::dijkstra_shortest_paths(G1, source,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G1))));
    
    if (dist_map[target] == INFINITE) // not reachable
    {
        printf("0\n");
        return;
    }
    
    flow_graph G2(nVerts); // maximum flow
    edge_adder adder(G2);

    // Construct G2
    // BFS
    vector<bool> visited(nVerts, false);
    queue<int> Q;

    Q.push(target);
    visited[target] = true;
    while (!Q.empty())
    {
        int to = Q.front(); Q.pop();

        for (int i = 0; i < adjList[to].size(); i++)
        {
            int &from = adjList[to][i].from, &width = adjList[to][i].width, &length = adjList[to][i].length;
            if ( dist_map[from] == INFINITE || dist_map[to] -  dist_map[from] != length )
                continue;

            // 'from' may be on shortest path (if not, no harm)
            adder.add_edge(from, to, width);
            if (!visited[from])
            {
                Q.push(from);
                visited[from] = true;
            }
        }
    }

    int flow = boost::push_relabel_max_flow(G2, source, target);
    printf("%d\n", flow);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}