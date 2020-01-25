#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>
#include <limits>

using std::vector; using std::priority_queue; using std::make_pair;
typedef std::pair<int, int> Element;

const int MAX_N = 3000, UNKNOWN = -1;
int cost[MAX_N][MAX_N];
int root[MAX_N];

// int getCost(int v1, int v2) { return v1 < v2 ? cost[v1][v2] : cost[v2][v1]; }
class Comp{
public:
    bool operator()(const Element &lhs, const Element &rhs) const 
    {
        if (lhs.first == rhs.first)
            return lhs.second < rhs.second; // prefer important stars
        else
            return lhs.first > rhs.first;
    }
};

void prim(vector<int>& pred, int source, int n)
{
    priority_queue<Element, vector<Element>, Comp> Q;
    vector<bool> done(n, false);
    
    pred[source] = source;

    Q.push( make_pair(0, source) );
    while (!Q.empty())
    {
        int v1 = Q.top().second; Q.pop();
        if (done[v1])
            continue;
        done[v1] = true;

        for (int v2 = 0; v2 < n; v2++) // all neighbors
        {
            if (v1 == v2 || done[v2]) continue;
            
            if ( pred[v2] == UNKNOWN || cost[ pred[v2] ][v2] > cost[v1][v2] )
            {
                pred[v2] = v1;
                Q.push(make_pair(cost[v1][v2], v2));
            }
        }
    }
}

void initSet(int n)
{
    memset(root, UNKNOWN, sizeof(int) * n);
}

int getRoot(int a)
{
    if (root[a] == UNKNOWN) // is root
        return a;
    return root[a] = getRoot(root[a]);
}

void unionSet(int a, int b)
{
    int rootA = getRoot(a), rootB = getRoot(b);
    if (rootA != rootB)
        root[rootA] = rootB;
}

void testcase()
{
    int n, source;
    scanf("%d%d", &n, &source);
    // printf("%d %d\n", n, source);
    --source; // it's one-based index

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
        {
            scanf("%d", &cost[i][j]);
            cost[j][i] = cost[i][j];
        }
    
    // Run prim
    vector<int> pred(n, UNKNOWN);
    prim(pred, source, n);

    
    int minCost = std::numeric_limits<int>::max();
    int sum = 0;
    for (int i = 0; i < n; i++) // the edge to change
    {
        
        sum += cost[i][pred[i]];
        if (i == source)
            continue;
        
        initSet(n);
        for (int j = 0; j < n; j++)
            if (j != i)
                unionSet( j, pred[j] );

        int leftRoot = getRoot(0);
        vector<int> left, right;
        for (int j = 0; j < n; j++)
            if (getRoot(j) == leftRoot)
                left.push_back(j);
            else
                right.push_back(j);
        
        int optimalCost = std::numeric_limits<int>::max();
        for (int j = 0; j < left.size(); j++)
            for (int k = 0; k < right.size(); k++)
            {
                int l = left[j], r = right[k];
                if (cost[l][r] < optimalCost && 
                    !( l == i && r == pred[i] ) && !( r == i && l == pred[i] ) )
                {
                    optimalCost = cost[l][r];
                }

            }
        optimalCost -= cost[i][pred[i]];

        if (optimalCost < minCost)
            minCost = optimalCost;
    }
    printf("%d\n", sum + minCost);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}