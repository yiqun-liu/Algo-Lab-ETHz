#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h> // need to add index info on vertex

// https://doc.cgal.org/latest/Triangulation_2/Triangulation_2_2info_insert_with_pair_iterator_2_8cpp-example.html#_a1
// https://doc.cgal.org/latest/Spatial_sorting/index.html#title4

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

inline double getTime(K::FT squaredDist)
{
    // 2 * t^2 + 1 = d
    double dist = sqrt(CGAL::to_double(squaredDist));
    if (dist <= 1) return 0;
    return ceil(sqrt((dist - 1) / 2));
}

void testcase(int n)
{
    // Process existing points
    int l, b, r, t;
    std::cin >> l >> b >> r >> t;

    std::vector<K::Point_2> pts;
    std::vector<K::FT> minDist; // squared distance
    pts.reserve(n);
    for (int i = 0; i < n; i++)
    {
        int x, y;
        std::cin >> x >> y;
        pts.push_back(K::Point_2(x, y));
    }

    Triangulation triangle;
    // Generate Delaunay triangulation robustly
    CGAL::spatial_sort(pts.begin(),pts.end());
    for (int i = 0; i < n; i++)
    {
        K::FT x = pts[i].x(), y = pts[i].y();
        K::FT dist = x - l; // dist to boundary
        dist = std::min(dist, r - x);
        dist = std::min(dist, y - b);
        dist = std::min(dist, t - y);
        
        Triangulation::Vertex_handle v = triangle.insert(pts[i]);
        v->info() = i; // index for later reference
        minDist.push_back(dist * 2 * dist * 2); // avoid sqrt
    }

    for (Edge_iterator e = triangle.finite_edges_begin(); e != triangle.finite_edges_end(); ++e)
    {
        K::FT squaredDist = triangle.segment(e).squared_length();
        Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
        Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
        int idx1 = v1->info(), idx2 = v2->info();
        minDist[idx1] = std::min(minDist[idx1], squaredDist);
        minDist[idx2] = std::min(minDist[idx2], squaredDist);
    }

    std::sort(minDist.begin(), minDist.end());
    std::cout << getTime(minDist[0]) << ' ' << getTime(minDist[n / 2]) 
            << ' ' << getTime(minDist[n - 1]) << std::endl;
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
