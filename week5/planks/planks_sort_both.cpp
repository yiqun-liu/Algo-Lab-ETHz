/**
 * Consider the high level of redundancy, I tried sorting both subset.
 * However it turns out sort one side in the best
 */
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
        // test & try
        layout[i] += plank[level];   

        if (layout[i] <= sideLength) // prune: the layout is still valid
        {
            if (level == n - 1) // done: record this layout (Note level start from 0)
                layouts.push_back(layout);
            else
                bruteForce(layouts, layout, plank, sideLength, n, level + 1);
        }

        // restore state
        layout[i] -= plank[level];
    }
}

void getComplement(Layout& dst, const Layout& src, int sideLength)
{
    dst[0] = sideLength - src[0]; dst[1] = sideLength - src[1];
    dst[2] = sideLength - src[2]; dst[3] = sideLength - src[3];
}

void getUnique(vector<Layout>& dst, vector<int>& count, vector<Layout>& src)
{
    dst.push_back(src[0]);
    count.push_back(1);
    for (int i = 1; i < src.size(); i++)
    {
        if (dst.back() == src[i])
            count.back()++;
        else
        {
            dst.push_back(src[i]);
            count.push_back(1);
        }
    }
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
   
    if (nRight == 0) // special case: important
        layouts_R.push_back(Layout(4, 0));
    
    // List
    sort(layouts_L.begin(), layouts_L.end());
    sort(layouts_R.begin(), layouts_R.end());

    // Get rid of redundancy
    if (layouts_L.empty() || layouts_R.empty()) // all combinations are not valid
    {
        printf("0\n");
        return;
    }
    vector<Layout> unique_L, unique_R;
    vector<int> count_L, count_R;
    getUnique(unique_L, count_L, layouts_L);
    getUnique(unique_R, count_R, layouts_R);
    
    Layout complement(4, 0);
    long long count = 0;
    for (int i = 0, j = unique_R.size() - 1; i < unique_L.size(); i++)
    {
        getComplement(complement, unique_L[i], sideLength);
        while (j && complement < unique_R[j])
            j--;
        if (complement == unique_R[j])
            count += count_L[i] * count_R[j];
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