#include <cstdio>
#include <cstring>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

const int MAX_N = 200, MAX_M = 1000;
int points[MAX_N], player[MAX_M][2];


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

void testcase()
{
    int n, m;
    scanf("%d%d", &n, &m);

    int unknown = 0;
    memset(points, 0, sizeof(int) * n); // points a player need to get to meet the final points
    for (int i = 0; i < m; i++)
    {
        int p1, p2, result;
        scanf("%d%d%d", &p1, &p2, &result);
        if (result == 0)
        {
            player[unknown][0] = p1;
            player[unknown++][1] = p2;
        }
        else if (result == 1) // p1 won
            points[p1]--; // verified point = -(point to assign)
        else // p2 won
            points[p2]--;
    }

    bool possible = true;
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        int finalPoints;
        scanf("%d", &finalPoints);
        points[i] += finalPoints;
        if (points[i] < 0)
            possible = false;
        sum += points[i];
    }

    if (!possible || sum != unknown) // Judge by other feature
    {
        printf("no\n");
        return;
    }

    // Construct Graph: player:[0,n-1], game:[n, n+unknown-1]
    const int baseGame = n, source = n + unknown, target = n + unknown + 1;
    graph G(n + unknown + 2);
    edge_adder adder(G);
    for (int i = 0; i < unknown; i++)
    {
        int gameIdx = baseGame + i;
        adder.add_edge(source, gameIdx, 1);
        adder.add_edge(gameIdx, player[i][0], 1);
        adder.add_edge(gameIdx, player[i][1], 1);
    }
    for (int i = 0; i < n; i++)
        adder.add_edge(i, target, points[i]);
    
    int flow = boost::push_relabel_max_flow(G, source, target);
    if (flow == unknown)
        printf("yes\n");
    else
        printf("no\n");
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}