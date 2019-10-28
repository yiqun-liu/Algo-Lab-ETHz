#include <cstdio>
#include <vector>
#include <map>

typedef std::vector<int> Layout;

// Reuse previous resuls: compute in a BFS manner
// 00, 10, 20, 30, 01, 11, 21, 31, 02, 12, 22..
void bruteForce(std::vector<Layout> &out, int n, const std::vector<int>& planks)
{
    // initial state, all planks belong to the first square-side
    Layout layout(4, 0);
    for (int i = 0; i < n; i++)
        layout[0] += planks[i];
    out.push_back(layout);
    
    // i indicates level of traverse and which planks to move
    int size = 1;
    for (int i = 0; i < n; i++)
    {
        for (int j = 1; j < 4; j++)
        {
            for (int s = 0; s < size; s++)
            {
                layout = out[s];
                layout[0] -= planks[i];
                layout[j] += planks[i];
                out.push_back(layout);
            }
        }
        size *= 4;
    }
}

// return false when met with invalid layout, can save one search
void getMissing(Layout& out, const Layout& in, int sideLength)
{
    for (int i = 0; i < 4; i++)
        out[i] = sideLength - in[i];
}

bool isValid(Layout layout, int sideLength)
{
    for (int i = 0; i < 4; i++)
        if (layout[i] > sideLength)
            return false;
    return true;
}

void testcase()
{
    int n;
    scanf("%d", &n);
    
    int sum = 0;

    // split: partition [0, n / 2), [n / 2, n)
    // we'll make sure neither subset would be empty after reading up all inputs
    int n1 = n / 2, n2 = n - n1;

    std::vector<int> p1(n1), p2(n2);
    for (int i = 0; i < n1; i++)
    {
        scanf("%d", &p1[i]);
        sum += p1[i];
    }
    for (int i = 0; i < n2; i++)
    {
        scanf("%d", &p2[i]);
        sum += p2[i];
    }
    
    if (n < 4 || sum % 4) // impossible to build square
    {
        printf("0\n");
        return;
    }
    
    int sideLength = sum / 4;
    std::vector<Layout> out1, out2;
    bruteForce(out1, n1, p1);
    bruteForce(out2, n2, p2);

    // list
    // sort out2
    int size = out2.size();
    std::map<Layout, int> map;
    std::map<Layout, int>::iterator it;
    for (int i = 0; i < size; i++)
    {
        if (!isValid(out2[i], sideLength)) // no need to consider
            continue;
        
        it = map.find(out2[i]);
        if (it == map.end())
            map.insert(std::make_pair(out2[i], 1));
        else
            it->second++;
    }

    long long count = 0;
    size = out1.size();
    Layout missed(4);
    for (int i = 0; i < size; i++)
    {
        if (!isValid(out1[i], sideLength)) // no need to consider
            continue;
        
        getMissing(missed, out1[i], sideLength);
        
        it = map.find(missed);
        if (it == map.end())
            continue;
        else
            count += it->second;
    }

    int result = count / (4 * 3 * 2 * 1);
    printf("%d\n", result);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}