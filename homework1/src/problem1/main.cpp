#include <iostream>
#define MAX_STACK_NUM 200000
using namespace std;

// by recursive
int A(int m, int n)
{
    if (!m)
        return n + 1;
    else if (!n)
        return A(m - 1, 1);
    else
        return A(m - 1, A(m, n - 1));
}


// by non-recursive
int A_nr(int m, int n)
{
    bool is_constant = false;
    int processing[] = {m, n};
    int array[MAX_STACK_NUM];
    int a_end = 0;
    while (true)
    {
        if (is_constant)
        {
            if (!a_end)
            {
                return processing[1];
            }
            else
            {
                processing[0] = array[a_end];
                --a_end;
                is_constant = false;
            }
        }
        else
        {
            if (!processing[0])
            {
                is_constant = true;
                ++processing[1];
            }
            else if (!processing[1])
            {
                --processing[0];
                processing[1] = 1;
            }
            else
            {
                ++a_end;
                array[a_end] = processing[0] - 1;
                --processing[1];
            }
        }
    }
}

int main()
{
    int a, b;
    while (cin >> a >> b)
    {
        int ans = A_nr(a, b);
        cout << "ans:" << ans << endl;
    }
    return 0;
}