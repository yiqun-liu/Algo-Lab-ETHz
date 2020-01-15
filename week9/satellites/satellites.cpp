#include <cstdio>
#include <vector>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using std::vector; using std::queue;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >    graph;
typedef boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator    out_edge_it;

class edge_adder {
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, int capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

void testcase()
{
    int g, s, l;
    scanf("%d%d%d", &g, &s, &l);

    // vertex index: 0~(g-1) ground station, g~(g+s-1) satellites, source, sink
    const int baseS = g, source = g + s, target = g + s + 1, nVerts = target + 1;
    graph G(nVerts);
    edge_adder adder(G);

    // Construct Graph
    for (int i = 0; i < l; i++) // ground -> satellite
    {
        int ground, satellite;
        scanf("%d%d", &ground, &satellite);
        adder.add_edge(ground, baseS + satellite, 1);
    }
    for (int i = 0; i < g; i++) // source -> ground station
        adder.add_edge(source, i, 1);
    for (int i = 0; i < s; i++) // satellites -> sink
        adder.add_edge(baseS + i, target, 1);
    
    int flow = boost::push_relabel_max_flow(G, source, target);
    if (flow == 0) // no need to do extra work
    {
        printf("0 0\n");
        return;
    }

    // BFS
    int vis_G = 0;
    vector<bool> visited(nVerts, false);
    queue<int> Q;
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    visited[source] = true;
    Q.push(source);
    while (!Q.empty())
    {
        int vertex = Q.front();
        Q.pop();

        visited[vertex] = true;
        if (vertex < g)
            vis_G++;
        
        out_edge_it e, eend;
		for (boost::tie(e, eend) = boost::out_edges(vertex, G); e != eend; ++e) 
        {
            int to = boost::target(*e, G);
            if (rc_map[*e] > 0 && !visited[to])
            {
                visited[to] = true;
                Q.push(to);
            }   
        }
    }

    int g0 = g - vis_G, s0  = flow - g0;
    printf("%d %d\n", g0, s0); // g0, s0
    
    for (int i = 0; i < g; i++)
        if (!visited[i])
            printf("%d ", i);
    for (int i = 0; i < s; i++)
        if (visited[baseS + i])
            printf("%d ", i);
    putchar('\n');
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--)
        testcase();
}