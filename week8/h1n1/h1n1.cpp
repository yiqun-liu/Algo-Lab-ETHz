#include <cstdio>
#include <vector>
#include <queue> // use priority_queue as heap
#include <limits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

using std::vector; using std::priority_queue; using std::make_pair; using std::pair;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<double,K> Fb; // Face with index
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::All_faces_iterator Face_iterator;
typedef Triangulation::Face_handle Face_handle;

void testcase(int n)
{
    vector<K::Point_2> points;
    points.reserve(n);

    for (int i = 0; i < n; i++)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        points.push_back(K::Point_2(x, y));
    }

    Triangulation t;
    t.insert(points.begin(), points.end());

    typedef pair<double, Face_handle> Element;
    priority_queue<Element, vector<Element> > heap; // key: safe distance, value: face
    const double INFINITE = std::numeric_limits<double>::max(), UNKNOWN = -1;
    for (Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f)
    {
        f->info() = UNKNOWN;
        if (t.is_infinite(f))
            heap.push(make_pair(INFINITE, f));
    }

    // Dijkstra like, greedy algorithm
    while (!heap.empty())
    {
        double bound_from = heap.top().first;
        Face_handle from = heap.top().second;
        heap.pop();

        if (from->info() != UNKNOWN) // redundant element
            continue;
        
        from->info() = bound_from;

        for (int i = 0; i < 3; i++) // three neighbor
        {
            Face_handle to = from->neighbor(i);
            // we must exclude infinite edge, or we cannot calc the length
            if (to->info() != UNKNOWN || t.is_infinite(to) )
                continue;
            
            double bound_to = std::min(bound_from, t.segment(from, i).squared_length() / 4.0);
            heap.push( make_pair(bound_to, to) ); // we allow duplicates
        }   
    }

    int m;
    scanf("%d", &m);
    for (int i = 0; i < m; i++)
    {
        double x, y, d;
        scanf("%lf%lf%lf", &x, &y, &d);
        K::Point_2 location(x, y);
        Triangulation::Point nearest = t.nearest_vertex(location)->point();

        if (d > CGAL::squared_distance(nearest, location)) 
        {
            printf("n");
            continue;
        }

        double safeDistance = t.locate(location)->info();
        if (d <= safeDistance)
            printf("y");
        else
            printf("n");
    }
    putchar('\n');
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