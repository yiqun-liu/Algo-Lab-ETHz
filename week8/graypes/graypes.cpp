#include <iostream>
#include <vector>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
// #include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h> // TL, use 

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase(int n)
{
    // 'graypes' position
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

    Edge_iterator e = t.finite_edges_begin();
    K::FT minLength = t.segment(e).squared_length(), length;
    for (e++; e != t.finite_edges_end(); ++e)
    {
        length = t.segment(e).squared_length();
        if (length < minLength)
            minLength = length;
    }
    std::cout << ceil(sqrt(CGAL::to_double(minLength)) * 100 / 2.0) << "\n";
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
