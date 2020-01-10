#include <cstdio>
#include <vector>
#include <algorithm>

const int MAX_N = 30;

typedef std::vector<int> Lights;
class State
{
public:
    State(int m): lights(m), count(0) {};
    
    Lights lights;
    int count; // number of flick

    bool operator< (const State& s) const// ignore count
    {
        int len = lights.size();
        for (int i = 0; i < len; i++)
            if (lights[i] < s.lights[i])
                return true;
            else if (lights[i] > s.lights[i])
                return false;
        return count < s.count;
    }
};

// Utilities
void add(Lights& dst, const Lights& src1, const Lights& src2);
void sub(Lights& dst, const Lights& src1, const Lights& src2);
bool isEqual(const Lights& src1,  const Lights& src2);


void bruteForce(int sw, int n, std::vector<State>& states, const std::vector<Lights>& flick)
{
    int toEnum = 1;
    for (int i = sw; i < sw + n; i++) // switches
    {
        // states of switch (0, 1, ... i-1) has been fully enumerated with (i, i+1, ... n-1) all off
        // now we flick the i^th switch. e.g. 000, 001 --> 000, 001, 010, 011 --> ...
        int base = toEnum;
        for (int j = 0; j < toEnum; j++) // number of state: base + j
        {
            State &off = states[j], &on = states[base + j]; // `off` is know, `on` to compute

            add(on.lights, off.lights, flick[i]); // turn on switch i
            on.count = off.count + 1;
        }
        toEnum *= 2;
    }
}

void testcase()
{
    int n, m;
    scanf("%d%d", &n, &m);

    Lights target(m);
    std::vector< Lights > flick(n, Lights(m));
    
    for (int i = 0; i < m; i++)
        scanf("%d", &target[i]);
    
    for (int i = 0; i < n; i++) // switch
        for (int j = 0; j < m; j++) // room
        {
            int on, off;
            scanf("%d%d", &on, &off);
            flick[i][j] = off - on;
            target[j] -= on; // we only remember how much to change=
        }

    // Initialize state[0] (all lights are off) and flick
    int left = n / 2, right = n - left; // number of switch
    int nLeft = 1 << left, nRight = 1 << right; // number of state
    std::vector<State> states_L(nLeft, State(m)), states_R(nRight, State(m));
    
    bruteForce(0, left, states_L, flick);
    bruteForce(left, right, states_R, flick);

    std::sort(states_R.begin(), states_R.end()); // careful: redundant elements inside

    int minCount = MAX_N + 1;
    State diff(m);
    for (int i = 0; i < nLeft; i++)
    {
        sub(diff.lights, target, states_L[i].lights);
        
        std::vector<State>::iterator it = std::lower_bound(states_R.begin(), states_R.end(), diff);
        if (it == states_R.end() || !isEqual(it->lights, diff.lights))
            continue;

        int count = states_L[i].count + it->count;
        if (count < minCount)
            minCount = count;
    }
    
    if (minCount > MAX_N)
        printf("impossible\n");
    else
        printf("%d\n", minCount);
    
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}

void add(Lights& dst, const Lights& src1, const Lights& src2)
{
    const int len = dst.size();
    for (int i = 0; i < len; i++)
        dst[i] = src1[i] + src2[i];
}
void sub(Lights& dst, const Lights& src1, const Lights& src2)
{
    const int len = dst.size();
    for (int i = 0; i < len; i++)
        dst[i] = src1[i] - src2[i];
}
bool isEqual(const Lights& src1,  const Lights& src2)
{
    const int len = src1.size();
    bool equal = true;
    for (int i = 0; equal && i < len; i++)
        equal = src1[i] == src2[i];
    return equal;
}