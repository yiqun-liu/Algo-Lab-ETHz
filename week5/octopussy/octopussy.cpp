#include <cstdio>
#include <algorithm>

struct Ball
{
    int time;
    bool deactivated;
};

const int MAX_N = 1 << 16;
const int FAILED = false, SUCCESS = true;
Ball ball[MAX_N];
int index[MAX_N]; // index of balls, sorted by time

bool cmpIndex(int idx1, int idx2)
{
    return ball[idx1].time < ball[idx2].time;
}

bool deactivate(int root, int n, int& time) // postorder DFS
{
    int leftChild = 2 * root + 1, rightChild = leftChild + 1;
    
    if (leftChild < n && !ball[leftChild].deactivated)
        if (deactivate(leftChild, n, time) == FAILED)
            return FAILED;
    if (rightChild < n && !ball[rightChild].deactivated)
        if (deactivate(rightChild, n, time) == FAILED)
            return FAILED;
    
    if (time >= ball[root].time) // on time?
        return FAILED;
    
    // Deactivate bomb
    time++;
    ball[root].deactivated = true;

    return SUCCESS;
}

void testcase()
{
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        index[i] = i;
        ball[i].deactivated = false;
        scanf("%d", &ball[i].time);
    }
    
    std::sort(index, index + n, cmpIndex);

    bool possible = true;
    int time = 0;
    for (int i = 0; possible && i < n; i++)
        if (!ball[index[i]].deactivated)
            possible = deactivate(index[i], n, time);

    if (possible)
        printf("yes\n");
    else
        printf("no\n");
    
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}