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

const int MAX_C = 1000, MAX_N_EDGES = 5000;

void testcase()
{
    int nVerts, nEdges;
    scanf("%d%d", &nVerts, &nEdges);

    graph G(nVerts);
    edge_adder adder(G);

    for (int i = 0; i < nEdges; i++)
    {
        int from, to, cost;
        scanf("%d%d%d", &from, &to, &cost);
        adder.add_edge(from, to, cost);
    }

    const int pivot = 0;
    int minFlow = MAX_C * MAX_N_EDGES, partner;
    bool asSource = true;
    for (int t = 1; t < nVerts; t++)
    {
        int flow = boost::push_relabel_max_flow(G, pivot, t);
        if (flow < minFlow)
        {
            minFlow = flow;
            partner = t;
        }
    }
    
    for (int s = 1; s < nVerts; s++)
    {
        int flow = boost::push_relabel_max_flow(G, s, pivot);
        if (flow < minFlow)
        {
            minFlow = flow;
            partner = s;
            asSource = false;
        }
    }

    const int source = asSource ? pivot : partner;
    const int target = asSource ? partner : pivot;
    int flow = boost::push_relabel_max_flow(G, source, target);

    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    std::vector<bool> vis(nVerts, false);
    std::queue<int> Q;
    vis[source] = true;
    int count = 1;
    Q.push(source);
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        out_edge_it ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = boost::target(*ebeg, G);
            // Only follow edges with spare capacity
            if (rc_map[*ebeg] == 0 || vis[v]) continue;
            vis[v] = true;
            count++;
            Q.push(v);
        }
    }

    printf("%d\n", flow);
    printf("%d", count);
    for (int i = 0; i < nVerts; i++)
        if (vis[i])
            printf(" %d", i);
    putchar('\n');
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}