#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int increment( std::vector<std::vector<int> >& commons, int v1, int v2 )
{
    if (v1 < v2)
        return ++commons[v1][v2];
    else
        return ++commons[v2][v1];
}

int maximum_matching(int n, const graph &G) {
    std::vector<vertex_desc> mate_map(n);
    const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

    boost::edmonds_maximum_cardinality_matching(G,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    
    // return matchin size
    return boost::matching_size(G,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
}

void testcase()
{
    int n, c, f;
    std::cin >> n >> c >> f;

    int threshold = f + 1;
    // number of characteristics in common
    std::vector<std::vector<int> > commons(n, std::vector<int>(n, 0));
    // indexed by feature, lists of student have this characteristics
    std::unordered_map<std::string, std::vector<int> > students;

    graph G(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < c; j++)
        {
            std::string charateristic;
            std::cin >> charateristic;
            
            std::unordered_map<std::string, std::vector<int> >::iterator it = students.find(charateristic);
            if (it == students.end()) // first met this characteristic
                students.insert( std::make_pair(charateristic, std::vector<int>(1, i)) );
            else
            {
                for (int k = 0; k < it->second.size(); k++)
                {
                    // only add to graph when they first reach threshold
                    if (increment(commons, i, it->second[k]) == threshold)
                        boost::add_edge(i, it->second[k], G);
                }
                it->second.push_back(i);
            }
        }
    }

    if (maximum_matching(n, G) == n / 2)
        printf("not optimal\n");
    else
        printf("optimal\n");
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) testcase();
    return 0;    
}