#include <cstdio>
#include <vector>
#include <queue>

using std::vector; using std::priority_queue; using std::make_pair;
typedef std::pair<int, int> Element; // distance to known, vertex index

const int MAX_N = 1000, UNKNOWN = -1, MAX_COST = 1 << 20;
int cost[MAX_N][MAX_N];
int maxCost[MAX_N][MAX_N]; // the maximum cost among all edges on Path from vertex i to vertex j

class Comp
{
public:
    bool operator()(const Element &lhs, const Element &rhs) const 
    {
        if (lhs.first == rhs.first) // break tie using star importance
            return lhs.second < rhs.second; // prefer greater vertex
        else
            return lhs.first > rhs.first; // min heap
    }
};

int prim(vector<int>& pred, int n, int source)
{
    int sum = 0; // edges sum

    priority_queue<Element, vector<Element>, Comp > heap;
    vector<bool> known(n, false);
    pred.resize(n, UNKNOWN); // re-init
    vector<int> knownVerts;
    knownVerts.reserve(n);

    heap.push(make_pair(0, source));
    while (!heap.empty())
    {
        int v1_cost = heap.top().first;
        int v1 = heap.top().second; heap.pop();

        if (known[v1])
            continue;
        known[v1] = true;
        sum += v1_cost;
        
        for (int i = 0; i < knownVerts.size(); i++)
        {
            int &v_known = knownVerts[i];
            // no need to check for source vert
            if (v_known != pred[v1]) // to reach v1, reach his father first
                maxCost[v_known][v1] = maxCost[v1][v_known] = std::max(v1_cost, maxCost[v_known][ pred[v1] ]);
            else
                maxCost[v_known][v1] = maxCost[v1][v_known] = v1_cost;
        }
        knownVerts.push_back(v1);

        for (int v2 = 0; v2 < n; v2++) // all neighbors
        {
            if (v2 == v1 || known[v2]) 
                continue;
            
            if (pred[v2] == UNKNOWN || cost[v1][v2] < cost[ pred[v2] ][ v2 ])
            {
                pred[v2] = v1;
                heap.push(make_pair(cost[v1][v2], v2));
            }
        }
    }
    
    return sum;
}

void testcase()
{
    int n, source;
    scanf("%d%d", &n, &source);
    source--; // we use 0-based index

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
        {
            scanf("%d", &cost[i][j]);
            cost[j][i] = cost[i][j];
        }
    
    vector<int> pred;
    int mstCost = prim(pred, n, source);

    int minCost = MAX_COST;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) // consider add edge (i, j)
        {
            if (pred[i] == j || pred[j] == i)
                continue;
            int thisCost = cost[i][j] - maxCost[i][j];
            if ( thisCost < minCost )
                minCost = thisCost;
        }
    
    printf("%d\n", mstCost + minCost);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}