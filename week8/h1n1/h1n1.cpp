#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h> // need to add index info for each face

// https://judge.inf.ethz.ch/doc/algolab/cgal/doc_html/Triangulation_2/Triangulation_2_2colored_face_8cpp-example.html

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb; // index
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef Triangulation::Finite_faces_iterator Finite_faces_iterator;
typedef Triangulation::All_faces_iterator All_faces_iterator;

typedef std::vector< std::pair<int, double> > Neighbor;
const double UNKNOWN = -1;
// const int INFINITY_FACE = 0;

 // Dijkstra-like algorithm
void calcSafeDist( std::vector<double>& safeDist, std::vector< Neighbor >& faceGraph, 
    std::vector<int> infinity_faces )
{
    std::multimap< double, int, std::greater<double> > maxHeap; // CAREFUL: redundant key
    const double infinity = std::numeric_limits<double>::max();
    for (std::vector<int>::iterator it = infinity_faces.begin(); it != infinity_faces.end(); it++)
    {
        safeDist[*it] = infinity;
        maxHeap.insert(std::make_pair(infinity, *it));
    }

    while(!maxHeap.empty())
    {
        int from = maxHeap.begin()->second; // heap.top
        maxHeap.erase(maxHeap.begin());
        for (Neighbor::iterator it = faceGraph[from].begin(); it != faceGraph[from].end(); it++)
        {
            int to = it->first;
            if (safeDist[to] != UNKNOWN)
            {
                double newSafeDist =  std::min(safeDist[from], it->second); // minimum - bottleneck
                if (newSafeDist > safeDist[to]) // update key
                {
                    // maxHeap.erase( safeDist[to] );
                    std::multimap< double, int, std::greater<double> >::iterator 
                        it_to_erase = maxHeap.find(safeDist[to]);
                    while (it_to_erase->second != to)
                        it_to_erase++;
                    maxHeap.erase( it_to_erase );

                    safeDist[to] = newSafeDist;
                    maxHeap.insert( std::make_pair(safeDist[to], to) );
                }
            }
            else
            {
                safeDist[to] = std::min(safeDist[from], it->second);
                maxHeap.insert( std::make_pair(safeDist[to], to) );
            }
        }
    }
}

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

    // Assign index to each faces
    int faceCount = 0; // the infinite one counted
    for (All_faces_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f)
        f->info() = faceCount++;
    
    // Pre-processing: run Dijkstra-like algorithm to find max acceptable d for escaping
    // Construct graph
    std::vector< Neighbor > faceGraph(faceCount);
    std::vector< bool > accessed(faceCount, false);
    std::vector< int > infinity_faces;
    for (All_faces_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f)
    {
        int f1 = f->info();
        if (t.is_infinite(f))
        {
            infinity_faces.push_back(f1);
            continue;
        }
        for (int i = 0; i < 3; i++) // deal with three neighbors
        {
            int f2 = f->neighbor(i)->info();
            // std::cout << f1 << " & " << f2 << ":\n";
            if (accessed[f2])
                continue;
            
            K::Point_2 p1 = f->vertex((i + 1) % 3)->point();
            K::Point_2 p2 = f->vertex((i + 2) % 3)->point();
            double dist = CGAL::to_double(CGAL::squared_distance(p1, p2) / 4.0); // (d / 2) ^ 2
            // std::cout << f1 << " & " << f2 << ":\t" << dist << std::endl;
            faceGraph[f1].push_back(std::make_pair(f2, dist));
            faceGraph[f2].push_back(std::make_pair(f1, dist));
        }
        accessed[f1] = true;
    }

    std::vector< double > safeDist(faceCount, UNKNOWN);
    calcSafeDist(safeDist, faceGraph, infinity_faces);

    int m;
    std::cin >> m;
    for (int i = 0; i < m; i++)
    {
        int x, y;
        double distance;

        std::cin >> x >> y >> distance;
        K::Point_2 p(x, y);

        Triangulation::Vertex_handle v = t.nearest_vertex(p); // O(logN)
        Triangulation::Point p0 = v->point();
        if(CGAL::to_double(CGAL::squared_distance(p, p0)) < distance) 
        {
            std::cout << "n";
            continue;
        }
        
        int face = t.locate(p)->info();
        if (safeDist[face] >= distance)
            std::cout << "y";
        else
            std::cout << "n";
    }
    std::cout << std::endl;
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