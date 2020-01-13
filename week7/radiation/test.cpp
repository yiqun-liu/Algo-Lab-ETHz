#include <vector>
#include <iostream>

using namespace std;

void print(vector<int>& v)
{
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
}

int main()
{
    vector<int> a(2, 2), b(3, 1), c(2, 3);
    vector<vector<int> > v;
    v.push_back(a);v.push_back(b);v.push_back(c);

    print(v[0]);
    print(v[1]);
    print(v[2]);
    v.erase(v.begin()+2);
    v.erase(v.begin());
    print(v[0]);
    // print(v[1]);
    cout << v.size();

}