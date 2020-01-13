#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using std::vector; using std::cin; using std::cout; using std::endl;

typedef CGAL::Gmpz IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

const int MAX_DEGREE = 30, MAX_SAMPLES = 50;

void splitNumber(int number, int piece, vector<int>& states, vector<vector<int> >& out)
{
    if (piece == 1)
    {
        states.push_back(number);
        out.push_back(states);
        states.pop_back();
    }
    else
        for (int i = 0; i <= number; i++) // this piece
        {
            states.push_back(i);
            splitNumber(number - i, piece - 1, states, out);
            states.pop_back();
        }
}

typedef vector<IT> Powers; // powers of a single number
typedef vector<Powers> PowerList; // power of a group of numbers

void testcase()
{
    int h, t; // health & tumor
    cin >> h >> t;

    vector<bool> alwaysZero(3, true);
    vector<PowerList> powerTable(3, PowerList(h + t)); // variable, sample, exponent

    for (int s = 0; s < h + t; s++) // samples
        for (int i = 0; i < 3; i++) // x, y, z
        {
            int coordinate;
            cin >> coordinate;
            if (coordinate)
                alwaysZero[i] = false;
            
            // precompute all powers
            powerTable[i][s].push_back(1); // exponent = 0
            if (coordinate)
                for (int e = 1; e <= MAX_DEGREE; e++)
                    powerTable[i][s].push_back( coordinate * powerTable[i][s].back() );
            else // for some marginal speedup...
                powerTable[i][s].resize(MAX_DEGREE + 1);
        }
    
    // Unused variables: this is an key optimization
    for (int i = 2; i >= 0; i--) // MUST be done in reverse order, otherwise consecutive erase will crash
        if (alwaysZero[i])
            powerTable.erase( powerTable.begin() + i );
    
    if (h == 0 || t == 0) // 0-degree polynomial suffices.
    {
        cout << 0 << endl;
        return;
    }
    if (powerTable.size() == 0) // all variables are 0
    {
        cout << "Impossible!\n";
        return;
    }

    Program lp(CGAL::SMALLER, false, 0, false, 0); // no bounds on variables
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND); // avoid cycling

    // One-time initialize
    for (int s = 0; s < h + t; s++)
    {
        if (s < h) // healthy
            lp.set_a(0, s, 1);
        else // tumor
            lp.set_a(0, s, -1); // negated
        lp.set_b(s, -1);
    }
        
    int index = 1; // index = 0 is already used by constant;
    // Note that in the loop we only append, previously set parameters are untouched and reused
    for (int d = 1; d <= MAX_DEGREE; d++) // degree
    {
        vector<int> temp;
        vector<vector<int> > out;
        splitNumber(d, powerTable.size(), temp, out); // split degree into exponents' sum

        for (int i = 0; i < out.size(); i++) // one combination of exponents
        {
            for (int s = 0; s < h + t; s++) // samples
            {
                IT term = 1;
                for (int var = 0; var < powerTable.size(); var++) // all non-zero varaibles
                    term *= powerTable[var][s][ out[i][var] ];
                
                if (s < h) // healthy
                    lp.set_a(index, s, term);
                else // tumor
                    lp.set_a(index, s, -term); // negated
            }
            index++;
        }

        Solution s = CGAL::solve_linear_program(lp, ET(), options);
        if (!s.is_infeasible())
        {
            cout << d << endl;
            return;
        }
    }

    cout << "Impossible!\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
    return 0;
}