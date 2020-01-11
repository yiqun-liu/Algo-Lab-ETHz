#include <cstdio>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// ---- Sample Code ----
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

class edge_adder {
    graph &G;
 public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, int capacity) {
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
// ---- Sample Code End ----

// mapping: coordinate -> vertex_id in our graph
int inVertex(int pos)  { return 2 * pos - 1; }
int outVertex(int pos) { return 2 * pos; }

void testcase()
{
    int width, nBricks;
    scanf("%d%d", &width, &nBricks);

    const int source = 0, target = 2 * width - 1; // correspond top left end/ right end of a row
    graph G(2 * width); // left-end 0, in(1), out(1), in(2), out(2), ... out(w-1), w right-end
    edge_adder adder(G);
    
    for (int i = 0; i < nBricks; i++)
    {
        int pos1, pos2;
        scanf("%d%d", &pos1, &pos2);
        if (pos1 < pos2) // ensure edges going from left to right
            adder.add_edge(outVertex(pos1), inVertex(pos2), 1); // capacity = 1: there is one such brick
        else
            adder.add_edge(outVertex(pos2), inVertex(pos1), 1);
    }

    for (int i = 1; i < width; i++) // Constraint: crack is never vertically aligned
        adder.add_edge(inVertex(i), outVertex(i), 1); // at most one brick can ends at this crack

    int flow = boost::push_relabel_max_flow(G, source, target);
    printf("%d\n", flow);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}