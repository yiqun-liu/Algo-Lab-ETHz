#include <cstdio>
#include <vector>
#include <map> // used as heap
#include <limits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

using std::vector; using std::multimap; using std::make_pair;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int,K> Fb; // Face with index
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::All_faces_iterator Face_iterator;
typedef Triangulation::Face_handle Face;

const double UNKNOWN = -1;

void calcThreshold(Triangulation& t, int nFaces, vector<double>& threshold)
{   
    vector<bool> known(nFaces, false);
    multimap<double, Face, std::greater<double> > heap;
    
    // Set infinite faces threshold to infinite
    const double INFINITE = std::numeric_limits<double>::max();
    Triangulation::Face_circulator f = t.incident_faces(t.infinite_vertex());
    do {
        threshold[f->info()] = INFINITE;
        heap.insert( make_pair(INFINITE, f) );
    } while (++f != t.incident_faces(t.infinite_vertex()));

    // Dijkstra like algorithm
    while (!heap.empty())
    {
        // pop biggest
        double bound = heap.begin()->first;
        Face f = heap.begin()->second;
        heap.erase(heap.begin());

        if (known[f->info()]) // redundant item in our heap
            continue;

        known[f->info()] = true; // mark as known

        for (int i = 0; i < 3; i++) // neighbors
        {
            Face neighbor = f->neighbor(i);
            int faceIdx = neighbor->info();
            
            if (known[faceIdx])
                continue;

            K::Point_2 p1 = f->vertex((i + 1) % 3)->point();
            K::Point_2 p2 = f->vertex((i + 2) % 3)->point();
            double newBound = CGAL::squared_distance(p1, p2) / 4.0;
            newBound = std::min(bound, newBound);

            // allow duplicate
            if (threshold[faceIdx] == UNKNOWN || newBound > threshold[faceIdx])
            {
                heap.insert( make_pair(newBound, neighbor) );
                threshold[faceIdx] = newBound;
            }
        }
    }
}

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

    // Assign index to faces
    int nFaces = 0;
    for (Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f)
        f->info() = nFaces++;
    
    vector<double> threshold(nFaces, UNKNOWN); // upper bound of acceptable d
    calcThreshold(t, nFaces, threshold);

    int m;
    scanf("%d", &m);
    for (int i = 0; i < m; i++)
    {
        double x, y, d;
        scanf("%lf%lf%lf", &x, &y, &d);
        K::Point_2 location(x, y);

        Triangulation::Vertex_handle v = t.nearest_vertex(location);
        Triangulation::Point nearest = v->point();
        if(CGAL::squared_distance(nearest, location) < d) 
        {
            printf("n");
            continue;
        }

        int faceIdx = t.locate(location)->info();
        if (d <= threshold[faceIdx])
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