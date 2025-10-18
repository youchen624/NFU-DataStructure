#include <iostream>
#include <algorithm>
#include <cmath>

// #include <limits.h>
#define INT_MAX 2147483647

using namespace std;

/*
#include <bit>
std::bit_ceil
*/
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

class Polynomial
{
public:
    // Construct the polynomial p(x) = 0.
    Polynomial(int length = 0) : capacity(_bit_ceil(length))
    {
        termArray = new Term[length];
    };

    // Copy constructor
    Polynomial(const Polynomial &that) : Polynomial(that.capacity)
    {
        for (int i = 0; i < capacity; ++i)
        {
            this->termArray[i] = that.termArray[i];
        }
    };

    // destructor
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
            avail_that = (that_ti < that.capacity)
        )
        {
            int this_exp = avail_this ? this->termArray[this_ti].exp : INT_MAX;
            int that_exp = avail_that ? that.termArray[that_ti].exp : INT_MAX;
            if (this_exp == that_exp)
            {
                const int temp_coef = this->termArray[this_ti].coef + that.termArray[that_ti].coef;
                ++this_ti;
                ++that_ti;
                if (!temp_coef) continue;
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
    Polynomial Mult(Polynomial poly) {};

    // Evaluate the polynomial *this at f and return the result.
    float Eval(float f)
    {
        float temp = 0;
        for (int i = 0; i < capacity; ++i)
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

public:
    Term() : coef(0.0f), exp(0) {};
    // Term operator=(const Term &that) {};

private:
    float coef; // coefficient
    int exp;    // exponent
};

int main()
{
    return 0;
}