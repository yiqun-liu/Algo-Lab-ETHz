#include <cstdio>
#include <limits>
#include <algorithm>

struct BoatNode
{
    int len;
    int pos;
};
typedef struct BoatNode Boat;

bool cmp(const Boat& b1, const Boat& b2) // sort only by position
{
    return b1.pos < b2.pos;
}

const int MAX_N = 100000;
Boat boats[MAX_N];

void testcase()
{
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
        scanf("%d%d", &boats[i].len, &boats[i].pos);
    std::sort(boats, boats + n, cmp);

    int tail = std::numeric_limits<int>::min(), count = 0;
    for (int i = 0; i < n; i++)
    {
        // Find one with `earliest` ending
        int nextTail = std::max(tail + boats[i].len, boats[i].pos);
        while (i + 1 < n && nextTail > boats[i + 1].pos) // Does current best collides with next boat?
        {
            ++i; // boat_i collides with boat_i+1, should not consider i+1 later on anyway
            int newTail = std::max(tail + boats[i].len, boats[i].pos);
            nextTail = std::min(nextTail, newTail); // update current best
        }
        tail = nextTail;
        count++;
    }

    printf("%d\n", count);   
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}