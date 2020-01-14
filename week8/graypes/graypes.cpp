#include <cstdio>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using std::vector;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase(int n)
{
    vector<K::Point_2> points;
    for (int i = 0; i < n; i++)
    {
        double x, y;
        scanf("%lf%lf", &x, &y);
        points.push_back( K::Point_2(x, y) );
    }

    Triangulation t;
    t.insert( points.begin(), points.end() );

    double minDist = -1; // squared
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
    {
        double dist = t.segment(e).squared_length();
        if (dist < minDist || minDist == -1)
            minDist = dist;
    }
    
    int time = (int)ceil( sqrt(minDist) * 100 / 2 ); // must be mutual; second -> hundredth of second
    printf("%d\n", time);
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