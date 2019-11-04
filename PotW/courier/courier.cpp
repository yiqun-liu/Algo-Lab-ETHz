#include <cstdio>

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

const int MAX_COST = 5000;

void testcase()
{
    int nZones, nJobs;
    scanf("%d%d", &nZones, &nJobs);

    // structure: source --> job layer --> zone layer --> target
    // zone: [0, nZones) job: [nZones, nZones + nJobs)
    int source = nZones + nJobs, target  = source + 1;
    graph G(nZones + nJobs + 1);
    edge_adder adder(G);

    int cost;
    for (int i = 0; i < nZones; i++)
    {
        scanf("%d", &cost);
        adder.add_edge(i, target, cost);
    }
    
    int reward, total = 0;
    for (int i = 0; i < nJobs; i++)
    {
        scanf("%d", &reward);
        total += reward;
        adder.add_edge(source, nZones + i, reward);
    }
    
    for (int i = 0; i < nJobs; i++)
    {
        int n, zone;
        scanf("%d", &n);
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &zone);
            adder.add_edge(nZones + i, zone, MAX_COST);
        }
    }

    // scratch of idea: assume we take all the assignments at first
    // - For some ('minimum') subset of 'assignments' and some subset of 'zones' (which is
    // the must for those assignments), if rewards is greater, we will pay all the 
    // cost (which is the flow), because it is profitable
    // - Otherwise, we would the maximum flow would be the rewards, it is not worthy to 
    // buy tickets for those area, and by subtracting the flow, we have our customer 'refunded'
    
    long flow = boost::push_relabel_max_flow(G, source, target);
    printf("%d\n", total - flow);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}