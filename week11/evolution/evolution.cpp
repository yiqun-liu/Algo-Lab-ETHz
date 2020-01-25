// idea from official solutions
// It's natural to consider binary search, but the thoughts of construct all path in O(N) time...
// I have to say it is out of my reach

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cin; using std::cout; using std::endl;
using std::unordered_map; using std::string; using std::vector; using std::make_pair;

typedef std::pair<int, int> Query; // max age, number of output

void dfs(vector<int>& results, vector<vector<Query> >& queries, vector<vector<int> >& child, 
    vector<int>& ageStack, vector<int>& indexStack, vector<int>& ages, int index)
{
    // Path
    ageStack.push_back(ages[index]);
    indexStack.push_back(index);

    // age stack is in non-increasing order
    for (int i = 0; i < queries[index].size(); i++)
    {

        int &ageBound = queries[index][i].first, &queryIndex = queries[index][i].second;
        vector<int>::iterator it = std::lower_bound(ageStack.begin(), ageStack.end(), ageBound,
            std::greater<int>());
        // always safe, guaranteed by problem specification
        results[queryIndex] = indexStack[it - ageStack.begin()];
    }
    
    for (int i = 0; i < child[index].size(); i++)
        dfs(results, queries, child, ageStack, indexStack, ages, child[index][i]);
    
    ageStack.pop_back();
    indexStack.pop_back();
}

void testcase()
{
    int n, nQueries;
    cin >> n >> nQueries;

    vector<string> names(n);
    vector<int> ages(n);
    unordered_map<string, int> hashMap; // name -> index

    int root = 0;
    for (int i = 0; i < n; i++)
    {
        cin >> names[i] >> ages[i];
        hashMap.insert(make_pair(names[i], i));
        if (ages[i] > ages[root]) // max age --> root
            root = i;
    }
    
    // Construct graph
    vector<vector<int> > child(n); // adjacency list
    for (int i = 1; i < n; i++)
    {
        string s, p;
        cin >> s >> p;

        int childIdx  = hashMap.find(s)->second; // unchecked
        int fatherIdx = hashMap.find(p)->second;
        child[fatherIdx].push_back(childIdx);
    }

    vector<vector<Query> > queries(n); // grouped by species
    for (int i = 0; i < nQueries; i++)
    {
        string name;
        int upper_bound;
        cin >> name >> upper_bound;

        int idx = hashMap.find(name)->second;
        queries[idx].push_back( make_pair(upper_bound, i) );
    }

    vector<int> results(nQueries);
    vector<int> ageStack; 
    vector<int> indexStack;
    dfs(results, queries, child, ageStack, indexStack, ages, root);

    cout << names[results[0]];
    for (int i = 1; i < nQueries; i++)
        cout << ' ' << names[results[i]];
    cout << endl;
}

int main()
{
    int t;
    cin >> t;
    while(t--) testcase();
    return 0;
}
