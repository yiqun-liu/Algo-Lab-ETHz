/**
 * 2-colorable is much easier AFTER we decides connectivity (i.e. connected components)
 */
#include <cstdio>
#include <vector>
#include <stack>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using std::vector; using std::pair; using std::make_pair; using std::stack;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb; // with index
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef pair<K::Point_2, int> P;

const int UNKNOWN = -1;

bool hasSafeLowerBound(Triangulation& t, double threshold)
{
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        if (t.segment(e).squared_length() <= threshold)
            return false; // lower bound <= threshold
    return true; // lower bound > threshold
}

bool dfs(vector<int>& component, vector<int>& label, vector<vector<int> >& adjList, 
    vector<P>& points, double range_2, int source, int mark)
{
    stack<int> s;
    vector<K::Point_2> pts[2];

    component[source] = mark;
    label[source] = 0;
    pts[0].push_back(points[source].first); // no need to keep info

    s.push(source);
    while (!s.empty()) // now start iterative dfs
    {
        const int v = s.top(), newLabel = 1 - label[v];
        s.pop();

        for (int i = 0; i < adjList[v].size(); i++)
        {
            int &neighbor = adjList[v][i];
            if (component[neighbor] == UNKNOWN)
            {
                component[neighbor] = mark;
                label[neighbor] = newLabel;
                pts[newLabel].push_back( points[neighbor].first );
                s.push(neighbor);
            }
            else if (label[v] == label[neighbor]) // unable to assign frequencies
                return false;
        }
    }
    // Triangulation only contains a subset of edges whose length <= r.
    // It is sufficient for us to assign colors, but does not guarantee correctness
    // we still need to check if there are length-violation within each color
    // core trick: use triangulation to check the lower bound: n^2 -> nlogn
    Triangulation t1(pts[0].begin(), pts[0].end()), t2(pts[1].begin(), pts[1].end());
    return hasSafeLowerBound(t1, range_2) && hasSafeLowerBound(t2, range_2);
}

void testcase()
{
    int nStations, nClues;
    double range;
    scanf("%d%d%lf", &nStations, &nClues, &range);

    vector<P> points;
    points.reserve(nStations);
    for (int i = 0; i < nStations; i++)
    {
        double x, y;
        scanf("%lf%lf", &x, &y);
        points.push_back( make_pair(K::Point_2(x, y), i) );
    }

    Triangulation t;
    t.insert(points.begin(), points.end());

    const double range_2 = range * range;
    vector<vector<int> > adjList(nStations); // adjacency list of triangulation

    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
    {
        Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
        Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
        if ( CGAL::squared_distance(v1->point(), v2->point()) <= range_2 )
        {
            int v1_idx = v1->info(), v2_idx = v2->info();
            adjList[v1_idx].push_back(v2_idx); // allow redundancy
            adjList[v2_idx].push_back(v1_idx);
        }
    }
    int nComponents = 0;
    vector<int> component(nStations, UNKNOWN); // index of the connected components it belongs
    vector<int> label(nStations, UNKNOWN);
    bool noInterfere = true;
    
    for (int i = 0; i < nStations && noInterfere; i++)
        if (component[i] == UNKNOWN)
            noInterfere = dfs(component, label, adjList, points, range_2,  i, nComponents++);
    
    for (int i = 0; i < nClues; i++)
    {
        double x1, y1, x2, y2;
        scanf("%lf%lf%lf%lf", &x1, &y1, &x2, &y2);
        if (!noInterfere)
        {
            putchar('n');
            continue;
        }

        K::Point_2 p1(x1, y1), p2(x2, y2);
        if (CGAL::squared_distance(p1, p2) <= range_2) // directly connected
            putchar('y');
        else // indirectly connected
        {
            Triangulation::Vertex_handle v1 = t.nearest_vertex(p1);
            Triangulation::Vertex_handle v2 = t.nearest_vertex(p2);

            bool connected = 
                CGAL::squared_distance(p1, v1->point()) <= range_2 && 
                CGAL::squared_distance(p2, v2->point()) <= range_2 && 
                component[v1->info()] == component[v2->info()];
            
            if (connected)
                putchar('y');
            else
                putchar('n');
        }
    }
    putchar('\n');
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}