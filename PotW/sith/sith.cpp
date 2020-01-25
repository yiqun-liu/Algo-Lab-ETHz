#include <cstdio>
#include <vector>
#include <cstring>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using std::vector; using std::pair; using std::make_pair;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

const int MAX_N = 30000;
int pred[MAX_N];

void initSets(int n) { memset(pred, -1, sizeof(int) * n); } 
int getRoot( int x )
{
    if (pred[x] < 0) // for root, we store negated number of elements in this set
        return x;
    return pred[x] = getRoot( pred[x] );
}
int merge(int a, int b)
{
    int root_a = getRoot(a), root_b = getRoot(b);
    if (root_a != root_b)
    {
        pred[root_b] += pred[root_a];
        pred[root_a] = root_b;
    }
    return -pred[root_b]; // numebr of elements
}

typedef pair<K::Point_2, int> MyPoint;
void testcase()
{
    int n;
    double r;
    scanf("%d%lf", &n, &r);

    const double r_2 = r * r;
    vector<K::Point_2> planets;

    planets.reserve(n);
    for (int i = 0; i < n; i++)
    {
        double x, y;
        scanf("%lf%lf", &x, &y);
        planets.push_back(K::Point_2(x, y));
    }
    
    Triangulation t;
    initSets(n); // Initialize union-find set
    
    // Manually insert the last one, or vertex circulator cannot work
    t.insert( planets[n - 1] )->info() = n - 1;
    int maxUnion = 1, maxSize = 1;
    for (int i = 2; i <= n; i++) // when there are i planets left
    {
        int v_index = n - i;
        Triangulation::Vertex_handle v0 = t.insert( planets[v_index] );
        v0->info() = v_index;
        Triangulation::Vertex_circulator v = t.incident_vertices(v0), end = v;
        do 
        {
            if (!t.is_infinite(v)) 
            {
                if (CGAL::squared_distance(v0->point(), v->point()) <= r_2)
                {
                    int size = merge(v_index, v->info());    // r-connected component
                    maxSize = std::max(size, maxSize);       // size/reachability limit
                    int current = std::min(n - i, maxSize);  // time limit: enemy captured no less than us
                    maxUnion = std::max(current, maxUnion);
                }
            }
        } while (++v != end);
    }

    printf("%d\n", maxUnion);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}