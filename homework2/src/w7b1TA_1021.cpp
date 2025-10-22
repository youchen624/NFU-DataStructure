#include <iostream>
#include <algorithm>
#include <cmath>

// #include <limits.h>
#define INT_MAX 2147483647

using namespace std;

// swap
/*
template <typename T>
void swap(T &a, T &b)
{
    T c = a;
    a = b;
    b = c;
}
*/

// Term quick-sort by .exp
void term_quick_sort_by_exp(Term arr[], int size)
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

// std::bit_ceil    // #include <bit>
unsigned int _bit_ceil(unsigned int n)
{
    if (!n)
        return 1;
    --n;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    // n |= n >> 32;
    ++n;
    return n;
};

// Term exp less than CMP-function
struct term_isExpLess
{
    bool operator()(const Term &a, const Term &b)
    {
        return (a.exp < b.exp);
    };
};

// CMP should be a compare-function bool(T a, T b)
template <typename T, typename CMP>
class Heap
{
public:
    Heap() {};
    ~Heap() {};
    T pop() {};
    T popush(T replace_) {};

private:
    void _heapify(size_t d_index = 0) {};
    size_t _size;
};

class Polynomial
{
    friend istream &operator>>(istream &input, Polynomial &poly);

public:
    // Construct the polynomial p(x) = 0.
    Polynomial(int length = 0) : capacity(_bit_ceil(length))
    {
        termArray = new Term[length];
    };

    // Copy constructor
    Polynomial(const Polynomial &that) : Polynomial(that.capacity)
    {
        this->terms = that.terms;
        for (int i = 0; i < terms; ++i)
        {
            this->termArray[i] = that.termArray[i];
        }
    };

    // Destructor
    ~Polynomial()
    {
        delete[] termArray;
    };

    // Return the sum of the polynomials *this and poly.
    Polynomial Add(const Polynomial that)
    {
        int temp_capacity = this->capacity + that.capacity;
        if (!temp_capacity)
            return Polynomial();
        int this_ti = 0, that_ti = 0, main_ti = 0;
        Polynomial temp_poly(temp_capacity);
        bool avail_this = (this_ti < this->capacity);
        bool avail_that = (that_ti < that.capacity);
        for (
            ; avail_this || avail_that;
            avail_this = (this_ti < this->capacity),
            avail_that = (that_ti < that.capacity))
        {
            int this_exp = avail_this ? this->termArray[this_ti].exp : INT_MAX;
            int that_exp = avail_that ? that.termArray[that_ti].exp : INT_MAX;
            if (this_exp == that_exp)
            {
                const int temp_coef = this->termArray[this_ti].coef + that.termArray[that_ti].coef;
                ++this_ti;
                ++that_ti;
                if (!temp_coef)
                    continue;
                temp_poly.termArray[main_ti].coef = temp_coef;
                temp_poly.termArray[main_ti].exp = this_exp;
            }
            else if (this_exp < that_exp)
            {
                temp_poly.termArray[main_ti].coef = this->termArray[this_ti].coef;
                temp_poly.termArray[main_ti].exp = this_exp;
                ++this_ti;
            }
            else
            {
                temp_poly.termArray[main_ti].coef = that.termArray[that_ti].coef;
                temp_poly.termArray[main_ti].exp = that_exp;
                ++that_ti;
            }
            ++main_ti;
        }
        if (temp_capacity == main_ti)
            return temp_capacity;
        Polynomial final_poly(main_ti);
        copy(temp_poly.termArray[0], temp_poly.termArray[main_ti - 1], final_poly.termArray);
        return final_poly;
    };

    // Return the product of the polynomials *this and poly.
    Polynomial Mult(Polynomial that)
    {
        ;
    };

    // Evaluate the polynomial *this at f and return the result.
    float Eval(float f)
    {
        float temp = 0;
        for (int i = 0; i < terms; ++i)
        {
            temp += termArray[i].coef * pow(f, termArray[i].exp);
        }
        return temp;
    };

    // add a term
    Polynomial &term_add(float coef, int exp) {};
    Polynomial &term_set(float coef, int exp) {};

    // copy operators
    Polynomial &operator=(Polynomial &that) {};

private:
    void _upgrade_capacity()
    {
        Term *temp_ptr = termArray;
        int tc = capacity;
        capacity *= 2;
        termArray = new Term[capacity];
        copy(temp_ptr, temp_ptr + tc, termArray);
        delete[] temp_ptr;
    };

    void _narrow()  // makes bug
    {
        Term *ar = new Term[capacity];
        int new_terms = terms;
        for (int i = 0; i < terms; ++i)
        {
            ar[i] = termArray[i];
            while (termArray[i].exp != termArray[i + 1].exp)
            {
                ar[i].coef += termArray[i + 1].coef;
                --new_terms;
                if(++i + 1 == terms) break;
            }
        }
        terms = new_terms;
        delete[] termArray;
        termArray = ar;
    };

    // binary search; # return index, -1 if not exists
    int _find(int exp)
    {
        int min_site = 0, max_site = terms - 1, mid;
        if (exp < termArray[min_site].exp || termArray[max_site].exp < exp)
            return -1;
        while (min_site <= max_site)
        {
            mid = (min_site + max_site) / 2;
            int mid_exp = termArray[mid].exp;
            if (exp > mid_exp)
            {
                min_site = mid + 1;
            }
            else if (exp < mid_exp)
            {
                max_site = mid - 1;
            }
            else
            {
                return mid;
            }
        }
        return -1;
    };

    Term *termArray; // array of nonzero terms
    int capacity;    // size of termArray
    int terms;       // number of nonzero terms
};

class Term
{
    friend Polynomial;
    friend struct term_isExpLess;
    friend void term_quick_sort_by_exp(Term arr[], int size);

public:
    Term() : coef(0.0f), exp(0) {};
    // Term operator=(const Term &that) {};

private:
    float coef; // coefficient
    int exp;    // exponent
};

istream &operator>>(const istream &input, Polynomial &poly) {};

int main()
{
    int m, n;
    while (cin >> n >> m)
    {
    }
    return 0;
}