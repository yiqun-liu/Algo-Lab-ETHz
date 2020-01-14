#include <cstdio>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using std::vector; using std::pair; using std::make_pair;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

double min2(double n1, double n2) { return n1 < n2 ? n1 : n2; }
double min4(double n1, double n2, double n3, double n4)
{
    return min2( min2(n1, n2), min2(n3, n4) );
}
int getTime(double squaredDist)
{
    return ceil(sqrt( (sqrt(squaredDist) - 1) * 0.5 ));
}

typedef pair<K::Point_2, int> P;
void testcase(int n)
{
    // size of the dish
    int l, b, r, t;
    scanf("%d%d%d%d", &l, &b, &r, &t);
    
    vector<P> points;
    vector<double> minDist; // store squared min distance
    points.reserve(n);
    minDist.reserve(n);

    for (int i = 0; i < n; i++)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        points.push_back( make_pair( K::Point_2(x,y), i ) ); // index i
        minDist.push_back( min4( x - l, r - x, y - b, t - y ) );
        // it is (2d)^2: we will calculate time as if the distance is 
        // shortened from two ends, so we doubled d between germs and boundaries
        minDist[i] *= minDist[i] * 4; 
    }

    Triangulation triangle;
    triangle.insert(points.begin(), points.end());

    for (Edge_iterator e = triangle.finite_edges_begin(); e != triangle.finite_edges_end(); ++e)
    {
        int v1_idx = e->first->vertex((e->second + 1) % 3)->info();
        int v2_idx = e->first->vertex((e->second + 2) % 3)->info();
        double dist = triangle.segment(e).squared_length();
        minDist[v1_idx] = min2(minDist[v1_idx], dist);
        minDist[v2_idx] = min2(minDist[v2_idx], dist);
    }
    
    std::sort(minDist.begin(), minDist.end());
    
    printf("%d %d %d\n", getTime(minDist[0]), getTime(minDist[n / 2]), getTime(minDist[n - 1]));
}

 int main()
 {
    while (true)
    {
        int n;
        scanf("%d", &n);
        if (n)
            testcase(n);
        else
            break;
    }
    return 0;
 }