#include <cstdio>
#include <vector>
#include <algorithm>
using std::vector; using std::sort; using std::lower_bound;

const int MAX_N = 20;
int length[MAX_N];

typedef vector<int> Layout;
typedef vector<Layout>::iterator MyIter;

// I use backtracking because its flexibility to do early pruning
void bruteForce(vector<Layout>& layouts, Layout& layout, const int *plank, int sideLength, int n, int level) // backtracking
{
    for (int i = 0; i < 4; i++) // side
    {
        // try
        layout[i] += plank[level];   
        // test
        if (layout[i] <= sideLength) // prune: the layout is still valid
        {
            if (level == n - 1) // done: record this layout (Note level start from 0)
                layouts.push_back(layout);
            else
                bruteForce(layouts, layout, plank, sideLength, n, level + 1);
        }
        // restore state (undo)
        layout[i] -= plank[level];
    }
}

void getComplement(Layout& dst, const Layout& src, int sideLength)
{
    dst[0] = sideLength - src[0]; dst[1] = sideLength - src[1];
    dst[2] = sideLength - src[2]; dst[3] = sideLength - src[3];
}

void testcase()
{
    int n;
    scanf("%d", &n);

    int sideLength = 0;
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &length[i]);
        sideLength += length[i];
    }
    sideLength /= 4;

    // Split
    const int nLeft = n / 2, nRight = n - nLeft; // nLeft > 0, nRight might = 0
    vector<Layout> layouts_L, layouts_R;
    Layout temp(4, 0);
    layouts_L.reserve(1 << nLeft);
    bruteForce(layouts_L, temp, length, sideLength, nLeft, 0);
    layouts_R.reserve(1 << nRight);
    bruteForce(layouts_R, temp, length + nLeft, sideLength, nRight, 0);
   
    if (nRight == 0) // special case
        layouts_R.push_back(Layout(4, 0));
    
    if (layouts_L.empty() || layouts_R.empty()) // should has, but no valid subset
    {
        printf("0\n");
        return;
    }
    
    // List
    sort(layouts_R.begin(), layouts_R.end());

    // Get rid of redundancy
    vector<Layout> unique_R;
    vector<int> count_R;
    unique_R.push_back(layouts_R[0]);
    count_R.push_back(1);
    for (int i = 1; i < layouts_R.size(); i++)
    {
        if (unique_R.back() == layouts_R[i])
            count_R.back()++;
        else
        {
            unique_R.push_back(layouts_R[i]);
            count_R.push_back(1);
        }
    }
    
    Layout complement(4, 0);
    long long count = 0;
    for (int i = 0; i < layouts_L.size(); i++)
    {
        getComplement(complement, layouts_L[i], sideLength);

        MyIter it = lower_bound(unique_R.begin(), unique_R.end(), complement);
        if (it != unique_R.end() && *it == complement) // Handle potential redundancy
        {
            count += count_R[it - unique_R.begin()];
            it++;
        }
    }
    printf("%lld\n", count / (4 * 3 * 2 * 1));
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}