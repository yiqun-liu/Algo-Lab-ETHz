#include <cstdio>
#include <vector>

struct StructNeighbor
{
    int to;
    double weight;
};
typedef struct StructNeighbor Neighbor;

typedef std::vector<Neighbor> AdjList;
typedef std::vector<AdjList> Graph;

const int MAX_VERTICES = 1000, MAX_MOVES = 4000;
double gain[MAX_VERTICES][MAX_MOVES];

void testcase()
{
    int nV, nE, nMoves;
    double score;
    scanf("%d%d%lf%d", &nV, &nE, &score, &nMoves);

    Graph graph(nV);
    for (int i = 0; i < nE; i++)
    {
        int from, to;
        double point;
        Neighbor temp;
        scanf("%d%d%lf", &from, &to, &point);
        temp.to = to;
        temp.weight = point;

        graph[from].push_back(temp);
        // gain[from][0] = 0
    }

    // TODO binary search
    for (int k = 1; k <= nMoves; k++)
    {
        for (int from = 0; from < nV; from++)
        {
            if (graph[from].size() == 0)
            {
                gain[from][k] = gain[0][k];
                continue;
            }
            
            double maxEarn = 0;
            for (int i = 0; i < graph[from].size(); i++)
            {
                double earn = graph[from][i].weight;
                if (k > 1)
                    earn += gain[ graph[from][i].to ][k - 1];
                if (earn > maxEarn)
                    maxEarn = earn;
            }
            gain[from][k] = maxEarn;
            
            if (from == 0 && maxEarn >= score)
            {
                printf("%d\n", k);
                return;
            }
        }
    }
    printf("Impossible\n");
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}