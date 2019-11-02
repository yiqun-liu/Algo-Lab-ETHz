#include <cstdio>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// input & solver types
// typedef int IT;
typedef CGAL::Gmpz IT; // might be (2^10)^30, we need Gmpz
typedef CGAL::Gmpz ET;

// program & solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

const int MAX_DEGREE = 30;
typedef std::vector<IT> PowerList;
typedef std::vector<PowerList> PowerTable;

void calcPowerTable(PowerTable& table);
void generateProgram(Program& lp, int h, int t, int deg, 
    const PowerTable& xTable, const PowerTable& yTable, const PowerTable& zTable);

void testcase()
{
    int h, t;
    scanf("%d%d", &h, &t);

    int nSamples = h + t;
    PowerTable  xTable(nSamples, PowerList(MAX_DEGREE + 1)),
                yTable(nSamples, PowerList(MAX_DEGREE + 1)),
                zTable(nSamples, PowerList(MAX_DEGREE + 1)); // nSamples * 30

    int x, y, z;
    for (int i = 0; i < h; i++)
    {
        scanf("%d%d%d", &x, &y, &z);
        xTable[i][0] = yTable[i][0] = zTable[i][0] = 1; // even when x = 0. This is what we expected
        xTable[i][1] = x; yTable[i][1] = y; zTable[i][1] = z;
    }
    for (int i = h; i < h + t; i++)
    {
        scanf("%d%d%d", &x, &y, &z);
        xTable[i][0] = yTable[i][0] = zTable[i][0] = 1; // even if x = 0, this is what we expected
        xTable[i][1] = x; yTable[i][1] = y; zTable[i][1] = z;
    }
    calcPowerTable(xTable); calcPowerTable(yTable); calcPowerTable(zTable);
    
    Program lp (CGAL::SMALLER, false, 0, false, 0); // lower bound: 0
    
    if (h == 0 || t == 0) 
    {
        printf("0\n");
        return;
    }
    
    // cycling observed in test: use Bland's rule to avoid cycling
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    for (int d = 1; d <= 30; d++) // degree
    {
        generateProgram(lp, h, t, d, xTable, yTable, zTable);
        
        Solution s = CGAL::solve_linear_program(lp, ET(), options);
        if (!s.is_infeasible())  // if solved
        {
            printf("%d\n", d);
            return;
        }
    }
    printf("Impossible!\n");
        
}

int main()
{
    int n;
    scanf("%d", &n);
    while (n--) testcase();
    return 0;
}

void calcPowerTable(PowerTable& table)
{
    for (int i = 0; i < table.size(); i++)
        if (table[i][1] != 0)
            for (int j = 2; j <= MAX_DEGREE; j++)
                table[i][j] = table[i][1] * table[i][j - 1];
        else
            std::fill(table[i].begin() + 1, table[i].end(), 0);
}

void generateProgram(Program& lp, int h, int t, int deg, 
    const PowerTable& xTable, const PowerTable& yTable, const PowerTable& zTable)
{
    // no objective, and no need to do extra settings, because all are strict constrains
    // healthy cells
    for (int row = 0; row < h; row++)
    {
        int col = 0;
        for (int ix = 0; ix <= deg; ix++)
            for (int iy = 0; iy <= deg - ix; iy++)
                for (int iz = 0; iz <= deg - ix - iy; iz++)
                {
                    lp.set_a(col, row, xTable[row][ix] * yTable[row][iy] * zTable[row][iz]);
                    col++;
                }
        lp.set_b(row, -1); // strictly smaller than 0 --> func <= -1 
        // (scales is not important, because there is no concept of unit in this problem)
    }

    // tumor cells
    for (int row = h; row < h + t; row++)
    {
        int col = 0;
        for (int ix = 0; ix <= deg; ix++)
            for (int iy = 0; iy <= deg - ix; iy++)
                for (int iz = 0; iz <= deg - ix - iy; iz++)
                {
                    // negate it to make it a "<= constrains"
                    lp.set_a(col, row, -xTable[row][ix] * yTable[row][iy] * zTable[row][iz]);
                    col++;
                }
        lp.set_b(row, -1); // strictly greater than 0 --> -func <= -1
    }
}