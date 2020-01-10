#include <cstdio>
#include <vector>

const int MAX_N = 30;

typedef std::vector<int> Lights;
class State
{
public:
    State(int m): lights(m), count(0) {};
    
    Lights lights;
    int count; // number of flick
};

// Utilities
void add(Lights& dst, const Lights& src1, const Lights& src2);
bool isEqual(const Lights& src1,  const Lights& src2);

void bruteForce(int n, std::vector<State>& states, const std::vector<Lights>& flick)
{
    int toEnum = 1;
    for (int i = 0; i < n; i++) // light
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
    std::vector< Lights > flick(n, Lights(m) );
    
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
    int nStates = 1 << n;
    std::vector<State> states(nStates, State(m));

    bruteForce(n, states, flick);
    int minCount = MAX_N + 1;
    for (int i = 0; i < nStates; i++)
        if (states[i].count < minCount && isEqual(states[i].lights, target)) // cheap check first
            minCount = states[i].count ;
    
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
bool isEqual(const Lights& src1,  const Lights& src2)
{
    const int len = src1.size();
    bool equal = true;
    for (int i = 0; equal && i < len; i++)
        equal = src1[i] == src2[i];
    return equal;
}