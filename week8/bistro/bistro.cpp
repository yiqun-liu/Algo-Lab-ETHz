#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase(int n)
{
    // Process existing points
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for (int i = 0; i < n; i++)
    {
        int x, y;
        std::cin >> x >> y;
        pts.push_back(K::Point_2(x, y));
    }

    // Generate Delaunay triangulation (used as Voronoi diagram)
    Triangulation t;
    t.insert(pts.begin(), pts.end()); // O(N*logN)

    // Queries
    int m;
    std::cin >> m;
    for (int i = 0; i < m; i++)
    {
        int x, y;
        std::cin >> x >> y;
        K::Point_2 p(x, y);
        Triangulation::Vertex_handle v = t.nearest_vertex(p); // O(logN)
        std::cout << CGAL::squared_distance(v->point(), p) << std::endl;
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0); // No scientific notation
    
    int n;
    std::cin >> n;
    while (n)
    {
        testcase(n);
        std::cin >> n;
    }
    return 0;
}
