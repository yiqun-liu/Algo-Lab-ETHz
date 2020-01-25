/** 
 * My intial thoughts is that the proper way to denote an alternative trip is 
 * by its lowest node, as in trees parent is unique while children are not.
 * However, the data range is so demanding that we must avoid any recalculating
 * the stats of common ancestors. DFS is the natural choice
 * 
 * Share some similarities with problem "Evolution"
 * 
 * I used multiset for productivity. I believe it can be improved by "sliding 
 * min" technique: 
 * https://people.cs.uct.ac.za/~ksmith/articles/sliding_window_minimum.html
 */
#include <cstdio>
#include <vector>
#include <stack>
#include <set>

using std::vector; using std::set; using std::multiset; using std::stack;
using std::pair; using std::make_pair;

void testcase()
{
    int n, length, maxDiff;
    scanf("%d%d%d", &n, &length, &maxDiff);

    vector<int> temperature;
    temperature.reserve(n);

    for (int i = 0; i < n; i++)
    {
        int temp;
        scanf("%d", &temp);
        temperature.push_back(temp);
    }

    vector<vector<int> > child(n);
    for (int i = 1; i < n; i++)
    {
        int from, to;
        scanf("%d%d", &from, &to);
        child[from].push_back(to);
    }

    // DFS
    multiset<int> tripTemps;
    vector<int> indices;
    set<int> validSource; // results

    const int IN = 0, OUT = 1;
    typedef pair<int, int> State;
    stack<State> stk;
    stk.push(make_pair(0, IN));
    while (!stk.empty())
    {
        State state = stk.top();
        stk.pop();

        int &index = state.first;
        if (state.second == IN) // first visit
        {
            stk.push(make_pair(index, OUT));
            tripTemps.insert( temperature[index] );
            indices.push_back( index );

            int depth = indices.size();
            if (depth >= length) // target length = 0?
            {
                if (depth > length)
                {
                    int toDelete = indices[depth - length - 1]; // source's parent
                    tripTemps.erase( tripTemps.find( temperature[toDelete] ) );
                }
                int diff = *tripTemps.rbegin() - *tripTemps.begin();
                if (diff <= maxDiff)
                    validSource.insert( indices[depth - length] ); // source
            }

            for (int i = 0; i < child[index].size(); i++)
                stk.push( make_pair(child[index][i], IN) );
        }
        else // second visit: restore intermediate vector
        {
            int depth = indices.size();
            tripTemps.erase( tripTemps.find(temperature[index]) );
            indices.pop_back();

            if (depth > length)
            {
                int deleted = indices[depth - length - 1];
                tripTemps.insert( temperature[deleted] );
            }
        }
    }

    for (set<int>::iterator it = validSource.begin(); it != validSource.end(); it++)
        printf("%d ", *it);
    if (validSource.empty())
        printf("Abort mission");    
    putchar('\n');
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}