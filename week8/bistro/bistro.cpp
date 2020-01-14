#include <cstdio>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using std::vector;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

void testcase(int n)
{
    vector<K::Point_2> points;
    points.reserve(n);

    for (int i = 0; i < n; i++)
    {
        double x, y;
        scanf("%lf%lf", &x, &y);
        points.push_back(K::Point_2(x, y));
    }

    Triangulation t;
    t.insert(points.begin(), points.end());

    int m;
    scanf("%d", &m);
    for (int i = 0; i < m; i++)
    {
        double x, y;
        scanf("%lf%lf", &x, &y);
        K::Point_2 location(x, y);
        K::Point_2 nearest = t.nearest_vertex(location)->point();
        double squaredDist = CGAL::squared_distance(location, nearest);
        printf("%.0f\n", squaredDist);
    }
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