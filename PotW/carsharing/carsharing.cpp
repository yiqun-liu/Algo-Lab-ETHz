/**
 * The constraints can be easily expressed as flows, but the our objective - profit
 * seems to have no relationship with "maximum flow", which bothered me quite a while.
 * The key is keep the maximum flow constant (in this case), or ensure that the flow
 * is maximized in an optimal solution, then use MCMF to find the min cost.
 * With this presumption, we can always avoid processing negative cost, even if we're
 * seeking maximum value.
 * 
 * Non-negative cost: we add a base cost which is a function of time. For unit flow,
 * it will generate a cost of `maxTime * MAX_PROFIT` by default.
 * 
 * My code is not optimal because I am not familiar with vertex_desc of BGL
 */
#include <cstdio>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, int> > > > > graph;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;
class edge_adder {
    graph &G;
public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, int capacity, int cost) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G);
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0;
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;
        w_map[rev_e] = -cost;
    }
};

using std::map; using std::make_pair; using std::pair;
typedef pair<int, int> Node; // station index, time
struct RequestNode
{
    Node from;
    Node to;
    int profit;
};
typedef struct RequestNode Request;

const int MAX_N_STATIONS = 10, MAX_N_REQUESTS = 10000, MAX_PROFIT = 100;
int initCars[MAX_N_STATIONS + 1]; // 1-based index
Request requests[MAX_N_REQUESTS];

void testcase()
{
    int nRequests, nStations;
    scanf ("%d%d", &nRequests, &nStations);

    int nCars = 0, nVerts = 1; // we keep 1-based index in flow graph
    map<Node, int> index;
    for (int i = 1; i <= nStations; i++)
    {
        scanf("%d", &initCars[i]);
        nCars += initCars[i];

        Node initNode;
        initNode.first = i; initNode.second = 0;
        index.insert( make_pair(initNode, nVerts++) );
    }
    
    int maxTime = 0;
    for (int i = 0; i < nRequests; i++)
    {
        scanf("%d%d%d%d%d", &requests[i].from.first, &requests[i].to.first, 
            &requests[i].from.second, &requests[i].to.second, &requests[i].profit);
        
        // Maintain: node - index mapping
        map<Node, int>::iterator it_from = index.find(requests[i].from);
        if (it_from == index.end())
            index.insert( make_pair(requests[i].from, nVerts++) );
        map<Node, int>::iterator it_to = index.find(requests[i].to);
        if (it_to == index.end())
            index.insert( make_pair(requests[i].to, nVerts++) );
        
        maxTime = std::max(maxTime, requests[i].to.second);
    }

    // Construct graph
    const int source = 0, target = nVerts++, MAX_FLOW = nCars;
    graph G(nVerts);
    edge_adder adder(G);

    // Connect super source with (station, 0)
    for (int i = 1; i <= nStations; i++)
        adder.add_edge(source, i, initCars[i], 0);
    
    // Customer requests
    for (int i = 0; i < nRequests; i++)
    {
        int from = index[ requests[i].from ], to = index[ requests[i].to];
        int cost = (requests[i].to.second - requests[i].from.second) * MAX_PROFIT - requests[i].profit;
        adder.add_edge(from, to, 1, cost);
    }
    
    map<Node, int>::iterator lastIt = index.begin(), it = index.begin(); it++;
    for (; it != index.end(); it++)
    {
        // Aliases: only for readability
        const int &station = (it->first).first, &lastStation = (lastIt->first).first;
        const int &time = (it->first).second, &lastTime = (lastIt->first).second;
        const int &index = it->second, &lastIndex = lastIt->second;

        if ( station == lastStation )  // same station, different time
            adder.add_edge(lastIndex, index, MAX_FLOW, (time - lastTime) * MAX_PROFIT );
        else // last station --> super sink
            adder.add_edge(lastIndex, target, MAX_FLOW, (maxTime - lastTime) * MAX_PROFIT );
        lastIt = it;
    }
    adder.add_edge( index.rbegin()->second, target, MAX_FLOW, 
        (maxTime - index.rbegin()->first.second) * MAX_PROFIT);

    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int bias = maxTime * MAX_PROFIT;
    int cost = bias * nCars - boost::find_flow_cost(G);
    printf("%d\n", cost);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}
