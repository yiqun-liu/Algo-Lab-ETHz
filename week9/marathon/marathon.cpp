#include <cstdio>
#include <vector>
#include <map>
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

struct NodeStruct
{
    int toVertex;
    int length;
    int width;
};
typedef struct NodeStruct Node;

const int UNKNOWN = -1;
// we need f for acceleration
bool dijkstra(const std::vector<std::vector<Node> > &adjList, int n, int s, int f,
    std::vector<int>& dist, std::vector<std::vector<std::pair<int, int> > >& pred)
{
    std::vector<bool> known(n, false);
    std::multimap<int, int> minHeap; // dist -> vertex id
    minHeap.insert(std::make_pair(0, s));
    dist[s] = 0;

    while (!minHeap.empty())
    {
        std::multimap<int, int>::iterator it = minHeap.begin();
        const int from = it->second;
        known[from] = true;
        minHeap.erase(it);

        if (from == f)
            return true; // Reachable, no need to compute dist anymore

        for (int i = 0; i < adjList[from].size(); i++)
        {
            const int& to = adjList[from][i].toVertex, &length = adjList[from][i].length,
                &width = adjList[from][i].width;
            if (known[to]) continue;
            
            if (dist[to] == UNKNOWN) // not in heap
            {
                dist[to] = dist[from] + length;
                minHeap.insert(std::make_pair(dist[to], to));
                pred[to].push_back(std::make_pair(from, width));
            }
            else if (dist[from] + length == dist[to]) // add another predecessor
                pred[to].push_back(std::make_pair(from, width));
            else if (dist[from] + length < dist[to]) // update dist in heap
            {
                std::multimap<int, int>::iterator toDelete = minHeap.find(dist[to]);
                while (toDelete->second != to)
                    toDelete++;
                minHeap.erase(toDelete);

                dist[to] = dist[from] + length;
                minHeap.insert(std::make_pair(dist[to], to));
                pred[to].clear();
                pred[to].push_back(std::make_pair(from, width));
            }
        }
    }

    return false; // Unreachable
}


void testcase()
{
    int n, m, s, f;
    scanf("%d%d%d%d", &n, &m, &s, &f);
    
    std::vector<std::vector<Node> > adjList(n);
    for(int i = 0; i < m; i++)
    {
        Node node;
        int a, b, width, length;
        scanf("%d%d%d%d", &a, &b, &width, &length);
        node.length = length; node.width = width;
        // all edges are bidirectional
        node.toVertex = b;
        adjList[a].push_back(node);
        
        node.toVertex = a;
        adjList[b].push_back(node);
        
        // It seems we doubled the width of street, but this causes no problem as length>0
    }

    std::vector<int> dist(n, UNKNOWN);
    std::vector<std::vector<std::pair<int, int> > > pred(n);

    if (dijkstra(adjList, n, s, f, dist, pred) == false) // not reachable
    {
        printf("0\n");
        return;
    }

    graph G(n);
    edge_adder adder(G);
    
    std::queue<int> Q;
    std::vector<bool> added(n, false);
    // build graphs by BFS
    Q.push(f);
    added[f] = true;

    while(!Q.empty())
    {
        int to = Q.front(); Q.pop();
        for (int i = 0; i < pred[to].size(); i++)
        {
            const int& from = pred[to][i].first, &width = pred[to][i].second;
            adder.add_edge(from, to, width);

            if (!added[from] && from != s)
            {
                Q.push(from);
                added[from] = true;
            }
        }
    }

    int flow = boost::push_relabel_max_flow(G, s, f);
    printf("%d\n", flow);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}