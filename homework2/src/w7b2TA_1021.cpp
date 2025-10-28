#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

class Term
{
public:
    double coef = 0.0;
    int exp = 0;
};

void term_quick_sort(Term arr[], int size)
{
    if (size <= 1)
        return;
    auto q_sort = [&](auto &self, int left, int right) mutable -> void
    {
        if (left >= right)
            return;

        int pivot = arr[left].exp;
        int i = left - 1;
        int j = right + 1;

        while (true)
        {
            do
            {
                ++i;
            } while (arr[i].exp < pivot);
            do
            {
                --j;
            } while (arr[j].exp > pivot);

            if (i >= j)
                break;

            swap(arr[i], arr[j]);
        }
        self(self, left, j);
        self(self, j + 1, right);
    };
    q_sort(q_sort, 0, size - 1);
}

void narrow(Term arr[], int size)
{
    for (int i = 0, j = 1; i < size; j = i + 1)
    {
        for (; j < size && arr[i].exp == arr[j].exp; ++j)
        {
            arr[i].coef += arr[j].coef;
            arr[j].coef = 0;
        }
        i = j + 1;
    }
};

void display(Term arr[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (!arr[i].coef)
            continue;
        string str = (arr[i].exp) ? (string("x") + ((arr[i].exp == 1) ? string("") : (string("^") + to_string(arr[i].exp)))) : string("");
        cout << arr[i].coef << str;
        if ((i + 1) < size && arr[i + 1].coef < 0)
            cout << "-";
        else
            cout << "+";
    };
    cout << endl;
}

int main()
{
    int m, n;
    cin >> m >> n;
    Term *ma = new Term[m], *na = new Term[n];
    for (int i = 0; i < m; ++i)
    {
        cin >> ma[i].coef >> ma[i].exp;
    }
    for (int i = 0; i < n; ++i)
    {
        cin >> na[i].coef >> na[i].exp;
    }
    term_quick_sort(ma, m);
}