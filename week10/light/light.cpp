#include <cstdio>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using std::vector; using std::make_pair;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K; // OVERFLOW?
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef std::pair<K::Point_2, double> Player; // position, radius

class Lamp
{
public:
    double x;
    double y;
    int index;

    int operator< (const Lamp& lamp)
    {
        return this->x < lamp.x;
    }
};

void testcase()
{
    int nPlayers, nLamps;
    scanf("%d%d", &nPlayers, &nLamps);

    vector<Player> players;
    players.reserve(nPlayers);
    for (int i = 0; i < nPlayers; i++)
    {
        double x, y, r;
        scanf("%lf%lf%lf", &x, &y, &r);
        players.push_back(make_pair(K::Point_2(x, y), r));
    }

    double radius;
    scanf("%lf", &radius); // radius of lighted area
    const double squared_radius = radius * radius;

    vector<K::Point_2> points;
    vector<Lamp> lamps;
    points.reserve(nLamps);
    for (int i = 0; i < nLamps; i++)
    {
        Lamp lamp;
        scanf("%lf%lf", &lamp.x, &lamp.y);
        points.push_back(K::Point_2(lamp.x, lamp.y));
        
        lamp.index = i;
        lamps.push_back(lamp);
    }

    Triangulation t;
    t.insert(points.begin(), points.end());

    bool survivorExist = false;
    for (int i = 0; i < nPlayers; i++)
    {
        K::Point_2 &player = players[i].first;
        double &radius_player = players[i].second;
        double threshold = (radius_player + radius) * (radius_player + radius);

        K::Point_2 &lamp = t.nearest_vertex(player)->point();
        if (CGAL::squared_distance(player, lamp) >= threshold)
        {
            survivorExist = true;
            printf("%d ", i);
        }
    }

    if (survivorExist)
    {
        putchar('\n');
        return;
    }

    std::sort(lamps.begin(), lamps.end());

    int lastElimination = -1;
    vector<int> survivors;
    for (int i = 0; i < nPlayers; i++)
    {
        K::Point_2 &player = players[i].first;
        double &radius_player = players[i].second;

        int px = player.x(), py = player.y();
        double r = radius_player + radius, r_squared = r * r;
        double lower = px - r, upper = px + r;
        
        Lamp temp;
        temp.x = lower;
        vector<Lamp>::iterator it = std::lower_bound(lamps.begin(), lamps.end(), temp);
        
        int cause = nLamps; // end of the game
        for (; it != lamps.end() && it->x <= upper; it++)
        {
            double dist_squared = (px - it->x) * (px - it->x) + (py - it->y) * (py - it->y);

            if ( dist_squared < r_squared && it->index < cause )
                cause = it->index; // look for the first lamp
        }
        
        if (cause > lastElimination)
        {
            lastElimination = cause;
            survivors.clear();
            survivors.push_back(i);
        }
        else if (cause == lastElimination)
            survivors.push_back(i);
    }
    for (int i = 0; i < survivors.size(); i++)
        printf("%d ", survivors[i]);
    putchar('\n');
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}
