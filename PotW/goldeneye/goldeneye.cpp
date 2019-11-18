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

const int MAX_N = 30000;
int father[MAX_N], a_father[MAX_N], b_father[MAX_N];

int getRoot(int x, int *father)
{
    if (father[x] == x)
        return x;
    
    return father[x] = getRoot(father[x], father); // compress
}

void merge(int x, int y, int *father)
{
    x = getRoot(x, father);
    y = getRoot(y, father);
    if (x != y)
        father[y] = x;
}

struct StructEdge
{
    double squaredLength;
    int a;
    int b;
};
typedef struct StructEdge MyEdge;

bool compareMyEdge(const MyEdge& e1, const MyEdge& e2)
{ return e1.squaredLength < e2.squaredLength; }

void testcase()
{
    int n, m;
    double p;
    std::cin >> n >> m >> p;

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
    CGAL::spatial_sort(pts.begin(),pts.end());
    for (int i = 0; i < n; i++)
    {
        Triangulation::Vertex_handle v = t.insert(pts[i]);
        v->info() = i; // index for later reference
    }

    // Preprocessing: vertices which are reachable from each other are grouped together
    // union find
    for (int i = 0; i < n; i++)
        father[i] = a_father[i] = b_father[i] = i;
    
    std::vector<MyEdge> edges;
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
    {
        double squaredDist = CGAL::to_double(t.segment(e).squared_length());
        
        Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3),
                                     v2 = e->first->vertex((e->second + 2) % 3);
        int idx1 = v1->info(), idx2 = v2->info();
        
        if (squaredDist <= p) // reachable
            merge(idx1, idx2, father);

        MyEdge myEdge;
        myEdge.a = idx1; myEdge.b = idx2; myEdge.squaredLength = squaredDist;
        edges.push_back(myEdge);
    }
    std::sort(edges.begin(), edges.end(), compareMyEdge);

    int a_idx = 0, b_idx = 0;
    double a_d0 = 0, b_d0 = 0;
    for (int i = 0; i < m; i++)
    {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        K::Point_2 p1(x1, y1), p2(x2, y2);

        Triangulation::Vertex_handle v1 = t.nearest_vertex(p1), v2 = t.nearest_vertex(p2);
        Triangulation::Point j1 = v1->point(), j2 = v2->point();
        double squared_2d = 4 * std::max(
            CGAL::to_double(CGAL::squared_distance(p1, j1)), 
            CGAL::to_double(CGAL::squared_distance(p2, j2)));
        
        bool possible = false;
        int idx1 = v1->info(), idx2 = v2->info();
        if (squared_2d <= p) // able to reach nearest jammer
        {
            if (getRoot(idx1, father) == getRoot(idx2, father))
            {
                possible = true;
                
                // make it is also possible for b
                if (squared_2d > b_d0)
                    b_d0 = squared_2d;
                while (getRoot(idx1, b_father) != getRoot(idx2, b_father))
                {
                    const MyEdge& myEdge = edges[b_idx++]; // no need for guard
                    merge(myEdge.a, myEdge.b, b_father);
                }
            }
        }
        
        // make this mission possible for a anyway
        if (squared_2d > a_d0)
            a_d0 = squared_2d;
        while (getRoot(idx1, a_father) != getRoot(idx2, a_father))
        {
            const MyEdge& myEdge = edges[a_idx++];
            merge(myEdge.a, myEdge.b, a_father);
        }

        if (possible)
            std::cout << "y";
        else
            std::cout << "n";
    }
    std::cout << std::endl;
    
    double a = a_idx == 0 ? a_d0 : std::max(a_d0, edges[a_idx - 1].squaredLength);
    double b = b_idx == 0 ? b_d0 : std::max(b_d0, edges[b_idx - 1].squaredLength);
    std::cout << a << std::endl;
    std::cout << b << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0); // No scientific notation
    
    int t;
    std::cin >> t;
    while(t--) testcase();
    return 0;
}