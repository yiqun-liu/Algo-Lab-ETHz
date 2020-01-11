#include <cstdio>
#include <cstring>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// ---- Sample Code ----
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

class edge_adder {
    graph &G;
 public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, int capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};
// ---- Sample Code End ----

const int MAX_H = 1000, MAX_W = 1000, MAX_LENGTH = 1000000, N_LETTER = 26;
char front[MAX_H][MAX_W + 1], back[MAX_H][MAX_W + 1], note[MAX_LENGTH + 1];
int combCount[N_LETTER][N_LETTER], charCount[N_LETTER];

int alpha2num(char c) { return c - 'A'; }

void testcase()
{
    int h, w;
    scanf("%d%d", &h, &w);

    scanf("%s", note);

    for (int i = 0; i < h; i++)
        scanf("%s", front[i]);
    for (int i = 0; i < h; i++)
        scanf("%s", back[i]);
    
    // preprocess notes & cards to condense the network
    memset(charCount, 0, sizeof(int) * N_LETTER);
    memset(combCount, 0, sizeof(int) * N_LETTER * N_LETTER);
    
    // counting
    const int noteLength = strlen(note);
    for (int i = 0; i < noteLength; i++)
        charCount[ alpha2num( note[i] ) ]++;
    
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            char c1 = alpha2num(front[i][j]), c2 = alpha2num(back[i][w - 1 - j]);
            if (c1 <= c2)
                combCount[c1][c2]++;
            else
                combCount[c2][c1]++;
        }
    
    // Contruct network
    const int charBase = N_LETTER * N_LETTER, source = charBase + N_LETTER, target = source + 1;
    graph G(target + 1); // index assignment: letter combinations, note chars, source, target
    edge_adder adder(G);

    for (int i = 0; i < N_LETTER; i++)
    {
        for (int j = i; j < N_LETTER; j++) // we stored all data at place where j >= i
            if (combCount[i][j])
            {
                int combIndex = N_LETTER * i + j;
                adder.add_edge(source, combIndex, combCount[i][j]); // source to card
                adder.add_edge(combIndex, charBase + i, combCount[i][j]); // card to two letters
                adder.add_edge(combIndex, charBase + j, combCount[i][j]);
            }
        // note chars to target
        if (charCount[i])
            adder.add_edge(charBase + i, target, charCount[i]);
    }

    int flow = boost::push_relabel_max_flow(G, source, target);
    if (flow == noteLength)
        printf("Yes\n");
    else
        printf("No\n");
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}